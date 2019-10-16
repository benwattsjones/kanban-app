/* src/presenters/kanban-board-presenter.c
 *
 * Copyright (C) 2019 Ben Watts-Jones
 *
 * This file is part of kanban-app.
 *
 * kanban-app is released under the terms of the GNU General Public License,
 * either version 3 of the license, or (at your option) any later version.
 * You should have received a copy of the GNU General Public License along 
 * with kanban-app. If not, see <https://www.gnu.org/licenses/>.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "kanban-board-presenter.h"

#include "kanban-column-viewer-interface.h"
#include "kanban-list-store.h"
#include "kanban-list-viewer-interface.h"
#include "kanban-card-viewmodel.h"
#include "model-observer.h"
#include "model-observer-interface.h"
#include "../models/kanban-data.h"

#include <gio/gio.h>
#include <gtk/gtk.h>

struct _KanbanBoardPresenter
{
  GObject                 parent_instance;

  GHashTable             *card_table; // TODO: will change much if use string card-id
  GHashTable             *column_table;
  GHashTable             *sequence_table;

  ModelObserverInterface *observer_object;
  KanbanBoardObserver    *view_observer;
};

enum
{
  PROP_VIEW_OBSERVER = 1,
  N_PROPERTIES
};

static GParamSpec *obj_properties[N_PROPERTIES] = { NULL, };

G_DEFINE_TYPE (KanbanBoardPresenter, kanban_board_presenter, G_TYPE_OBJECT)

// helper funcs for readability

inline GSequenceIter *
get_card_iter_from_id (KanbanBoardPresenter *self,
                       gint               card_id)
{
  GSequenceIter *card_iter;
  card_iter = g_hash_table_lookup (self->card_table, GINT_TO_POINTER (card_id));
  g_assert (card_iter != NULL);
  return card_iter;
}

KanbanListStore *
get_column_from_card_iter (KanbanBoardPresenter *self,
                           GSequenceIter        *card_iter)
{
  GSequence *seq = g_sequence_iter_get_sequence (card_iter);
  KanbanListStore *column = g_hash_table_lookup (self->sequence_table, seq);
  g_assert (column != NULL);
  return column;
}

KanbanListStore *
get_column_from_id (KanbanBoardPresenter *self,
                    gint                  column_id)
{
  KanbanListStore *column;
  column = g_hash_table_lookup (self->column_table, GINT_TO_POINTER (column_id));
  g_assert (column != NULL);
  return column;
}

// implementation vfuncs for ModelObserverInterface

static void
kanban_board_presenter_add_card (void              *vself,
                                 const KanbanData  *card_data)
{
  KanbanBoardPresenter *self = vself;
  KanbanListStore *column = get_column_from_id (self, card_data->column_id);
  GSequenceIter *card_iter = kanban_list_store_new_card (column, card_data); 
  g_hash_table_insert (self->card_table,
                       GINT_TO_POINTER (card_data->card_id), card_iter);
}

static void
kanban_board_presenter_edit_card (void              *vself,
                                  const KanbanData  *card_data)
{
  KanbanBoardPresenter *self = vself;
  GSequenceIter *card_iter = get_card_iter_from_id (self, card_data->card_id);
  KanbanCardViewModel *card = g_sequence_get (card_iter);
  kanban_card_viewmodel_update_contents (card, card_data->heading, card_data->content);
}

static void
kanban_board_presenter_move_card (void              *vself,
                                  const KanbanData  *card_data)
{
  KanbanBoardPresenter *self = vself;
  GSequenceIter *old_iter = get_card_iter_from_id (self, card_data->card_id);
  KanbanListStore *old_col = get_column_from_card_iter (self, old_iter);
  KanbanListStore *new_col = get_column_from_id (self, card_data->column_id);
  gint old_position = g_sequence_iter_get_position (old_iter);
  gint new_position = card_data->priority;
  GSequenceIter *new_iter = kanban_list_store_get_iter_at_pos (new_col, new_position);

  g_sequence_move (old_iter, new_iter);
  if (new_col != old_col)
    {
      kanban_list_store_alert_removed (old_col, old_position);
      kanban_list_store_alert_added (new_col, new_position);
    }
  else
    {
      kanban_list_store_alert_moved (old_col, old_position, new_position);
    }
}

static void
kanban_board_presenter_add_column (void              *vself,
                                   const KanbanData  *column_data)
{
  KanbanBoardPresenter *self = vself;
  KanbanListStore *new_column = kanban_list_store_new (column_data->column_id,
                                                       column_data->heading);
  g_hash_table_insert (self->column_table,
                       GINT_TO_POINTER (column_data->column_id), new_column);
  GSequence *column_sequence = kanban_list_store_get_sequence (new_column);
  g_hash_table_insert (self->sequence_table, column_sequence, new_column);
  kanban_board_observer_add_column (self->view_observer,
                                    KANBAN_LIST_VIEWER (new_column),
                                    column_data->priority);
}

static void
kanban_board_presenter_edit_column (void              *vself,
                                    const KanbanData  *column_data)
{
  KanbanBoardPresenter *self = vself;
  g_print ("Edited column: ID: %d; HEADING: %s\n",
           column_data->column_id, column_data->heading);
}

static void
kanban_board_presenter_move_column (void              *vself,
                                    const KanbanData  *column_data)
{
  KanbanBoardPresenter *self = vself;
  g_print ("Moved column: ID: %d; HEADING: %s\n",
           column_data->column_id, column_data->heading);
}

static ModelObserverInterface *
model_observer_iface_init (KanbanBoardPresenter *self)
{
  ModelObserverInterface *observer = g_malloc (sizeof *observer);
  observer->viewmodel = self;
  observer->task_func[TASK_ADD_CARD] = kanban_board_presenter_add_card;
  observer->task_func[TASK_EDIT_CARD] = kanban_board_presenter_edit_card;
  observer->task_func[TASK_MOVE_CARD] = kanban_board_presenter_move_card;
  observer->task_func[TASK_ADD_COLUMN] = kanban_board_presenter_add_column;
  observer->task_func[TASK_EDIT_COLUMN] = kanban_board_presenter_edit_column;
  observer->task_func[TASK_MOVE_COLUMN] = kanban_board_presenter_move_column;
  return observer;
}

// Functions for KanbanBoardPresenter GObject

static void
kanban_board_presenter_set_property (GObject      *object,
                                  guint         property_id,
                                  const GValue *value,
                                  GParamSpec   *pspec)
{
  KanbanBoardPresenter *self = KANBAN_BOARD_PRESENTER (object);

  switch (property_id)
    {
    case PROP_VIEW_OBSERVER:
      self->view_observer = g_value_get_pointer (value);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}

static void
kanban_board_presenter_get_property (GObject    *object,
                                  guint       property_id,
                                  GValue     *value,
                                  GParamSpec *pspec)
{
  KanbanBoardPresenter *self = KANBAN_BOARD_PRESENTER (object);

  switch (property_id)
    {
    case PROP_VIEW_OBSERVER:
      g_value_set_pointer (value, self->view_observer);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}

static void
kanban_board_presenter_finalize (GObject *object)
{
  KanbanBoardPresenter *self = KANBAN_BOARD_PRESENTER (object);

  deregister_kanban_viewmodel_observer (self->observer_object);
  g_free (self->observer_object);
  g_clear_pointer (&self->column_table, g_hash_table_destroy);
  g_clear_pointer (&self->sequence_table, g_hash_table_destroy);
  g_clear_pointer (&self->card_table, g_hash_table_destroy);

  G_OBJECT_CLASS (kanban_board_presenter_parent_class)->finalize (object);
}

static void
kanban_board_presenter_init (KanbanBoardPresenter *self)
{
  self->card_table = g_hash_table_new (g_direct_hash, g_direct_equal);
  self->sequence_table = g_hash_table_new (g_direct_hash, g_direct_equal);
  self->column_table = g_hash_table_new_full (g_direct_hash, g_direct_equal,
                                              NULL, kanban_list_store_destroy);
  self->observer_object = model_observer_iface_init (self);
  register_kanban_viewmodel_observer (self->observer_object);
}

static void
kanban_board_presenter_class_init (KanbanBoardPresenterClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  object_class->finalize = kanban_board_presenter_finalize;
  object_class->set_property = kanban_board_presenter_set_property;
  object_class->get_property = kanban_board_presenter_get_property;


  obj_properties[PROP_VIEW_OBSERVER] =
    g_param_spec_pointer("view-observer",
                         "View Observer",
                         "GUI observer bound via KanbanBoardObserver iface",
                         G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);

  g_object_class_install_properties (object_class, N_PROPERTIES, obj_properties);
}

KanbanBoardPresenter *
kanban_board_presenter_new (KanbanBoardObserver *view_observer)
{
  return g_object_new (KANBAN_TYPE_BOARD_PRESENTER,
                       "view-observer", view_observer,
                       NULL);
}

void
kanban_board_presenter_destroy (KanbanBoardPresenter *self)
{
  g_object_unref (self);
  self = NULL;
}

// Functions used only to provide testing API

#ifdef TESTING_ONLY_ACCESS

KanbanCardViewModel *
kanban_board_presenter_get_card (KanbanBoardPresenter *self,
                                 gint               card_id)
{
  GSequenceIter *card_iter = g_hash_table_lookup (self->card_table,
                                                  GINT_TO_POINTER (card_id));
  return KANBAN_CARD_VIEWMODEL (g_sequence_get (card_iter));
}

KanbanListStore *
kanban_board_presenter_get_column (KanbanBoardPresenter *self,
                                   gint               column_id)
{
  KanbanListStore *column = g_hash_table_lookup (self->column_table,
                                                  GINT_TO_POINTER (column_id));
  return column;
}

ModelObserverInterface *
kanban_board_presenter_get_observer (KanbanBoardPresenter *self)
{
  return self->observer_object;
}

#endif

