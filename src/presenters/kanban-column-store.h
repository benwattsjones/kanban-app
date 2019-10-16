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

#ifndef KANBAN_BOARD_PRESENTER_H
#define KANBAN_BOARD_PRESENTER_H

#include "kanban-column-viewer-interface.h"

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define KANBAN_TYPE_BOARD_PRESENTER (kanban_board_presenter_get_type ())

G_DECLARE_FINAL_TYPE (KanbanBoardPresenter, kanban_board_presenter, KANBAN, BOARD_PRESENTER, GObject)

KanbanBoardPresenter   *kanban_board_presenter_new           (KanbanColumnViewer *view_observer);
void                    kanban_board_presenter_destroy       (KanbanBoardPresenter  *self);


#ifdef TESTING_ONLY_ACCESS
#include "kanban-card-viewmodel.h"
KanbanCardViewModel    *kanban_board_presenter_get_card      (KanbanBoardPresenter  *self,
                                                           gint                card_id);
#include "kanban-list-store.h"
KanbanListStore        *kanban_board_presenter_get_column    (KanbanBoardPresenter  *self,
                                                           gint                column_id);
#include "model-observer-interface.h"
ModelObserverInterface *kanban_board_presenter_get_observer  (KanbanBoardPresenter  *self);
#endif /* TESTING_ONLY_ACCESS */

G_END_DECLS

#endif /* KANBAN_BOARD_PRESENTER_H */

