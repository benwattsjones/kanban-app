/* src/views/kanban-window.c
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

#include "kanban-window.h"

#include "../presenters/presenter-view-interface.h"

struct _KanbanWindow
{
  GtkApplicationWindow parent_instance;
};

G_DEFINE_TYPE(KanbanWindow, kanban_window, GTK_TYPE_APPLICATION_WINDOW)

static void
kanban_window_init (KanbanWindow *self)
{
  gtk_widget_init_template (GTK_WIDGET (self));
}

static void
kanban_window_class_init (KanbanWindowClass *klass)
{
  gtk_widget_class_set_template_from_resource (GTK_WIDGET_CLASS (klass),
                                               "/com/benwattsjones/kanban/window.ui");
}

void
initialize_kanban_view (KanbanApplication *app)
{
  KanbanWindow *win = g_object_new (KANBAN_WINDOW_TYPE, 
                                    "application", app, 
                                    NULL);
  gtk_window_present (GTK_WINDOW (win));
}

