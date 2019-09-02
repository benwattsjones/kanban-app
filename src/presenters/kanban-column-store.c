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
#include "model-observer-interface.h"

#include <gio/gio.h>
#include <gtk/gtk.h>

struct _KanbanColumnStore
{
  GObject          parent_instance;

  GHashTable      *card_table; // TODO: will need to change much if use string card-id

  KanbanListStore *card_list;
  ModelObserverInterface *observer_object;
};

G_DEFINE_TYPE (KanbanColumnStore, kanban_column_store, G_TYPE_OBJECT)

// implementation vfuncs for ModelObserverInterface

static void
kanban_column_store_add_card (void              *vself,
                              const KanbanCard  *card_data)
{
  KanbanColumnStore *self = vself;
  GSequenceIter *card_iter;
  card_iter = kanban_list_store_new_card (self->card_list, card_data); 
  g_hash_table_insert (self->card_table,
                       GINT_TO_POINTER (card_data->card_id), card_iter);
}

static void
kanban_column_store_edit_card (void              *vself,
                               const KanbanCard  *card_data)
{
  KanbanColumnStore *self = vself;
  GSequenceIter *card_iter = g_hash_table_lookup (self->card_table, 
                                          GINT_TO_POINTER (card_data->card_id));
  KanbanCardViewModel *card = g_sequence_get (card_iter);
  kanban_card_viewmodel_update_contents (card, card_data->heading, card_data->content);
}

static void
kanban_column_store_move_card (void              *vself,
                               const KanbanCard  *card_data)
{
  KanbanColumnStore *self = vself;
  g_print ("Moved card: ID: %d, COLUMN: %d, PRIORITY: %d\n",
           card_data->card_id, card_data->column_id, card_data->priority);
}

static void
kanban_column_store_edit_column (void              *vself,
                                 const KanbanCard  *card_data)
{
  KanbanColumnStore *self = vself;
  g_print ("Changed column: ID: %d; HEADING: %s\n",
           card_data->column_id, card_data->heading);
}

// Functions for KanbanColumnStore GObject

static void
kanban_column_store_finalize (GObject *object)
{
  KanbanColumnStore *self = KANBAN_COLUMN_STORE (object);

  deregister_kanban_viewmodel_observer (self->observer_object);
  g_free (self->observer_object);
  kanban_list_store_destroy (self->card_list);
  g_clear_pointer (&self->card_table, g_hash_table_destroy);

  G_OBJECT_CLASS (kanban_column_store_parent_class)->finalize (object);
}

static void
kanban_column_store_init (KanbanColumnStore *self)
{
  self->card_table = g_hash_table_new (g_direct_hash, g_direct_equal);
  self->card_list = kanban_list_store_new (1); // TODO: temp column id and var.

  self->observer_object = (ModelObserverInterface *) 
                              g_malloc (sizeof (ModelObserverInterface));
  self->observer_object->viewmodel = self;
  self->observer_object->add_card = kanban_column_store_add_card;
  self->observer_object->edit_card = kanban_column_store_edit_card;
  self->observer_object->move_card = kanban_column_store_move_card;
  self->observer_object->edit_column = kanban_column_store_edit_column;
  register_kanban_viewmodel_observer (self->observer_object);
}

static void
kanban_column_store_class_init (KanbanColumnStoreClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  object_class->finalize = kanban_column_store_finalize;
}

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

ModelObserverInterface *
kanban_column_store_get_observer (KanbanColumnStore *self)
{
  return self->observer_object;
}

#endif

