/* src/presenters/kanban-board-observer-interface.c
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

#include "kanban-board-observer-interface.h"

#include "kanban-list-viewer-interface.h"

#include <glib-object.h>

G_DEFINE_INTERFACE (KanbanBoardObserver, kanban_board_observer, G_TYPE_OBJECT)

static void
kanban_board_observer_default_init (KanbanBoardObserverInterface *iface)
{
  // Add properties and signals to interface here
  (void) iface;
}

void
kanban_board_observer_add_column (KanbanBoardObserver     *self,
                                  KanbanColumnObservable  *new_column,
                                  gint                     priority)
{
  if (!self)
    return;
  KanbanBoardObserverInterface *iface;
  g_return_if_fail (KANBAN_IS_BOARD_OBSERVER (self));
  iface = KANBAN_BOARD_OBSERVER_GET_IFACE (self);
  g_return_if_fail (iface->add_column != NULL);
  iface->add_column (self, new_column, priority);
}
