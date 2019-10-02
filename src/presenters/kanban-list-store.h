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

#include "../models/kanban-data.h"

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define KANBAN_LIST_STORE_TYPE (kanban_list_store_get_type ())

G_DECLARE_FINAL_TYPE (KanbanListStore, kanban_list_store, KANBAN, LIST_STORE, GObject)

KanbanListStore  *kanban_list_store_new             (gint              col_id);
void              kanban_list_store_destroy         (gpointer          vself);

GSequenceIter    *kanban_list_store_new_card        (KanbanListStore  *self,
                                                     const KanbanData *card_data);
GSequence        *kanban_list_store_get_sequence    (KanbanListStore  *self);
GSequenceIter    *kanban_list_store_get_iter_at_pos (KanbanListStore  *self,
                                                     gint              position);
void              kanban_list_store_alert_removed   (KanbanListStore  *self,
                                                     gint              position);
void              kanban_list_store_alert_added     (KanbanListStore  *self,
                                                     gint              position);
void              kanban_list_store_alert_moved     (KanbanListStore  *self,
                                                     gint              old_position,
                                                     gint              new_position);



G_END_DECLS

#endif /* KANBAN_LIST_STORE_H */

