/* src/views/kanban-list-box.c
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

#include "kanban-list-box.h"

#include <gtk/gtk.h>

struct _KanbanListBox
{
  GtkListBox parent_instance;
};

G_DEFINE_TYPE (KanbanListBox, kanban_list_box, GTK_TYPE_LIST_BOX)

static void
kanban_list_box_init (KanbanListBox *self)
{

}

static void
kanban_list_box_class_init (KanbanListBoxClass *klass)
{

}

KanbanListBox *
kanban_list_box_new ()
{
  return g_object_new (KANBAN_LIST_BOX_TYPE, NULL);
}

