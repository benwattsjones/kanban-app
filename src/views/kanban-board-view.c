/* src/views/kanban-board-view.c
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

#include "kanban-board-view.h"

#include "kanban-column-view.h"
#include "../presenters/kanban-board-observer-interface.h"
#include "../presenters/kanban-column-observable-interface.h"
#include <kanban-config.h>

#include <gtk/gtk.h>

struct _KanbanBoardView
{
  GtkGrid         parent_instance;
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
  // TODO free KanbanColumnView object if column deleted
  KanbanColumnView *new_column_widget = kanban_column_view_new (new_column);
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
kanban_board_view_init (KanbanBoardView *self)
{
  gtk_widget_init_template (GTK_WIDGET (self));
}

static void
kanban_board_view_class_init (KanbanBoardViewClass *klass)
{
  gtk_widget_class_set_template_from_resource (GTK_WIDGET_CLASS (klass),
                                               GRESOURCE_PREFIX "board.ui");
}

KanbanBoardView *
kanban_board_view_new ()
{
  return g_object_new (KANBAN_TYPE_BOARD_VIEW, NULL);
}

#ifdef TESTING_ONLY_ACCESS

gint
kanban_board_count_columns (KanbanBoardView *self)
{
  GList *columns_list, *l;
  int num_columns = 0;
  columns_list = gtk_container_get_children (GTK_CONTAINER (self));
  for (l = columns_list ; l != NULL; l = l->next)
    ++num_columns;
  g_list_free (columns_list);
  return num_columns;
}

#endif /* TESTING_ONLY_ACCESS */

