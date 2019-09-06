/* src/views/kanban-list-box.h
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

#ifndef KANBAN_LIST_BOX_H
#define KANBAN_LIST_BOX_H

#include <gtk/gtk.h>
#include <gio/gio.h>

G_BEGIN_DECLS

#define KANBAN_LIST_BOX_TYPE (kanban_list_box_get_type ())

G_DECLARE_FINAL_TYPE (KanbanListBox, kanban_list_box, KANBAN, LIST_BOX, GtkListBox)

KanbanListBox *kanban_list_box_new (GListModel *column_data);

G_END_DECLS

#endif /* KANBAN_LIST_BOX_H */

