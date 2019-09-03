/* src/models/kanban-data.c
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

#include "kanban-data.h"

#include "model-presenter-interface.h"

#include <stdlib.h>
#include <unistd.h> /* only for usleep test stub - TODO: remove */

void
test_observers()
{
  KanbanData *card = (KanbanData *) malloc (sizeof (KanbanData));

  card->card_id = 0;
  card->column_id = 1;
  card->heading = "Column Heading";
  card->content = NULL;
  card->priority = 0;
  card->task = TASK_ADD_COLUMN;
  emit_kanban_card_change_signal (card);

  usleep (500000); 
  card->card_id = 1;
  card->heading = "Card Heading";
  card->content = "Card content\nMultiline!";
  card->task = TASK_ADD_CARD;
  emit_kanban_card_change_signal (card);

  usleep (200000);
  card->content = "New Card Content!";
  card->column_id = 0;
  card->task = TASK_EDIT_CARD;
  emit_kanban_card_change_signal (card);
  
  free (card);
  card = NULL;
}

