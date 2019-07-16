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

#include <gtk/gtk.h> /* for g_print only - may remove in future */
#include <assert.h>
#include <stdlib.h>

#include "model-observer.h"

#include "../models/model-presenter-interface.h"
#include "../models/kanban-cards.h" /* for test_observers() only */

struct KanbanViewModel
{
  char *heading;
  char *content;
};

static void 
kanban_model_changed(void             *instance, 
                     const KanbanCard *card_data)
{
  KanbanViewModelPtr viewmodel = instance;
  viewmodel->heading = card_data->heading;
  viewmodel->content = card_data->content;
  assert (viewmodel != NULL);
  g_print ("Data observed:\nHeading: %s\nContent: %s\n", 
           viewmodel->heading, viewmodel->content);
}

KanbanViewModelPtr 
create_kanban_viewmodel()
{
  KanbanViewModelPtr viewmodel = malloc (sizeof (*viewmodel));
  assert (viewmodel != NULL);
  KanbanModelObserver observer = { .instance = viewmodel,
                                   .notification = kanban_model_changed };
  attach_observer (&observer);
  test_observers ();
  return viewmodel;
}

void
destroy_kanban_viewmodel(KanbanViewModelPtr viewmodel)
{
  KanbanModelObserver observer = { .instance = viewmodel,
                                   .notification = kanban_model_changed };
  detach_observer (&observer);
  free (viewmodel);
  viewmodel = NULL;
}

