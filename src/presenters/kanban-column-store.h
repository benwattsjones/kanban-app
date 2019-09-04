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

#include "kanban-column-viewer-interface.h"

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define KANBAN_COLUMN_STORE_TYPE (kanban_column_store_get_type ())

G_DECLARE_FINAL_TYPE (KanbanColumnStore, kanban_column_store, KANBAN, COLUMN_STORE, GObject)

KanbanColumnStore      *kanban_column_store_new           (KanbanColumnViewer *view_observer);
void                    kanban_column_store_destroy       (KanbanColumnStore  *self);

GListModel             *kanban_column_store_get_card_list (KanbanColumnStore  *self);


#ifdef TESTING_ONLY_ACCESS
#include "kanban-card-viewmodel.h"
KanbanCardViewModel    *kanban_column_store_get_card      (KanbanColumnStore  *self,
                                                           gint                card_id);
#include "kanban-list-store.h"
KanbanListStore        *kanban_column_store_get_column    (KanbanColumnStore  *self,
                                                           gint                column_id);
#include "model-observer-interface.h"
ModelObserverInterface *kanban_column_store_get_observer  (KanbanColumnStore  *self);
#endif /* TESTING_ONLY_ACCESS */

G_END_DECLS

#endif /* KANBAN_COLUMN_STORE_H */

