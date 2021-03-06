/* tests/application-entry-tests.cpp
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
  #include "../src/kanban-application.h"
  #include "../src/models/model-presenter-interface.h"
  #include "../src/presenters/kanban-board-presenter.h"
  #include "../src/presenters/model-observer-interface.h"
  #include "../src/presenters/kanban-board-observer-interface.h"
  #include "../src/views/kanban-board-view.h"
  #include "../src/views/kanban-column-view.h"

  #include <gtk/gtk.h>
}

#include "gtest/gtest.h"

/* WARNING:
 * The model is currently expected to conform to the following API rules,
 * and as such checks for them breaking is not performed currently by
 * presenter code.

 *  - A column must be created (TASK_ADD_COLUMN) with corresponding column-id,
 *    before a card may be added to that column (TASK_ADD_CARD).
 *  - Within a column, a cards priority cannot be greater than the number
 *    of cards in the column. Thus the first card added must have priority
 *    value zero; the second card added may have priority zero (making it
 *    the higher priority card) or priority one (making it the lower priority
 *    card).
 *  - Card and column ID values must exist for each column and card, and are
 *    unique and immutable.
 *  - Operations to move or edit cards and columns (TASK_EDIT_*, TASK_MOVE_*),
 *    may only be performed on cards / columns that have already been created
 *    (with TASK_ADD_*).
 *
 * Currently, breaking these API rules invokes undefined behaviour.
 * However this project is currently under active development, and APIs
 * between model, presenter and view code will be formalized in the future,
 * along with documentation.
 */


KanbanModelObserver observer_model;

// Stubs:
extern "C"
{
  void attach_observer (const KanbanModelObserver *observer)
  {
    observer_model.instance = observer->instance;
    observer_model.notification = observer->notification;
  }

  void detach_observer (const KanbanModelObserver *observer)
  {
    (void) observer;
  }
}


// Test Fixtures:
class PresentersViewsIntergrationTests : public ::testing::Test
{
protected:
  KanbanBoardView *view;
  KanbanBoardPresenter *presenter;
  KanbanData model_data;

  void SetUp() override
  {
    gtk_init (0, NULL);

    view = kanban_board_view_new ();
    presenter = kanban_board_presenter_new (KANBAN_BOARD_OBSERVER (view));

    model_data.card_id = 1;
    model_data.column_id = 1;
    model_data.heading = NULL;
    model_data.content = NULL;
    model_data.priority = 0;
  }

  void TearDown() override
  {
    kanban_board_presenter_destroy (presenter);
  }
};


// Tests:
TEST_F (PresentersViewsIntergrationTests, checkObjectsNotNull)
{
  EXPECT_NE (view, nullptr);
  EXPECT_NE (presenter, nullptr);
  EXPECT_NE (observer_model.notification, nullptr);
  EXPECT_NE (observer_model.instance, nullptr);
}

TEST_F (PresentersViewsIntergrationTests, checkAddColumnCreatesWidget)
{
  int num_columns_before = kanban_board_view_count_columns (view);
  model_data.task = TASK_ADD_COLUMN;
  observer_model.notification (observer_model.instance, &model_data);
  int num_columns_after = kanban_board_view_count_columns (view);

  ASSERT_NE (num_columns_after, 0);
  EXPECT_EQ (num_columns_after, num_columns_before + 1);
}

TEST_F (PresentersViewsIntergrationTests, checkAddCardCreatesWidget)
{
  KanbanColumnView *column;
  model_data.task = TASK_ADD_COLUMN;
  observer_model.notification (observer_model.instance, &model_data);

  column = kanban_board_view_get_nth_column (view, model_data.priority);
  int num_cards_before = kanban_column_view_count_cards (column);
  model_data.task = TASK_ADD_CARD;
  observer_model.notification (observer_model.instance, &model_data);
  int num_cards_after = kanban_column_view_count_cards (column);

  ASSERT_NE (num_cards_after, 0);
  EXPECT_EQ (num_cards_after, num_cards_before + 1);
}

