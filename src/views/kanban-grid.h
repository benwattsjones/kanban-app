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

#ifndef KANBAN_GRID_H
#define KANBAN_GRID_H

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define KANBAN_GRID_TYPE (kanban_grid_get_type ())

G_DECLARE_FINAL_TYPE (KanbanGrid, kanban_grid, KANBAN, GRID, GtkGrid)

KanbanGrid *kanban_grid_new ();

G_END_DECLS

#endif /* KANBAN_GRID_H */

