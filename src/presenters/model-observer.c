/* src/presenters/model-observer.c
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

#include "model-observer.h"

#include "model-observer-interface.h"
#include "../models/model-presenter-interface.h"
#include "../models/kanban-data.h"

#include <assert.h>
#include <stddef.h>


static void 
kanban_model_changed (void             *instance, 
                      const KanbanData *card_data)
{
  ModelObserverInterface *observer = instance;
  assert (observer != NULL);
  
  observer->task_func[card_data->task] (observer->viewmodel, card_data);
}

void 
register_kanban_viewmodel_observer (ModelObserverInterface *observer)
{
  int i;
  for (i = 0; i < NUM_TASKS; ++i)
    assert (observer->task_func[i] != NULL);
  KanbanModelObserver observer_wrap = { .instance = observer,
                                        .notification = kanban_model_changed };
  attach_observer (&observer_wrap);
}

void
deregister_kanban_viewmodel_observer (ModelObserverInterface *observer)
{
  KanbanModelObserver observer_wrap = { .instance = observer,
                                        .notification = kanban_model_changed };
  detach_observer (&observer_wrap);
}

