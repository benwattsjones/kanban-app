/* src/presenters/kanban-list-viewer-interface.h
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

#ifndef KANBAN_LIST_VIEWER_INTERFACE_H
#define KANBAN_LIST_VIEWER_INTERFACE_H

#include <glib-object.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define KANBAN_COLUMN_VIEWER_TYPE (kanban_column_viewer_get_type ())

G_DECLARE_INTERFACE (KanbanListViewer, kanban_list_viewer, KANBAN, LIST_VIEWER, GObject)

struct _KanbanListViewerInterface
{
  GTypeInterface parent_iface;

  GtkTextBuffer *   (*get_heading)    (KanbanListViewer *self);
};

GtkTextBuffer  *kanban_list_viewer_get_heading  (KanbanListViewer *self);

G_END_DECLS

#endif /* KANBAN_LIST_VIEWER_INTERFACE_H */

