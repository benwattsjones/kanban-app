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

#include <gtk/gtk.h>

#include "kanban-tree-store.h"
#include "../models/kanban-cards.h"

#include "model-observer.h"

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

GtkTreeStore *
initialize_viewmodel ()
{
  GtkTreeStore *viewmodel;
  GtkTreeIter iter;
  gint column_iter = 0;
  char *column_name;

  viewmodel = gtk_tree_store_new (NUM_COLUMNS,
                                  G_TYPE_INT,
                                  G_TYPE_INT,
                                  G_TYPE_STRING,
                                  G_TYPE_STRING,
                                  G_TYPE_INT,
                                  G_TYPE_BOOLEAN);
  while (TRUE)
    {
      column_name = column_names[column_iter];
      if (!column_name)
        break;
      ++column_iter;
      gtk_tree_store_append (viewmodel, &iter, KANBAN_TREE_APPEND_TO_END);
      gtk_tree_store_set (viewmodel, &iter,
                          COLUMN_ID_COLUMN, column_iter,
                          CARD_ID_COLUMN, 0,
                          HEADING_COLUMN, column_name,
                          CONTENT_COLUMN, NULL,
                          PRIORITY_COLUMN, column_iter,
                          VISIBLE_COLUMN, FALSE,
                          END_TREE_SET_INSERSION);
    }

  register_kanban_viewmodel_observer (viewmodel);
  return viewmodel;
}

void
destroy_viewmodel (GtkTreeStore *viewmodel)
{
  deregister_kanban_viewmodel_observer (viewmodel);
  // free - link to eventual destructor
}

void
viewmodel_change_card (const KanbanCard *card_data)
{
  g_print ("Signal Recieved:\nHeading: %s\nContent: %s\n",
           card_data->heading, card_data->content);
}

