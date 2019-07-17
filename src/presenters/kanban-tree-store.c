/* src/presenters/kanban-tree-store.c
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

#define KANBAN_TREE_APPEND_TO_END NULL
#define END_TREE_SET_INSERSION -1

#include "kanban-tree-store.h"

#include "model-observer.h"
#include "../models/kanban-cards.h"

#include <gtk/gtk.h>



/* NOTE:
 *  - columns in kanban board physically, are not the same as columns in tree
 *  - Intented to use the same struct for cards and columns, identifying
 *    columns as having card_id of 0.
 */

enum
{
  COLUMN_ID_COLUMN = 0,
  CARD_ID_COLUMN,
  HEADING_COLUMN,
  CONTENT_COLUMN,
  PRIORITY_COLUMN,

  VISIBLE_COLUMN,
  NUM_COLUMNS
};

static char *column_names[] = {"Backlog", "Soon", "In Progress", "On Hold", 
                               "Done", NULL};

struct _KanbanTreeStore
{
  GtkTreeStore  parent_instance;
  gchar       **kanban_column_names;
};

G_DEFINE_TYPE (KanbanTreeStore, kanban_tree_store, GTK_TYPE_TREE_STORE)

static void
kanban_tree_store_finalize (GObject *object)
{
  KanbanTreeStore *self = KANBAN_TREE_STORE (object);
  deregister_kanban_viewmodel_observer (self);
  g_print ("finalize KanbanTreeStore called\n");
  G_OBJECT_CLASS (kanban_tree_store_parent_class)->finalize (object);
}

static void
kanban_tree_store_init (KanbanTreeStore *self)
{
  GtkTreeStore *tree = &self->parent_instance;
  GtkTreeIter iter;
  gint column_iter = 0;
  char *kanban_column_name;
  GType tree_col_types[] = {G_TYPE_INT, G_TYPE_INT, G_TYPE_STRING, G_TYPE_STRING,
                           G_TYPE_INT, G_TYPE_BOOLEAN};

  gtk_tree_store_set_column_types (tree, NUM_COLUMNS, tree_col_types);

  // replace with call to model
  self->kanban_column_names = column_names;

  while (TRUE)
    {
      kanban_column_name = self->kanban_column_names[column_iter];
      if (!kanban_column_name)
        break;
      ++column_iter;
      gtk_tree_store_append (tree, &iter, KANBAN_TREE_APPEND_TO_END);
      gtk_tree_store_set (tree, &iter,
                          COLUMN_ID_COLUMN, column_iter,
                          CARD_ID_COLUMN, 0,
                          HEADING_COLUMN, kanban_column_name,
                          CONTENT_COLUMN, NULL,
                          PRIORITY_COLUMN, column_iter,
                          VISIBLE_COLUMN, FALSE,
                          END_TREE_SET_INSERSION);
    }
  register_kanban_viewmodel_observer (self);
}

static void
kanban_tree_store_class_init (KanbanTreeStoreClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  object_class->finalize = kanban_tree_store_finalize;
}


KanbanTreeStore *
initialize_viewmodel ()
{
  return g_object_new (KANBAN_TREE_STORE_TYPE, NULL);
}

void
destroy_viewmodel (KanbanTreeStore *viewmodel)
{
  g_object_unref (viewmodel);
  viewmodel = NULL;
}

void
viewmodel_change_card (const KanbanCard *card_data)
{
  g_print ("Signal Recieved:\nHeading: %s\nContent: %s\n",
           card_data->heading, card_data->content);
}

