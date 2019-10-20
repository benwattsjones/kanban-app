/* src/views/kanban-grid.h
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

#ifndef KANBAN_BOARD_VIEW_H
#define KANBAN_BOARD_VIEW_H

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define KANBAN_TYPE_BOARD_VIEW (kanban_board_view_get_type ())

G_DECLARE_FINAL_TYPE (KanbanBoardView, kanban_board_view, KANBAN, BOARD_VIEW, GtkGrid)

KanbanBoardView *kanban_board_view_new ();

G_END_DECLS

#endif /* KANBAN_BOARD_VIEW_H */

