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

TEST_F (PresentersViewsIntergrationTests, checkChangeHeadingUpdatesWidget)
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

