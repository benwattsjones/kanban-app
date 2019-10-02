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
  #include "../src/presenters/kanban-column-store.h"
  #include "../src/presenters/model-observer-interface.h"
  #include "../src/models/model-presenter-interface.h"

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
class PresentersCardsIntergrationTests : public ::testing::Test
{
protected:
  KanbanColumnStore *presenter;
  KanbanData model_data;

  void SetUp() override
  {
    presenter = kanban_column_store_new (NULL);

    model_data.card_id = 1;
    model_data.column_id = 1;
    model_data.heading = NULL;
    model_data.content = NULL;
    model_data.priority = 0;
  }

  void TearDown() override
  {
    kanban_column_store_destroy (presenter);
  }
};


// Tests:
TEST_F (PresentersCardsIntergrationTests, checkPresenterNotNull)
{
  EXPECT_NE (presenter, nullptr);
  EXPECT_NE (observer_model.notification, nullptr);
  EXPECT_NE (observer_model.instance, nullptr);
}

TEST_F (PresentersCardsIntergrationTests, checkCreateColumnSuccessful)
{
  model_data.task = TASK_ADD_COLUMN;
  observer_model.notification (observer_model.instance, &model_data);
  gpointer new_column = kanban_column_store_get_column (presenter,
                                                        model_data.column_id);
  EXPECT_NE (new_column, nullptr);
}

