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

//#define KANBAN_TREE_APPEND_TO_END NULL
//#define END_TREE_SET_INSERSION -1

#include "kanban-list-store.h"

#include "kanban-card-viewmodel.h"
#include "model-observer.h"
#include "../models/kanban-cards.h"

#include <gio/gio.h>
#include <gtk/gtk.h>

// TODO: Clean up and remove old comments from change.

/* NOTE:
 *  - columns in kanban board physically, are not the same as columns in tree
 *  - Intented to use the same struct for cards and columns, identifying
 *    columns as having card_id of 0.
 */

/*
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
*/

struct _KanbanListStore
{
  GObject               parent_instance;

  guint                 num_cards;
  gchar               **kanban_column_names;

  KanbanCardViewModel  *card_list[10]; // TODO: replace stub with GHashTable /similer
};

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
  return (g_object_ref (KANBAN_LIST_STORE (model))->card_list)[i];
}

static void
g_list_model_iface_init (GListModelInterface *iface)
{
  iface->get_item_type = kanban_list_model_get_type;
  iface->get_n_items = kanban_list_model_get_n_items;
  iface->get_item = kanban_list_model_get_item;
}

void
kanban_list_store_change_column (KanbanListStore *self,
                                 gint             column_id,
                                 gchar           *heading)
{
  g_print ("Changed column: ID: %d; HEADING: %s\n", column_id, heading);
}

void
kanban_list_store_change_content (KanbanListStore *self,
                                  gint             card_id,
                                  gchar           *heading,
                                  gchar           *content)
{
  g_print ("Changed card content: ID: %d; HEADING: %s; CONTENT: %s\n", 
           card_id, heading, content);
}

void
kanban_list_store_move_card (KanbanListStore *self,
                             gint             card_id,
                             gint             column_id,
                             gint             priority)
{
  g_print ("Moved card: ID: %d, COLUMN: %d, PRIORITY: %d\n",
           card_id, column_id, priority);
}

void
kanban_list_store_new_card (KanbanListStore  *self,
                            const KanbanCard *card_data)
{
  g_print ("New card: HEADING: %s; CONTENT: %s\n",
           card_data->heading, card_data->content);
}

static void
kanban_list_store_finalize (GObject *object)
{
  KanbanListStore *self = KANBAN_LIST_STORE (object);
  deregister_kanban_viewmodel_observer (self);
  g_print ("finalize KanbanListStore called\n");
  G_OBJECT_CLASS (kanban_list_store_parent_class)->finalize (object);
}

static void
kanban_list_store_init (KanbanListStore *self)
{
/*
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
*/
  register_kanban_viewmodel_observer (self);
}

static void
kanban_list_store_class_init (KanbanListStoreClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  object_class->finalize = kanban_list_store_finalize;
}


KanbanListStore *
initialize_viewmodel ()
{
  return g_object_new (KANBAN_LIST_STORE_TYPE, NULL);
}

void
destroy_viewmodel (KanbanListStore *viewmodel)
{
  g_object_unref (viewmodel);
  viewmodel = NULL;
}

