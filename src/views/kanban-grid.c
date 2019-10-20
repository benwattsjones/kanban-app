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
#include "../presenters/kanban-board-observer-interface.h"
#include "../presenters/kanban-column-observable-interface.h"
#include <kanban-config.h>

#include <gtk/gtk.h>

struct _KanbanBoardView
{
  GtkGrid         parent_instance;

  GtkCssProvider *css_provider;
};

static void kanban_board_observer_iface_init (KanbanBoardObserverInterface *iface);

G_DEFINE_TYPE_WITH_CODE (KanbanBoardView, kanban_board_view, GTK_TYPE_GRID,
                         G_IMPLEMENT_INTERFACE (KANBAN_TYPE_BOARD_OBSERVER,
                                                kanban_board_observer_iface_init))

// KanbanBoardObserver iface implementation:

static void
kanban_board_view_add_column (KanbanBoardObserver     *self,
                              KanbanColumnObservable  *new_column,
                              gint                     priority)
{
  // TODO free KanbanListBox object if column deleted
  KanbanListBox *new_column_widget = kanban_list_box_new (new_column);
  gtk_grid_insert_column (GTK_GRID (self), priority);
  gtk_grid_attach (GTK_GRID (self), GTK_WIDGET (new_column_widget), priority, 0, 1, 1);
}

static void
kanban_board_observer_iface_init (KanbanBoardObserverInterface *iface)
{
  iface->add_column = kanban_board_view_add_column;
}

// KanbanBoardView GObject implementation:

static void
kanban_board_view_finalize (GObject *object)
{
  KanbanBoardView *self = KANBAN_BOARD_VIEW (object);

  g_object_unref (self->css_provider);
}

static void
kanban_board_view_init (KanbanBoardView *self)
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
kanban_board_view_class_init (KanbanBoardViewClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->finalize = kanban_board_view_finalize;

  gtk_widget_class_set_template_from_resource (GTK_WIDGET_CLASS (klass),
                                               GRESOURCE_PREFIX "board.ui");
}

KanbanBoardView *
kanban_board_view_new ()
{
  return g_object_new (KANBAN_TYPE_BOARD_VIEW, NULL);
}

