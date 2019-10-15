/* src/presenters/kanban-list-viewer-interface.c
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

#include "kanban-list-viewer-interface.h"

#include <gio/gio.h>
#include <gtk/gtk.h>

G_DEFINE_INTERFACE (KanbanListViewer, kanban_list_viewer, G_TYPE_LIST_MODEL)

static void
kanban_list_viewer_default_init (KanbanListViewerInterface *iface)
{
  // Add properties and signals to interface here
}


GtkTextBuffer *
kanban_list_viewer_get_heading (KanbanListViewer *self)
{
  KanbanListViewerInterface *iface;
  g_return_val_if_fail (KANBAN_IS_LIST_VIEWER (self), NULL);
  iface = KANBAN_LIST_VIEWER_GET_IFACE (self);
  g_return_val_if_fail (iface->get_heading != NULL, NULL);
  return iface->get_heading (self);
}

