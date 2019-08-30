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

#include <gtk/gtk.h>

struct _KanbanColumnStore
{
  GObject          parent_instance;

  KanbanListStore *card_list;
};

G_DEFINE_TYPE (KanbanColumnStore, kanban_column_store, G_TYPE_OBJECT)

static void
kanban_column_store_finalize (GObject *object)
{
  KanbanColumnStore *self = KANBAN_COLUMN_STORE (object);
  destroy_viewmodel (self->card_list);
  G_OBJECT_CLASS (kanban_column_store_parent_class)->finalize (object);
}

static void
kanban_column_store_init (KanbanColumnStore *self)
{
  self->card_list = initialize_viewmodel (1); // TODO: temp column id and var.
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