TEST_F (PresentersViewsIntergrationTests, checkEditCardHeadingUpdatesWidget)
{
  KanbanColumnView *column;
  gchar *heading;
  gchar *new_heading = g_strdup ("New Card Heading");
  model_data.task = TASK_ADD_COLUMN;
  observer_model.notification (observer_model.instance, &model_data);
  model_data.task = TASK_ADD_CARD;
  observer_model.notification (observer_model.instance, &model_data);

  model_data.heading = new_heading;
  model_data.task = TASK_EDIT_CARD;
  observer_model.notification (observer_model.instance, &model_data);
  column = kanban_board_view_get_nth_column (view, model_data.priority);
  heading = kanban_column_view_get_card_heading (column, model_data.priority);

  ASSERT_NE (heading, nullptr);
  EXPECT_STREQ (heading, new_heading);

  g_free (heading);
  g_free (new_heading);
}

TEST_F (PresentersViewsIntergrationTests, checkMoveCardMovesWidget)
{
  KanbanColumnView *column1, *column2;
  model_data.task = TASK_ADD_COLUMN;
  observer_model.notification (observer_model.instance, &model_data);
  model_data.task = TASK_ADD_CARD;
  observer_model.notification (observer_model.instance, &model_data);
  model_data.task = TASK_ADD_COLUMN;
  model_data.column_id++;
  observer_model.notification (observer_model.instance, &model_data);
  column1 = kanban_board_view_get_nth_column (view, model_data.priority);
  column2 = kanban_board_view_get_nth_column (view, model_data.priority+1);


  int col1_len_before = kanban_column_view_count_cards (column1);
  int col2_len_before = kanban_column_view_count_cards (column2);
  model_data.task = TASK_MOVE_CARD;
  observer_model.notification (observer_model.instance, &model_data);
  int col1_len_after = kanban_column_view_count_cards (column1);
  int col2_len_after = kanban_column_view_count_cards (column2);

  ASSERT_NE (column1, nullptr);
  ASSERT_NE (column2, nullptr);
  EXPECT_NE (col2_len_before, 0);
  EXPECT_EQ (col2_len_after, col2_len_before - 1);
  EXPECT_NE (col1_len_after, 0);
  EXPECT_EQ (col1_len_after, col1_len_before + 1);
}

TEST_F (PresentersViewsIntergrationTests, checkEditColumnChangesColumnWidgetText)
{
  KanbanColumnView *column;
  char *column_name_stored;
  model_data.task = TASK_ADD_COLUMN;
  observer_model.notification (observer_model.instance, &model_data);
  column = kanban_board_view_get_nth_column (view, model_data.priority);

  model_data.heading = g_strdup ("new column name");
  model_data.task = TASK_EDIT_COLUMN;
  observer_model.notification (observer_model.instance, &model_data);
  column_name_stored = kanban_column_view_get_heading (column);

  ASSERT_NE (column_name_stored, nullptr);
  EXPECT_STREQ (column_name_stored, model_data.heading);

  g_free (column_name_stored);
  g_free (model_data.heading);
}

TEST_F (PresentersViewsIntergrationTests, checkMoveColumnMovesColumnWidget)
{
  KanbanColumnView *column1_before, *column2_before, *column1_after, *column2_after;
  model_data.task = TASK_ADD_COLUMN;
  observer_model.notification (observer_model.instance, &model_data);
  column1_before = kanban_board_view_get_nth_column (view, model_data.priority);
  model_data.column_id++;
  model_data.priority++;
  observer_model.notification (observer_model.instance, &model_data);
  column2_before = kanban_board_view_get_nth_column (view, model_data.priority);

  model_data.priority--;
  model_data.task = TASK_MOVE_COLUMN;
  observer_model.notification (observer_model.instance, &model_data);
  column1_after = kanban_board_view_get_nth_column (view, model_data.priority);
  column2_after = kanban_board_view_get_nth_column (view, model_data.priority + 1);

  ASSERT_NE (column1_after, nullptr);
  ASSERT_NE (column2_after, nullptr);
  EXPECT_EQ (column1_after, column2_before);
  EXPECT_EQ (column2_after, column1_before);
}

