/* src/models/kanban-cards.c
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

#include <stdlib.h>
#include <unistd.h> /* only for usleep test stub - TODO: remove */

#include "model-presenter-interface.h"

#include "kanban-cards.h"

void
test_observers()
{
  KanbanCard *card = (KanbanCard *) malloc (sizeof (KanbanCard));

  card->card_id = 1;
  card->column_id = 1;
  card->heading = "Card Heading";
  card->content = "Card content\nMultiline!";
  card->priority = 1;

  usleep (1000000); 
  emit_kanban_card_change_signal (card);
  usleep (2000000);
  card->content = "New Card Content!";
  emit_kanban_card_change_signal (card);
  
  free(card);
  card = NULL;
}

