/* src/presenters/kanban-column-store.h
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

#ifndef KANBAN_COLUMN_STORE_H
#define KANBAN_COLUMN_STORE_H

#include "kanban-list-store.h"
#include "../models/kanban-cards.h"

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define KANBAN_COLUMN_STORE_TYPE (kanban_column_store_get_type ())

G_DECLARE_FINAL_TYPE (KanbanColumnStore, kanban_column_store, KANBAN, COLUMN_STORE, GObject)

KanbanColumnStore   *kanban_column_store_new           ();
void                 kanban_column_store_destroy       (KanbanColumnStore *self);

KanbanListStore     *kanban_column_store_get_card_list (KanbanColumnStore *self);

void                 kanban_column_store_add_card      (KanbanColumnStore *self,
                                                        const KanbanCard  *card_data);
void                 kanban_column_store_edit_card     (KanbanColumnStore *self,
                                                        const KanbanCard  *card_data);
void                 kanban_column_store_move_card     (KanbanColumnStore *self,
                                                        const KanbanCard  *card_data);
void                 kanban_column_store_edit_column   (KanbanColumnStore *self,
                                                        const KanbanCard  *card_data);

#ifdef TESTING_ONLY_ACCESS
#include "kanban-card-viewmodel.h"
KanbanCardViewModel *kanban_column_store_get_card      (KanbanColumnStore *self,
                                                        gint               card_id);
#endif

G_END_DECLS

#endif /* KANBAN_COLUMN_STORE_H */

