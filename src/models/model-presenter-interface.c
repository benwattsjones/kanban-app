/* src/models/model-presenter-interface.c
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

#include <stddef.h>
#include <unistd.h>
#include <stdlib.h>

#include "model-presenter-interface.h"

static KanbanModelObserver *active_observer;

void 
attach_observer(const KanbanModelObserver *observer)
{
  // TODO: linked list implementation will not have memory leak!
  active_observer = malloc (sizeof (KanbanModelObserver)); /* malloc else NULL ptr */
  active_observer->instance = observer->instance; /* deep copy else segfault */
  active_observer->notification = observer->notification;
}

void 
detach_observer(const KanbanModelObserver *observer)
{
  (void)observer; /* TODO: linked list implementation (will need parameter) */
  active_observer->instance = NULL;
  active_observer->notification = NULL;
}

static void 
change_kanban(int new_data)
{
  KanbanModelData new_kanban_data = { .data = new_data };
  if (active_observer->instance != NULL)
    {
      active_observer->notification(active_observer->instance, &new_kanban_data);
    }
}

void
test_observers()
{
  /* note: usleep is depreciated - only accepted as testing stub */
  usleep (1000000); 
  change_kanban (4);
  usleep (2000000);
  change_kanban (20);
}

