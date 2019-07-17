/* src/presenters/kanban-tree-store.h
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

#ifndef KANBAN_TREE_STORE_H
#define KANBAN_TREE_STORE_H

#include "../models/kanban-cards.h"

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define KANBAN_TREE_STORE_TYPE (kanban_tree_store_get_type ())

G_DECLARE_FINAL_TYPE (KanbanTreeStore, kanban_tree_store, KANBAN, TREE_STORE, GtkTreeStore)

KanbanTreeStore  *initialize_viewmodel  ();
void              destroy_viewmodel                (KanbanTreeStore   *viewmodel);

void              kanban_tree_store_change_column  (KanbanTreeStore   *self,
                                                    gint               column_id,
                                                    gchar             *heading);
void              kanban_tree_store_change_content (KanbanTreeStore   *self,
                                                    gint               card_id,
                                                    gchar             *heading,
                                                    gchar             *content);
void              kanban_tree_store_move_card      (KanbanTreeStore   *self,
                                                    gint               card_id,
                                                    gint               column_id,
                                                    gint               priority);
void              kanban_tree_store_new_card       (KanbanTreeStore   *self,
                                                    const KanbanCard  *card_data);



G_END_DECLS

#endif /* KANBAN_TREE_STORE_H */

