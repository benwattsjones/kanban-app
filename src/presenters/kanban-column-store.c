/* src/presenters/kanban-column-store.c
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

#include "kanban-column-store.h"

#include "kanban-column-viewer-interface.h"
#include "kanban-list-store.h"
#include "kanban-card-viewmodel.h"
#include "model-observer.h"
#include "model-observer-interface.h"
#include "../models/kanban-data.h"

#include <gio/gio.h> // TODO - delete once using kanban-column-viewer-interface
#include <gtk/gtk.h>

struct _KanbanColumnStore
{
  GObject                 parent_instance;

  GHashTable             *card_table; // TODO: will change much if use string card-id
  GHashTable             *column_table;

  ModelObserverInterface *observer_object;
  KanbanColumnViewer     *view_observer;
};

enum
{
  PROP_VIEW_OBSERVER = 1,
  N_PROPERTIES
};

static GParamSpec *obj_properties[N_PROPERTIES] = { NULL, };

G_DEFINE_TYPE (KanbanColumnStore, kanban_column_store, G_TYPE_OBJECT)

// implementation vfuncs for ModelObserverInterface

static void
kanban_column_store_add_card (void              *vself,
                              const KanbanData  *card_data)
{
  KanbanColumnStore *self = vself;
  GSequenceIter *card_iter;
  KanbanListStore *column;
  column = g_hash_table_lookup (self->column_table,
                                GINT_TO_POINTER (card_data->column_id));
  g_assert (column != NULL);
  card_iter = kanban_list_store_new_card (column, card_data); 
  g_hash_table_insert (self->card_table,
                       GINT_TO_POINTER (card_data->card_id), card_iter);
}

static void
kanban_column_store_edit_card (void              *vself,
                               const KanbanData  *card_data)
{
  KanbanColumnStore *self = vself;
  GSequenceIter *card_iter = g_hash_table_lookup (self->card_table, 
                                          GINT_TO_POINTER (card_data->card_id));
  KanbanCardViewModel *card = g_sequence_get (card_iter);
  kanban_card_viewmodel_update_contents (card, card_data->heading, card_data->content);
}

static void
kanban_column_store_move_card (void              *vself,
                               const KanbanData  *card_data)
{
  KanbanColumnStore *self = vself;
  g_print ("Moved card: ID: %d, COLUMN: %d, PRIORITY: %d\n",
           card_data->card_id, card_data->column_id, card_data->priority);
}

static void
kanban_column_store_add_column (void              *vself,
                                const KanbanData  *column_data)
{
  KanbanColumnStore *self = vself;
  KanbanListStore *new_column = kanban_list_store_new (column_data->column_id);
  g_hash_table_insert (self->column_table,
                       GINT_TO_POINTER (column_data->column_id), new_column);
}

static void
kanban_column_store_edit_column (void              *vself,
                                 const KanbanData  *column_data)
{
  KanbanColumnStore *self = vself;
  g_print ("Edited column: ID: %d; HEADING: %s\n",
           column_data->column_id, column_data->heading);
}

static void
kanban_column_store_move_column (void              *vself,
                                 const KanbanData  *column_data)
{
  KanbanColumnStore *self = vself;
  g_print ("Moved column: ID: %d; HEADING: %s\n",
           column_data->column_id, column_data->heading);
}

static ModelObserverInterface *
model_observer_iface_init (KanbanColumnStore *self)
{
  ModelObserverInterface *observer = g_malloc (sizeof *observer);
  observer->viewmodel = self;
  observer->task_func[TASK_ADD_CARD] = kanban_column_store_add_card;
  observer->task_func[TASK_EDIT_CARD] = kanban_column_store_edit_card;
  observer->task_func[TASK_MOVE_CARD] = kanban_column_store_move_card;
  observer->task_func[TASK_ADD_COLUMN] = kanban_column_store_add_column;
  observer->task_func[TASK_EDIT_COLUMN] = kanban_column_store_edit_column;
  observer->task_func[TASK_MOVE_COLUMN] = kanban_column_store_move_column;
  return observer;
}

// Functions for KanbanColumnStore GObject

static void
kanban_column_store_set_property (GObject      *object,
                                  guint         property_id,
                                  const GValue *value,
                                  GParamSpec   *pspec)
{
  KanbanColumnStore *self = KANBAN_COLUMN_STORE (object);

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
kanban_column_store_get_property (GObject    *object,
                                  guint       property_id,
                                  GValue     *value,
                                  GParamSpec *pspec)
{
  KanbanColumnStore *self = KANBAN_COLUMN_STORE (object);

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
kanban_column_store_finalize (GObject *object)
{
  KanbanColumnStore *self = KANBAN_COLUMN_STORE (object);

  deregister_kanban_viewmodel_observer (self->observer_object);
  g_free (self->observer_object);
  g_clear_pointer (&self->column_table, g_hash_table_destroy);
  g_clear_pointer (&self->card_table, g_hash_table_destroy);

  G_OBJECT_CLASS (kanban_column_store_parent_class)->finalize (object);
}

static void
kanban_column_store_init (KanbanColumnStore *self)
{
  self->card_table = g_hash_table_new (g_direct_hash, g_direct_equal);
  self->column_table = g_hash_table_new_full (g_direct_hash, g_direct_equal,
                                              NULL, kanban_list_store_destroy);
  self->observer_object = model_observer_iface_init (self);
  register_kanban_viewmodel_observer (self->observer_object);
}

static void
kanban_column_store_class_init (KanbanColumnStoreClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  object_class->finalize = kanban_column_store_finalize;
  object_class->set_property = kanban_column_store_set_property;
  object_class->get_property = kanban_column_store_get_property;


  obj_properties[PROP_VIEW_OBSERVER] =
    g_param_spec_pointer("view-observer",
                         "View Observer",
                         "GUI observer bound via KanbanColumnViewer iface",
                         G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);

  g_object_class_install_properties (object_class, N_PROPERTIES, obj_properties);
}

KanbanColumnStore *
kanban_column_store_new (KanbanColumnViewer *view_observer)
{
  return g_object_new (KANBAN_COLUMN_STORE_TYPE,
                       "view-observer", view_observer,
                       NULL);
}

void
kanban_column_store_destroy (KanbanColumnStore *self)
{
  g_object_unref (self);
  self = NULL;
}

GListModel * // TODO: temp func before GUI implemented
kanban_column_store_get_card_list (KanbanColumnStore *self)
{
  GList *columns_list = g_hash_table_get_values(self->column_table);
  if (!columns_list)
    return NULL;
  KanbanListStore *column = columns_list->data;
  g_list_free (columns_list);
  return G_LIST_MODEL (column);
}

// Functions used only to provide testing API

#ifdef TESTING_ONLY_ACCESS

KanbanCardViewModel *
kanban_column_store_get_card (KanbanColumnStore *self,
                              gint               card_id)
{
  GSequenceIter *card_iter = g_hash_table_lookup (self->card_table,
                                                  GINT_TO_POINTER (card_id));
  return KANBAN_CARD_VIEWMODEL (g_sequence_get (card_iter));
}

KanbanListStore *
kanban_column_store_get_column (KanbanColumnStore *self,
                                gint               column_id)
{
  KanbanListStore *column = g_hash_table_lookup (self->column_table,
                                                  GINT_TO_POINTER (column_id));
  return column;
}

ModelObserverInterface *
kanban_column_store_get_observer (KanbanColumnStore *self)
{
  return self->observer_object;
}

#endif

