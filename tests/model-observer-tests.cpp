/* tests/model-observer-tests.cpp
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
  #include "../src/presenters/model-observer.h"
  #include "../src/presenters/model-observer-interface.h"
  #include "../src/models/kanban-data.h"
  #include "../src/models/model-presenter-interface.h"

  #include <gtk/gtk.h>
}

#include "gtest/gtest.h"

KanbanModelObserver observer_data;

static struct FuncCallCounter
{
    int add_card = 0;
    int edit_card = 0;
    int move_card = 0;
    int add_column = 0;
    int edit_column = 0;
    int move_column = 0;
} func_call_count, func_call_count_reset;

// Stubs:
extern "C"
{
  void attach_observer (const KanbanModelObserver *observer)
  {
    observer_data.instance = observer->instance;
    observer_data.notification = observer->notification;
  }
  void detach_observer (const KanbanModelObserver *observer)
  {
    (void) observer;
  }

  static void mock_add_card (void *vself, const KanbanData *card_data)
  {
    (void) vself;
    (void) card_data;
    ++func_call_count.add_card;
  }
  static void mock_edit_card (void *vself, const KanbanData *card_data)
  {
    (void) vself;
    (void) card_data;
    ++func_call_count.edit_card;
  }
  static void mock_move_card (void *vself, const KanbanData *card_data)
  {
    (void) vself;
    (void) card_data;
    ++func_call_count.move_card;
  }
  static void mock_add_column (void *vself, const KanbanData *column_data)
  {
    (void) vself;
    (void) column_data;
    ++func_call_count.add_column;
  }
  static void mock_edit_column (void *vself, const KanbanData *column_data)
  {
    (void) vself;
    (void) column_data;
    ++func_call_count.edit_column;
  }
  static void mock_move_column (void *vself, const KanbanData *column_data)
  {
    (void) vself;
    (void) column_data;
    ++func_call_count.move_column;
  }
}

// Test Fixtures:
class ModelObserverTests : public ::testing::Test
{
protected:
  KanbanData card_data;
  ModelObserverInterface observer;

  void SetUp() override
  {
    func_call_count = func_call_count_reset;

    card_data.card_id = 1;
    card_data.column_id = 2;
    card_data.heading = NULL;
    card_data.content = NULL;
    card_data.priority = 3;

    observer.viewmodel = NULL;
    observer.task_func[TASK_ADD_CARD] = mock_add_card;
    observer.task_func[TASK_EDIT_CARD] = mock_edit_card;
    observer.task_func[TASK_MOVE_CARD] = mock_move_card;
    observer.task_func[TASK_ADD_COLUMN] = mock_add_column;
    observer.task_func[TASK_EDIT_COLUMN] = mock_edit_column;
    observer.task_func[TASK_MOVE_COLUMN] = mock_move_column;

    register_kanban_viewmodel_observer (&observer);
  }

  void TearDown() override
  {
  }
};

// Tests:
TEST_F (ModelObserverTests,
        RegisterObserver_ValidInterfacePassed_ObserverObjectNotNull)
{
  EXPECT_NE (observer_data.notification, nullptr);
  EXPECT_NE (observer_data.instance, nullptr);
//  card_data->task = TASK_ADD_CARD;
//  observer_data.notification (observer_data.instance, card_data);
//  EXPECT_EQ (func_call_count.add_card, 1);
}

TEST_F (ModelObserverTests,
        DataChanged_EnumTaskIsAddCard_AddCardFuncPointerCalled)
{
  card_data.task = TASK_ADD_CARD;
  observer_data.notification (observer_data.instance, &card_data);
  EXPECT_EQ (func_call_count.add_card, 1);
}

TEST_F (ModelObserverTests,
        DataChanged_EnumTaskIsEditCard_EditCardFuncPointerCalled)
{
  card_data.task = TASK_EDIT_CARD;
  observer_data.notification (observer_data.instance, &card_data);
  EXPECT_EQ (func_call_count.edit_card, 1);
}

TEST_F (ModelObserverTests,
        DataChanged_EnumTaskIsMoveCard_MoveCardFuncPointerCalled)
{
  card_data.task = TASK_MOVE_CARD;
  observer_data.notification (observer_data.instance, &card_data);
  EXPECT_EQ (func_call_count.move_card, 1);
}

TEST_F (ModelObserverTests,
        DataChanged_EnumTaskIsAddColumn_AddColumnFuncPointerCalled)
{
  card_data.task = TASK_ADD_COLUMN;
  observer_data.notification (observer_data.instance, &card_data);
  EXPECT_EQ (func_call_count.add_column, 1);
}

TEST_F (ModelObserverTests,
        DataChanged_EnumTaskIsEditColumn_EditColumnFuncPointerCalled)
{
  card_data.task = TASK_EDIT_COLUMN;
  observer_data.notification (observer_data.instance, &card_data);
  EXPECT_EQ (func_call_count.edit_column, 1);
}

TEST_F (ModelObserverTests,
        DataChanged_EnumTaskIsMoveColumn_MoveColumnFuncPointerCalled)
{
  card_data.task = TASK_MOVE_COLUMN;
  observer_data.notification (observer_data.instance, &card_data);
  EXPECT_EQ (func_call_count.move_column, 1);
}


