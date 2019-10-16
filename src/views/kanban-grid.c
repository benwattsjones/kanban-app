/* src/views/kanban-grid.c
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

#include "kanban-grid.h"

#include "kanban-list-box.h"
#include "../presenters/kanban-column-viewer-interface.h"
#include "../presenters/kanban-list-viewer-interface.h"
#include <kanban-config.h>

#include <gtk/gtk.h>

struct _KanbanGrid
{
  GtkGrid         parent_instance;

  GtkCssProvider *css_provider;
};

static void kanban_board_observer_iface_init (KanbanBoardObserverInterface *iface);

G_DEFINE_TYPE_WITH_CODE (KanbanGrid, kanban_grid, GTK_TYPE_GRID,
                         G_IMPLEMENT_INTERFACE (KANBAN_TYPE_BOARD_OBSERVER,
                                                kanban_board_observer_iface_init))

// KanbanBoardObserver iface implementation:

static void
kanban_grid_add_column (KanbanBoardObserver *self,
                        KanbanListViewer    *new_column,
                        gint                 priority)
{
  // TODO free KanbanListBox object if column deleted
  KanbanListBox *new_column_widget = kanban_list_box_new (new_column);
  gtk_grid_insert_column (GTK_GRID (self), priority);
  gtk_grid_attach (GTK_GRID (self), GTK_WIDGET (new_column_widget), priority, 0, 1, 1);
}

static void
kanban_board_observer_iface_init (KanbanBoardObserverInterface *iface)
{
  iface->add_column = kanban_grid_add_column;
}

// KanbanGrid GObject implementation:

static void
kanban_grid_finalize (GObject *object)
{
  KanbanGrid *self = KANBAN_GRID (object);

  g_object_unref (self->css_provider);
}

static void
kanban_grid_init (KanbanGrid *self)
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
kanban_grid_class_init (KanbanGridClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->finalize = kanban_grid_finalize;

  gtk_widget_class_set_template_from_resource (GTK_WIDGET_CLASS (klass),
                                               GRESOURCE_PREFIX "board.ui");
}

KanbanGrid *
kanban_grid_new ()
{
  return g_object_new (KANBAN_GRID_TYPE, NULL);
}

