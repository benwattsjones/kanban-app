/* tests/kanban-board-presenter-tests.cpp
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

extern "C"
{
  #include "../src/presenters/kanban-board-presenter.h"
  #include "../src/presenters/kanban-board-observer-interface.h"
  #include "../src/presenters/model-observer-interface.h"
  #include "../src/presenters/kanban-column-viewmodel.h"
  #include "../src/presenters/kanban-column-observable-interface.h"
  #include "../src/presenters/kanban-card-viewmodel.h"
  #include "../src/models/kanban-data.h"

  #include <gtk/gtk.h>
  #include <gio/gio.h>
}

#include "gtest/gtest.h"


// Stubs:
extern "C"
{
  void register_kanban_viewmodel_observer (ModelObserverInterface *observer)
  {
    (void) observer;
  }

  void deregister_kanban_viewmodel_observer (ModelObserverInterface *observer)
  {
    (void) observer;
  }

  void kanban_board_observer_add_column (KanbanBoardObserver     *self,
                                         KanbanColumnObservable  *new_column,
                                         gint                     priority)
  {
    (void) self;
    (void) new_column;
    (void) priority;
  }

  void kanban_board_observer_move_column (KanbanBoardObserver  *self,
                                          gint                  column_id,
                                          gint                  priority)
  {
    (void) self;
    (void) column_id;
    (void) priority;
  }
}

// Test Fixtures:
class KanbanBoardPresenterTests : public ::testing::Test
{
protected:
  KanbanBoardPresenter *viewmodel;
  ModelObserverInterface *observer;
  KanbanData card_data;

  void SetUp() override
  {
    card_data.card_id = 1;
    card_data.column_id = 2;
    card_data.heading = g_strdup ("heading!");
    card_data.content = g_strdup ("content.");
    card_data.priority = 0;

    viewmodel = kanban_board_presenter_new (NULL);
    observer = kanban_board_presenter_get_observer (viewmodel);
  }

  void TearDown() override
  {
    kanban_board_presenter_destroy (viewmodel);
    g_free (card_data.heading);
    g_free (card_data.content);
  }
};

// Tests:
TEST_F (KanbanBoardPresenterTests,
        New_NullPassed_NewObjectReturned)
{
  ASSERT_NE (viewmodel, nullptr);
}

TEST_F (KanbanBoardPresenterTests,
        AddColumn_ValidDataPassed_NewColumnAddedToObjectAndRetrievable)
{
  int column_id_stored;
  KanbanColumnViewModel *column_added;
  observer->task_func[TASK_ADD_COLUMN] (observer->viewmodel, &card_data);
  column_added = kanban_board_presenter_get_column (viewmodel, card_data.column_id);

  g_object_get (column_added, "column-id", &column_id_stored, NULL);
  ASSERT_NE (column_added, nullptr);
  EXPECT_EQ (column_id_stored, card_data.column_id);
}

TEST_F (KanbanBoardPresenterTests,
        AddCard_ValidDataPassed_NewCardCreatedAndRetrievable)
{
  int card_id_stored;
  char *heading_stored;
  KanbanCardViewModel *card_added;
  observer->task_func[TASK_ADD_COLUMN] (observer->viewmodel, &card_data);
  observer->task_func[TASK_ADD_CARD] (observer->viewmodel, &card_data);
  card_added = kanban_board_presenter_get_card (viewmodel, card_data.card_id);

  g_object_get (card_added, "card-id", &card_id_stored, NULL);
  g_object_get (card_added, "heading", &heading_stored, NULL);
  ASSERT_NE (card_added, nullptr);
  EXPECT_EQ (card_id_stored, card_data.card_id);
  EXPECT_STREQ (card_data.heading, heading_stored);

  g_free (heading_stored);
}

TEST_F (KanbanBoardPresenterTests,
        EditCard_NewHeadingDataPassed_CardHeadingChanged)
{
  KanbanCardViewModel *card;
  char *heading_stored;
  observer->task_func[TASK_ADD_COLUMN] (observer->viewmodel, &card_data);
  observer->task_func[TASK_ADD_CARD] (observer->viewmodel, &card_data);
  card = kanban_board_presenter_get_card (viewmodel, card_data.card_id);
  free (card_data.heading);
  card_data.heading = g_strdup ("new heading");
  observer->task_func[TASK_EDIT_CARD] (observer->viewmodel, &card_data);

  g_object_get (card, "heading", &heading_stored, NULL);
  EXPECT_STREQ (card_data.heading, heading_stored);

  g_free (heading_stored);
}

TEST_F (KanbanBoardPresenterTests,
        MoveCard_CurrantColumnAndPriorityPassed_BoardUnchanged)
{
  KanbanColumnViewModel *column;
  gpointer card1, card2, top_card_before, bottom_card_before,
           top_card_after, bottom_card_after;
  observer->task_func[TASK_ADD_COLUMN] (observer->viewmodel, &card_data);
  observer->task_func[TASK_ADD_CARD] (observer->viewmodel, &card_data);
  card1 = kanban_board_presenter_get_card (viewmodel, card_data.card_id);
  card_data.card_id = 2;
  card_data.priority = 1;
  observer->task_func[TASK_ADD_CARD] (observer->viewmodel, &card_data);
  card2 = kanban_board_presenter_get_card (viewmodel, card_data.card_id);

  column = kanban_board_presenter_get_column (viewmodel, card_data.column_id);
  top_card_before = g_list_model_get_item (G_LIST_MODEL (column), 0);
  bottom_card_before = g_list_model_get_item (G_LIST_MODEL (column), 1);
  observer->task_func[TASK_MOVE_CARD] (observer->viewmodel, &card_data);
  top_card_after = g_list_model_get_item (G_LIST_MODEL (column), 0);
  bottom_card_after = g_list_model_get_item (G_LIST_MODEL (column), 1);

  EXPECT_EQ (card1, top_card_before);
  EXPECT_EQ (card2, bottom_card_before);
  EXPECT_EQ (card1, top_card_after);
  EXPECT_EQ (card2, bottom_card_after);

  g_object_unref (top_card_before);
  g_object_unref (top_card_after);
  g_object_unref (bottom_card_before);
  g_object_unref (bottom_card_after);
}

TEST_F (KanbanBoardPresenterTests,
        MoveCard_NewPriorityPassed_CardMovesToPriorityPosition)
{
  KanbanColumnViewModel *column;
  gpointer card1, card2, top_card_before, bottom_card_before,
           top_card_after, bottom_card_after;
  observer->task_func[TASK_ADD_COLUMN] (observer->viewmodel, &card_data);
  observer->task_func[TASK_ADD_CARD] (observer->viewmodel, &card_data);
  card1 = kanban_board_presenter_get_card (viewmodel, card_data.card_id);
  card_data.card_id = 2;
  card_data.priority = 1;
  observer->task_func[TASK_ADD_CARD] (observer->viewmodel, &card_data);
  card2 = kanban_board_presenter_get_card (viewmodel, card_data.card_id);

  column = kanban_board_presenter_get_column (viewmodel, card_data.column_id);
  top_card_before = g_list_model_get_item (G_LIST_MODEL (column), 0);
  bottom_card_before = g_list_model_get_item (G_LIST_MODEL (column), 1);
  card_data.priority = 0;
  observer->task_func[TASK_MOVE_CARD] (observer->viewmodel, &card_data);
  top_card_after = g_list_model_get_item (G_LIST_MODEL (column), 0);
  bottom_card_after = g_list_model_get_item (G_LIST_MODEL (column), 1);

  EXPECT_EQ (card1, top_card_before);
  EXPECT_EQ (card2, bottom_card_before);
  EXPECT_EQ (card2, top_card_after);
  EXPECT_EQ (card1, bottom_card_after);

  g_object_unref (top_card_before);
  g_object_unref (top_card_after);
  g_object_unref (bottom_card_before);
  g_object_unref (bottom_card_after);
}

TEST_F (KanbanBoardPresenterTests,
        MoveCard_NewColumnPassed_CardMovesToNewColumn)
{
  KanbanColumnViewModel *column1, *column2;
  gint col1_count_before, col2_count_before, col1_count_after, col2_count_after;
  observer->task_func[TASK_ADD_COLUMN] (observer->viewmodel, &card_data);
  observer->task_func[TASK_ADD_CARD] (observer->viewmodel, &card_data);
  column1 = kanban_board_presenter_get_column (viewmodel, card_data.column_id);
  card_data.column_id++;
  observer->task_func[TASK_ADD_COLUMN] (observer->viewmodel, &card_data);
  column2 = kanban_board_presenter_get_column (viewmodel, card_data.column_id);

  col1_count_before = g_list_model_get_n_items (G_LIST_MODEL (column1));
  col2_count_before = g_list_model_get_n_items (G_LIST_MODEL (column2));
  observer->task_func[TASK_MOVE_CARD] (observer->viewmodel, &card_data);
  col1_count_after = g_list_model_get_n_items (G_LIST_MODEL (column1));
  col2_count_after = g_list_model_get_n_items (G_LIST_MODEL (column2));

  EXPECT_NE (column1, nullptr);
  EXPECT_NE (column2, nullptr);
  EXPECT_NE (column1, column2);
  EXPECT_EQ (col1_count_before, 1);
  EXPECT_EQ (col2_count_before, 0);
  EXPECT_EQ (col1_count_after, 0);
  EXPECT_EQ (col2_count_after, 1);
}

TEST_F (KanbanBoardPresenterTests,
        EditColumn_NewHeadingDataPassed_ColumnNameChanged)
{
  char *column_name_stored;
  KanbanColumnViewModel *column_added;
  observer->task_func[TASK_ADD_COLUMN] (observer->viewmodel, &card_data);
  column_added = kanban_board_presenter_get_column (viewmodel, card_data.column_id);
  g_free (card_data.heading);
  card_data.heading = g_strdup ("My new column heading.");

  observer->task_func[TASK_EDIT_COLUMN] (observer->viewmodel, &card_data);
  g_object_get (column_added, "column-name", &column_name_stored, NULL);

  EXPECT_STREQ (card_data.heading, column_name_stored);

  g_free (column_name_stored);
}

