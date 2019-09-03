/* src/presenters/kanban-list-store.c
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

#include "kanban-list-store.h"

#include "kanban-card-viewmodel.h"
#include "../models/kanban-data.h"

#include <gio/gio.h>
#include <gtk/gtk.h>

struct _KanbanListStore
{
  GObject     parent_instance;

  guint       num_cards;
  gint        column_id;

  GSequence  *card_list;
};

enum
{
  PROP_COLUMN_ID = 1,
  N_PROPERTIES
};

static GParamSpec *obj_properties[N_PROPERTIES] = { NULL, };

static void g_list_model_iface_init (GListModelInterface *iface);

G_DEFINE_TYPE_WITH_CODE (KanbanListStore, kanban_list_store, G_TYPE_OBJECT,
                         G_IMPLEMENT_INTERFACE (G_TYPE_LIST_MODEL,
                                                g_list_model_iface_init))

/* GListModel iface */

static GType
kanban_list_model_get_type (GListModel *model)
{
  (void) model;
  return KANBAN_CARD_VIEWMODEL_TYPE;
}

static guint
kanban_list_model_get_n_items (GListModel *model)
{
  return (KANBAN_LIST_STORE (model))->num_cards;
}

static gpointer
kanban_list_model_get_item (GListModel *model,
                            guint       i)
{
  GSequenceIter *iter;
  KanbanCardViewModel *card;
  iter = g_sequence_get_iter_at_pos (KANBAN_LIST_STORE (model)->card_list, i);
  card =  g_sequence_get(iter);
  return g_object_ref (card);
}

static void
g_list_model_iface_init (GListModelInterface *iface)
{
  iface->get_item_type = kanban_list_model_get_type;
  iface->get_n_items = kanban_list_model_get_n_items;
  iface->get_item = kanban_list_model_get_item;
}

/* funcs for class */

static void
kanban_list_store_set_property (GObject      *object,
                                guint         property_id,
                                const GValue *value,
                                GParamSpec   *pspec)
{
  KanbanListStore *self = KANBAN_LIST_STORE (object);

  switch (property_id)
    {
    case PROP_COLUMN_ID:
      self->column_id = g_value_get_int (value);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}

static void
kanban_list_store_get_property (GObject    *object,
                                guint       property_id,
                                GValue     *value,
                                GParamSpec *pspec)
{
  KanbanListStore *self = KANBAN_LIST_STORE (object);

  switch (property_id)
    {
    case PROP_COLUMN_ID:
      g_value_set_int (value, self->column_id);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}

static void
kanban_list_store_finalize (GObject *object)
{
  KanbanListStore *self = KANBAN_LIST_STORE (object);

  g_clear_pointer (&self->card_list, g_sequence_free);

  G_OBJECT_CLASS (kanban_list_store_parent_class)->finalize (object);
}

static void
kanban_list_store_init (KanbanListStore *self)
{
  self->num_cards = 0;
  self->card_list = g_sequence_new (g_object_unref);
}

static void
kanban_list_store_class_init (KanbanListStoreClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->set_property = kanban_list_store_set_property;
  object_class->get_property = kanban_list_store_get_property;
  object_class->finalize = kanban_list_store_finalize;

  obj_properties[PROP_COLUMN_ID] =
    g_param_spec_int("column-id",
                     "Column ID", 
                     "Unique, immutable kanban column identifier",
                     0, G_MAXINT, 0,
                     G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);
  g_object_class_install_properties (object_class, N_PROPERTIES, obj_properties);
}

// Funcs to act on object:

KanbanListStore *
kanban_list_store_new (gint col_id)
{
  return g_object_new (KANBAN_LIST_STORE_TYPE,
                       "column-id", col_id,
                       NULL);
}

void
kanban_list_store_destroy (gpointer vself)
{
  KanbanListStore *self = KANBAN_LIST_STORE (vself);
  g_object_unref (self);
  self = NULL;
}

GSequenceIter *
kanban_list_store_new_card (KanbanListStore  *self,
                            const KanbanData *card_data)
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

