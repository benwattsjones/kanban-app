/* src/kanban-views/kanban-app-win.c
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

#include <gtk/gtk.h>

#include "kanban-app-win.h"

#include "../kanban-presenters/kanban-app.h"

struct _KanbanAppWindow
{
  GtkApplicationWindow parent_instance;
};

G_DEFINE_TYPE(KanbanAppWindow, kanban_app_window, GTK_TYPE_APPLICATION_WINDOW)

static void
kanban_app_window_init (KanbanAppWindow *win)
{
  gtk_widget_init_template (GTK_WIDGET (win));
}

void
kanban_app_window_open (KanbanAppWindow *win,
                        GFile           *file)
{
  (void) win;
  (void) file;
}

static void
kanban_app_window_class_init (KanbanAppWindowClass *class)
{
  gtk_widget_class_set_template_from_resource (GTK_WIDGET_CLASS (class),
                                               "/com/benwattsjones/kanban/window.ui");
}

void
initialize_kanban_view (KanbanApp *app)
{
  KanbanAppWindow *win = g_object_new (KANBAN_APP_WINDOW_TYPE, "application",
                                       app, NULL);
  gtk_window_present (GTK_WINDOW (win));
}


