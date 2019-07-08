/* src/kanban-presenters/presenter-view-interface.h
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

#ifndef PRESENTER_VIEW_INTERFACE_H
#define PRESENTER_VIEW_INTERFACE_H

#include <gtk/gtk.h>

#include "kanban-application.h"

G_BEGIN_DECLS

/* Function must be implemented by the view as the entry point. */
void initialize_kanban_view (KanbanApplication *app);

G_END_DECLS

#endif /* PRESENTER_VIEW_INTERFACE_H */

