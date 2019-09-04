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

#include <gtk/gtk.h>

struct _KanbanGrid
{
  GtkGrid       parent_instance;
};

static void kanban_column_viewer_iface_init (KanbanColumnViewerInterface *iface);

G_DEFINE_TYPE_WITH_CODE (KanbanGrid, kanban_grid, GTK_TYPE_GRID,
                         G_IMPLEMENT_INTERFACE (KANBAN_COLUMN_VIEWER_TYPE,
                                                kanban_column_viewer_iface_init))

// KanbanColumnViewer iface implementation:

static void
kanban_grid_add_column (KanbanColumnViewer *self,
                        GListModel         *new_column)
{
  KanbanListBox *new_column_widget = kanban_list_box_new (new_column);
  gtk_grid_insert_column (GTK_GRID (self), 0); // always prepends new columns TODO
  gtk_grid_attach (GTK_GRID (self), GTK_WIDGET (new_column), 0, 0, 1, 1);
}

static void
kanban_column_viewer_iface_init (KanbanColumnViewerInterface *iface)
{
  iface->add_column = kanban_grid_add_column;
}

// KanbanGrid GObject implementation:

static void
kanban_grid_init (KanbanGrid *self)
{

}

static void
kanban_grid_class_init (KanbanGridClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

}

KanbanGrid *
kanban_grid_new (KanbanListStore *card_data)
{
  return g_object_new (KANBAN_GRID_TYPE, NULL);
}

