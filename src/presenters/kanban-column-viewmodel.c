/* src/presenters/kanban-column-viewmodel.c
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

#include "kanban-column-viewmodel.h"

#include "kanban-list-viewer-interface.h"
#include "kanban-card-viewmodel.h"
#include "../models/kanban-data.h"

#include <gio/gio.h>
#include <gtk/gtk.h>

struct _KanbanColumnViewModel
{
  GObject         parent_instance;

  guint           num_cards;
  gint            column_id;
  GtkTextBuffer  *column_name;
  GSequence      *card_list;
};

enum
{
  PROP_COLUMN_ID = 1,
  PROP_COLUMN_NAME,
  N_PROPERTIES
};

static GParamSpec *obj_properties[N_PROPERTIES] = { NULL, };

static void g_list_model_iface_init (GListModelInterface *iface);
static void kanban_list_viewer_iface_init (KanbanListViewerInterface *iface);

G_DEFINE_TYPE_WITH_CODE (KanbanColumnViewModel, kanban_column_viewmodel, G_TYPE_OBJECT,
                         G_IMPLEMENT_INTERFACE (G_TYPE_LIST_MODEL,
                                                g_list_model_iface_init)
                         G_IMPLEMENT_INTERFACE (KANBAN_LIST_VIEWER_TYPE,
                                                kanban_list_viewer_iface_init))

/* GListModel iface */

static GType
kanban_column_viewmodel_get_item_type (GListModel *model)
{
  (void) model;
  return KANBAN_TYPE_CARD_VIEWMODEL;
}

static guint
kanban_column_viewmodel_get_n_items (GListModel *model)
{
  return (KANBAN_COLUMN_VIEWMODEL (model))->num_cards;
}

static gpointer
kanban_column_viewmodel_get_item (GListModel *model,
                                  guint       i)
{
  KanbanColumnViewModel *self = KANBAN_COLUMN_VIEWMODEL (model);
  if (i >= self->num_cards)
    return NULL;
  GSequenceIter *iter = g_sequence_get_iter_at_pos (self->card_list, i);
  KanbanCardViewModel *card =  g_sequence_get(iter);
  return g_object_ref (card);
}

static void
g_list_model_iface_init (GListModelInterface *iface)
{
  iface->get_item_type = kanban_column_viewmodel_get_item_type;
  iface->get_n_items = kanban_column_viewmodel_get_n_items;
  iface->get_item = kanban_column_viewmodel_get_item;
}

static GtkTextBuffer *
kanban_column_viewmodel_get_heading (KanbanListViewer *model)
{
  return KANBAN_COLUMN_VIEWMODEL (model)->column_name;
}

static void
kanban_list_viewer_iface_init (KanbanListViewerInterface *iface)
{
  iface->get_heading = kanban_column_viewmodel_get_heading;
}

/* funcs for class */

static void
kanban_column_viewmodel_set_property (GObject      *object,
                                      guint         property_id,
                                      const GValue *value,
                                      GParamSpec   *pspec)
{
  KanbanColumnViewModel *self = KANBAN_COLUMN_VIEWMODEL (object);

  switch (property_id)
    {
    case PROP_COLUMN_ID:
      self->column_id = g_value_get_int (value);
      break;

    case PROP_COLUMN_NAME:
      gtk_text_buffer_set_text (self->column_name, g_value_get_string (value), -1);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}

static void
kanban_column_viewmodel_get_property (GObject    *object,
                                      guint       property_id,
                                      GValue     *value,
                                      GParamSpec *pspec)
{
  KanbanColumnViewModel *self = KANBAN_COLUMN_VIEWMODEL (object);
  GtkTextIter start, end;
  gchar *buffer_text = NULL;

  switch (property_id)
    {
    case PROP_COLUMN_ID:
      g_value_set_int (value, self->column_id);
      break;

    case PROP_COLUMN_NAME:
      gtk_text_buffer_get_bounds (self->column_name, &start, &end);
      buffer_text = gtk_text_buffer_get_text (self->column_name, &start, &end, FALSE);
      g_value_set_string (value, buffer_text);
      g_free (buffer_text);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}

static void
kanban_column_viewmodel_finalize (GObject *object)
{
  KanbanColumnViewModel *self = KANBAN_COLUMN_VIEWMODEL (object);

  g_clear_pointer (&self->card_list, g_sequence_free);
  g_object_unref (self->column_name);

  G_OBJECT_CLASS (kanban_column_viewmodel_parent_class)->finalize (object);
}

static void
kanban_column_viewmodel_init (KanbanColumnViewModel *self)
{
  self->num_cards = 0;
  self->card_list = g_sequence_new (g_object_unref);
  self->column_name = gtk_text_buffer_new (NULL);
}

static void
kanban_column_viewmodel_class_init (KanbanColumnViewModelClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->set_property = kanban_column_viewmodel_set_property;
  object_class->get_property = kanban_column_viewmodel_get_property;
  object_class->finalize = kanban_column_viewmodel_finalize;

  obj_properties[PROP_COLUMN_ID] =
    g_param_spec_int("column-id",
                     "Column ID", 
                     "Unique, immutable kanban column identifier",
                     0, G_MAXINT, 0,
                     G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);
  obj_properties[PROP_COLUMN_NAME] =
    g_param_spec_string("column-name",
                        "Column Name",
                        "Name of column to display above cards list",
                        NULL,
                        G_PARAM_CONSTRUCT | G_PARAM_READWRITE);

  g_object_class_install_properties (object_class, N_PROPERTIES, obj_properties);
}

// Funcs to act on object:

KanbanColumnViewModel *
kanban_column_viewmodel_new (gint         col_id,
                             const gchar *col_name)
{
  const gchar *heading = col_name ? col_name : "";
  return g_object_new (KANBAN_TYPE_COLUMN_VIEWMODEL,
                       "column-id", col_id,
                       "column-name", heading,
                       NULL);
}

void
kanban_column_viewmodel_destroy (gpointer vself)
{
  KanbanColumnViewModel *self = KANBAN_COLUMN_VIEWMODEL (vself);
  g_object_unref (self);
  self = NULL;
}

GSequenceIter *
kanban_column_viewmodel_new_card (KanbanColumnViewModel *self,
                                  const KanbanData      *card_data)
{
  KanbanCardViewModel *new_card = kanban_card_viewmodel_new(card_data);
  GSequenceIter *iter = g_sequence_get_iter_at_pos (self->card_list,
                                                    card_data->priority);
  iter = g_sequence_insert_before (iter, new_card);
  self->num_cards++;
  guint position = g_sequence_iter_get_position (iter);
  g_list_model_items_changed (G_LIST_MODEL (self), position, 0, 1);
  return iter;
}

GSequence *
kanban_column_viewmodel_get_sequence (KanbanColumnViewModel *self)
{
  return self->card_list;
}

void
kanban_column_viewmodel_move_card (KanbanColumnViewModel *current_column,
                                   KanbanColumnViewModel *new_column,
                                   GSequenceIter         *card_iter,
                                   gint                   new_position)
{
  gint old_position = g_sequence_iter_get_position (card_iter);
  GSequenceIter *new_iter = g_sequence_get_iter_at_pos (new_column->card_list,
                                                        new_position);
  g_sequence_move (card_iter, new_iter);

  if (new_column != current_column)
    {
      current_column->num_cards--;
      new_column->num_cards++;
    }
  g_list_model_items_changed (G_LIST_MODEL (current_column), old_position, 1, 0);
  g_list_model_items_changed (G_LIST_MODEL (new_column), new_position, 0, 1);
}

