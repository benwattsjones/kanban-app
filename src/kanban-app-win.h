/* src/kanban-app-win.h
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

#ifndef KANBAN_APP_WIN_H
#define KANBAN_APP_WIN_H

#include <gtk/gtk.h>
#include "kanban-app.h"


#define KANBAN_APP_WINDOW_TYPE (kanban_app_window_get_type ())
G_DECLARE_FINAL_TYPE (KanbanAppWindow, kanban_app_window, KANBAN, APP_WINDOW, GtkApplicationWindow)


KanbanAppWindow        *kanban_app_window_new          (KanbanApp *app);
void                    kanban_app_window_open         (KanbanAppWindow *win,
                                                        GFile           *file);


#endif /* KANBAN_APP_WIN_H */
