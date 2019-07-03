/* src/kanban-app.h
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

#ifndef KANBAN_APP_H
#define KANBAN_APP_H

#include <gtk/gtk.h>


#define KANBAN_APP_TYPE (kanban_app_get_type ())

G_DECLARE_FINAL_TYPE (KanbanApp, kanban_app, KANBAN, APP, GtkApplication)


KanbanApp     *kanban_app_new         (void);


#endif /* KANBAN_APP_H */
