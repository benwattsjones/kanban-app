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

#include "kanban-list-store.h"
#include "kanban-card-viewmodel.h"
#include "model-observer.h"

#include <gio/gio.h>
#include <gtk/gtk.h>

struct _KanbanColumnStore
{
  GObject          parent_instance;

  GHashTable      *card_table; // TODO: will need to change much if use string card-id

  KanbanListStore *card_list;
};

G_DEFINE_TYPE (KanbanColumnStore, kanban_column_store, G_TYPE_OBJECT)

static void
kanban_column_store_finalize (GObject *object)
{
  KanbanColumnStore *self = KANBAN_COLUMN_STORE (object);

  deregister_kanban_viewmodel_observer (self);
  kanban_list_store_destroy (self->card_list);
  g_clear_pointer (&self->card_table, g_hash_table_destroy);

  G_OBJECT_CLASS (kanban_column_store_parent_class)->finalize (object);
}

static void
kanban_column_store_init (KanbanColumnStore *self)
{
  self->card_table = g_hash_table_new (g_direct_hash, g_direct_equal);
  self->card_list = kanban_list_store_new (1); // TODO: temp column id and var.
  register_kanban_viewmodel_observer (self);
}

static void
kanban_column_store_class_init (KanbanColumnStoreClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  object_class->finalize = kanban_column_store_finalize;
}

// Functions to act on object:

KanbanColumnStore *
kanban_column_store_new ()
{
  return g_object_new (KANBAN_COLUMN_STORE_TYPE, NULL);
}

void
kanban_column_store_destroy (KanbanColumnStore *self)
{
  g_object_unref (self);
  self = NULL;
}

KanbanListStore *
kanban_column_store_get_card_list (KanbanColumnStore *self)
{
  return self->card_list;
}

void
kanban_column_store_add_card (KanbanColumnStore *self,
                              const KanbanCard  *card_data)
{
  GSequenceIter *card_iter;
  card_iter = kanban_list_store_new_card (self->card_list, card_data); 
  g_hash_table_insert (self->card_table,
                       GINT_TO_POINTER (card_data->card_id), card_iter);
}

void
kanban_column_store_edit_card (KanbanColumnStore *self,
                               const KanbanCard  *card_data)
{
  GSequenceIter *card_iter = g_hash_table_lookup (self->card_table, 
                                          GINT_TO_POINTER (card_data->card_id));
  KanbanCardViewModel *card = g_sequence_get (card_iter);
  kanban_card_viewmodel_update_contents (card, card_data->heading, card_data->content);
}

void
kanban_column_store_move_card (KanbanColumnStore *self,
                               const KanbanCard  *card_data)
{
  g_print ("Moved card: ID: %d, COLUMN: %d, PRIORITY: %d\n",
           card_data->card_id, card_data->column_id, card_data->priority);
}

void
kanban_column_store_edit_column (KanbanColumnStore *self,
                                 const KanbanCard  *card_data)
{
  g_print ("Changed column: ID: %d; HEADING: %s\n",
           card_data->column_id, card_data->heading);
}

