/* src/presenters/column-view-interface.h
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

#ifndef KANBAN_COLUMN_VIEWER_INTERFACE_H
#define KANBAN_COLUMN_VIEWER_INTERFACE_H

#include "kanban-list-viewer-interface.h"

#include <glib-object.h>

G_BEGIN_DECLS

#define KANBAN_COLUMN_VIEWER_TYPE (kanban_column_viewer_get_type ())

G_DECLARE_INTERFACE (KanbanColumnViewer, kanban_column_viewer, KANBAN, COLUMN_VIEWER, GObject)

struct _KanbanColumnViewerInterface
{
  GTypeInterface parent_iface;

  void (*add_column) (KanbanColumnViewer *self,
                      KanbanListViewer   *new_column,
                      gint                priority);
};

void kanban_column_viewer_add_column (KanbanColumnViewer *self,
                                      KanbanListViewer   *new_column,
                                      gint                priority);

G_END_DECLS

#endif /* KANBAN_COLUMN_VIEWER_INTERFACE_H */

