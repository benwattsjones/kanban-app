/* src/kanban-views/kanban-window.h
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

#ifndef KANBAN_WINDOW_H
#define KANBAN_WINDOW_H

#include <gtk/gtk.h>

#include "../kanban-presenters/kanban-application.h"

G_BEGIN_DECLS

#define KANBAN_WINDOW_TYPE (kanban_window_get_type ())

G_DECLARE_FINAL_TYPE (KanbanWindow, kanban_window, KANBAN, WINDOW, GtkApplicationWindow)

void initialize_kanban_view (KanbanApplication *app);
void kanban_app_window_open (KanbanWindow *win,
                             GFile *file);
G_END_DECLS

#endif /* KANBAN_WINDOW_H */

