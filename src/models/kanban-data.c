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

  card->card_id = 1;
  card->heading = "Card Heading";
  card->content = "Card content\nMultiline!";
  card->task = TASK_ADD_CARD;
  emit_kanban_card_change_signal (card);

  card->content = "New Card Content!";
  card->task = TASK_EDIT_CARD;
  emit_kanban_card_change_signal (card);

  card->card_id = 2;
  card->column_id = 1;
  card->heading = "Second Card";
  card->content = "content no2. This card is being given a long single line of text to check that it wraps correctly.\nHere is some more text on a new line.";
  card->priority = 1;
  card->task = TASK_ADD_CARD;
  emit_kanban_card_change_signal (card);

  card->card_id = 3;
  card->column_id = 1;
  card->heading = "Card to be Moved";
  card->content = "from col 1 priority 2 to col 2 priority 1.\n\nThis card has some more content after a gap. Long paragraph to test line wrapping.\n\nA third paragraph. Lets try a Markup-style list:\n - point one\n - bullet two\n - a final point\n\nHow about a really long word?\n\nonetwothreefourfivesixseveneightnineteneleventwelvethriteenfourteenfifteensixteenseventeeneighteennineteentwenty.\n\nDone!\n";
  card->priority = 2;
  card->task = TASK_ADD_CARD;
  emit_kanban_card_change_signal (card);

  card->card_id = 0;
  card->column_id = 2;
  card->heading = "COLUMN 2";
  card->content = NULL;
  card->priority = 1;
  card->task = TASK_ADD_COLUMN;
  emit_kanban_card_change_signal (card);

  card->card_id = 4;
  card->column_id = 2;
  card->heading = "Col2Card";
  card->content = "This is the first card in column two.";
  card->priority = 0;
  card->task = TASK_ADD_CARD;
  emit_kanban_card_change_signal (card);

  card->card_id = 5;
  card->column_id = 2;
  card->heading = "heading col2card2. Blank content.\nNew line to test cut off abcdefghijklmnopqrstuvwxyzverylongwordtestwrapping.";
  card->content = "";
  card->priority = 1;
  card->task = TASK_ADD_CARD;
  emit_kanban_card_change_signal (card);

  card->card_id = 3;
  card->column_id = 2;
  card->priority = 1;
  card->task = TASK_MOVE_CARD;
  emit_kanban_card_change_signal (card);

  card->card_id = 0;
  card->column_id = 2;
  card->heading = "COLUMN 2 renamed!";
  card->content = NULL;
  card->priority = 1;
  card->task = TASK_EDIT_COLUMN;
  emit_kanban_card_change_signal (card);


  free (card);
  card = NULL;
}

