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

#include "kanban-window.h"

#include <kanban-config.h>

#include <gtk/gtk.h>

struct _KanbanWindow
{
  GtkApplicationWindow parent_instance;

  GtkCssProvider *css_provider;
};

G_DEFINE_TYPE (KanbanWindow, kanban_window, GTK_TYPE_APPLICATION_WINDOW)

static void
kanban_window_finalize (GObject *object)
{
  KanbanWindow *self = KANBAN_WINDOW (object);

  g_object_unref (self->css_provider);

  G_OBJECT_CLASS (kanban_window_parent_class)->finalize (object);
}

static void
kanban_window_init (KanbanWindow *self)
{
  self->css_provider = gtk_css_provider_new ();
  gtk_css_provider_load_from_resource (self->css_provider,
                                       GRESOURCE_PREFIX "board.css");
  gtk_style_context_add_provider_for_screen (gdk_screen_get_default (),
                                             GTK_STYLE_PROVIDER (self->css_provider),
                                             GTK_STYLE_PROVIDER_PRIORITY_USER);

  gtk_widget_init_template (GTK_WIDGET (self));
}

static void
kanban_window_class_init (KanbanWindowClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->finalize = kanban_window_finalize;

  gtk_widget_class_set_template_from_resource (GTK_WIDGET_CLASS (klass),
                                               GRESOURCE_PREFIX "window.ui");
}

KanbanWindow *
kanban_window_new (GApplication *app)
{
  return g_object_new (KANBAN_TYPE_WINDOW,
                       "application", app,
                       NULL);
}

void
kanban_window_display_board (KanbanWindow *self,
                             GtkWidget    *board)
{
  gtk_container_add (GTK_CONTAINER (self), board);
  gtk_widget_show_all (GTK_WIDGET (self));
}

