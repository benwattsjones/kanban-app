/* src/presenters/column-view-interface.c
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

#include "kanban-column-viewer-interface.h"

#include <gio/gio.h>
#include <glib-object.h>

G_DEFINE_INTERFACE (KanbanColumnViewer, kanban_column_viewer, G_TYPE_OBJECT)

static void
kanban_column_viewer_default_init (KanbanColumnViewerInterface *iface)
{
  // Add properties and signals to interface here
}

void
kanban_column_viewer_add_column (KanbanColumnViewer *self,
                                 GListModel         *new_column)
{
  KanbanColumnViewerInterface *iface;
  g_return_if_fail (KANBAN_IS_COLUMN_VIEWER (self));
  iface = KANBAN_COLUMN_VIEWER_GET_IFACE (self);
  g_return_if_fail (iface->add_column != NULL);
  iface->add_column (self, new_column);
}
