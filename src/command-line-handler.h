/* src/command-line-handler.h
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

#ifndef COMMAND_LINE_HANDLER_H
#define COMMAND_LINE_HANDLER_H

#include <gtk/gtk.h>

G_BEGIN_DECLS

void set_recognised_cmd_options (int argc, char *argv[]);

G_END_DECLS

#endif /* COMMAND_LINE_HANDLER_H */

