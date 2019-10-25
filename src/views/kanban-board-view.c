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

  GHashTable     *id_column_table;
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

  int column_id = kanban_column_observable_get_id (new_column);
  g_hash_table_insert (KANBAN_BOARD_VIEW (self)->id_column_table,
                       GINT_TO_POINTER (column_id), new_column_widget);
}

static void
kanban_board_view_move_column (KanbanBoardObserver *self,
                               gint                 column_id,
                               gint                 priority)
{
  g_print ("hello\n");
}

static void
kanban_board_observer_iface_init (KanbanBoardObserverInterface *iface)
{
  iface->add_column = kanban_board_view_add_column;
  iface->move_column = kanban_board_view_move_column;
}

// KanbanBoardView GObject implementation:

static void
kanban_board_view_finalize (GObject *object)
{
  KanbanBoardView *self = KANBAN_BOARD_VIEW (object);

  g_clear_pointer (&self->id_column_table, g_hash_table_destroy);

  G_OBJECT_CLASS (kanban_board_view_parent_class)->finalize (object);
}

static void
kanban_board_view_init (KanbanBoardView *self)
{
  self->id_column_table = g_hash_table_new (g_direct_hash, g_direct_equal);

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

#ifdef TESTING_ONLY_ACCESS

gint
kanban_board_view_count_columns (KanbanBoardView *self)
{
  GList *columns_list, *l;
  int num_columns = 0;
  columns_list = gtk_container_get_children (GTK_CONTAINER (self));
  for (l = columns_list ; l != NULL; l = l->next)
    ++num_columns;
  g_list_free (columns_list);
  return num_columns;
}

KanbanColumnView *
kanban_board_view_get_nth_column (KanbanBoardView *self,
                                  gint             priority)
{
  return KANBAN_COLUMN_VIEW (gtk_grid_get_child_at (GTK_GRID (self), priority, 0));
}

#endif /* TESTING_ONLY_ACCESS */

