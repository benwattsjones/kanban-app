/* src/presenters/kanban-board-observer-interface.h
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

#ifndef KANBAN_BOARD_OBSERVER_INTERFACE_H
#define KANBAN_BOARD_OBSERVER_INTERFACE_H

#include "kanban-column-observable-interface.h"

#include <glib-object.h>

G_BEGIN_DECLS

#define KANBAN_TYPE_BOARD_OBSERVER (kanban_board_observer_get_type ())

G_DECLARE_INTERFACE (KanbanBoardObserver, kanban_board_observer, KANBAN, BOARD_OBSERVER, GObject)

struct _KanbanBoardObserverInterface
{
  GTypeInterface parent_iface;

  void (*add_column)  (KanbanBoardObserver     *self,
                       KanbanColumnObservable  *new_column,
                       gint                     priority);

  void (*move_column) (KanbanBoardObserver     *self,
                       gint                     column_id,
                       gint                     priority);
};

void kanban_board_observer_add_column  (KanbanBoardObserver     *self,
                                        KanbanColumnObservable  *new_column,
                                        gint                     priority);

void kanban_board_observer_move_column (KanbanBoardObserver     *self,
                                        gint                     column_id,
                                        gint                     priority);

G_END_DECLS

#endif /* KANBAN_BOARD_OBSERVER_INTERFACE_H */

