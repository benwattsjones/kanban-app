/* src/kanban-utils.h
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

#ifndef KANBAN_UTILS_H
#define KANBAN_UTILS_H

#include <gtk/gtk.h>

G_BEGIN_DECLS

GtkWidget  *kanban_utils_find_widget_by_name  (GtkWidget   *parent,
                                               const gchar *name);

G_END_DECLS

#endif /* KANBAN_UTILS_H */

