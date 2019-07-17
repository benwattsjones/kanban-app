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
void              viewmodel_change_card (const KanbanCard  *card_data);
void              destroy_viewmodel     (KanbanTreeStore   *viewmodel);

G_END_DECLS

#endif /* KANBAN_TREE_STORE_H */

