/* src/presenters/kanban-list-store.h
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

#ifndef KANBAN_LIST_STORE_H
#define KANBAN_LIST_STORE_H

#include "../models/kanban-cards.h"

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define KANBAN_LIST_STORE_TYPE (kanban_list_store_get_type ())

G_DECLARE_FINAL_TYPE (KanbanListStore, kanban_list_store, KANBAN, LIST_STORE, GObject)

KanbanListStore  *initialize_viewmodel             (gint               col_id);
void              destroy_viewmodel                (KanbanListStore   *viewmodel);

void              kanban_list_store_change_column  (KanbanListStore   *self,
                                                    const KanbanCard  *card_data);
void              kanban_list_store_change_content (KanbanListStore   *self,
                                                    const KanbanCard  *card_data);
void              kanban_list_store_move_card      (KanbanListStore   *self,
                                                    const KanbanCard  *card_data);
void              kanban_list_store_new_card       (KanbanListStore   *self,
                                                    const KanbanCard  *card_data);



G_END_DECLS

#endif /* KANBAN_LIST_STORE_H */

