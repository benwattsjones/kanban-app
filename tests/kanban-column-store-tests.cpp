/* tests/kanban-column-store-tests.cpp
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
  #include "../src/presenters/kanban-column-store.h"
  #include "../src/presenters/kanban-column-viewer-interface.h"
  #include "../src/presenters/model-observer-interface.h"
  #include "../src/presenters/kanban-list-store.h"
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

  void kanban_column_viewer_add_column (KanbanColumnViewer *self,
                                        GListModel         *new_column)
  {
    (void) self;
    (void) new_column;
  }
}

// Test Fixtures:
class KanbanColumnStoreTests : public ::testing::Test
{
protected:
  KanbanColumnStore *viewmodel;
  ModelObserverInterface *observer;
  KanbanData card_data;

  void SetUp() override
  {
    card_data.card_id = 1;
    card_data.column_id = 2;
    card_data.heading = g_strdup ("heading!");
    card_data.content = g_strdup ("content.");
    card_data.priority = 0;

    viewmodel = kanban_column_store_new (NULL);
    observer = kanban_column_store_get_observer (viewmodel);
  }

  void TearDown() override
  {
    kanban_column_store_destroy (viewmodel);
    g_free (card_data.heading);
    g_free (card_data.content);
  }
};

// Tests:
TEST_F (KanbanColumnStoreTests,
        New_NullPassed_NewObjectReturned)
{
  ASSERT_NE (viewmodel, nullptr);
}

TEST_F (KanbanColumnStoreTests,
        AddColumn_ValidDataPassed_NewColumnAddedToObjectAndRetrievable)
{
  int column_id_stored;
  KanbanListStore *column_added;
  observer->task_func[TASK_ADD_COLUMN] (observer->viewmodel, &card_data);
  column_added = kanban_column_store_get_column (viewmodel, card_data.column_id);

  g_object_get (column_added, "column-id", &column_id_stored, NULL);
  ASSERT_NE (column_added, nullptr);
  EXPECT_EQ (column_id_stored, card_data.column_id);
}

TEST_F (KanbanColumnStoreTests,
        AddCard_ValidDataPassed_NewCardCreatedAndRetrievable)
{
  int card_id_stored;
  char *heading_stored;
  KanbanCardViewModel *card_added;
  observer->task_func[TASK_ADD_COLUMN] (observer->viewmodel, &card_data);
  observer->task_func[TASK_ADD_CARD] (observer->viewmodel, &card_data);
  card_added = kanban_column_store_get_card (viewmodel, card_data.card_id);

  g_object_get (card_added, "card-id", &card_id_stored, NULL);
  g_object_get (card_added, "heading", &heading_stored, NULL);
  ASSERT_NE (card_added, nullptr);
  EXPECT_EQ (card_id_stored, card_data.card_id);
  EXPECT_STREQ (card_data.heading, heading_stored);

  g_free (heading_stored);
}

TEST_F (KanbanColumnStoreTests,
        EditCard_NewHeadingDataPassed_CardHeadingChanged)
{
  KanbanCardViewModel *card;
  char *heading_stored;
  observer->task_func[TASK_ADD_COLUMN] (observer->viewmodel, &card_data);
  observer->task_func[TASK_ADD_CARD] (observer->viewmodel, &card_data);
  card = kanban_column_store_get_card (viewmodel, card_data.card_id);
  free (card_data.heading);
  card_data.heading = g_strdup ("new heading");
  observer->task_func[TASK_EDIT_CARD] (observer->viewmodel, &card_data);

  g_object_get (card, "heading", &heading_stored, NULL);
  EXPECT_STREQ (card_data.heading, heading_stored);

  g_free (heading_stored);
}

TEST_F (KanbanColumnStoreTests,
        MoveCard_CurrantColumnAndPriorityPassed_BoardUnchanged)
{
  KanbanListStore *column;
  gpointer card1, card2, top_card_before, bottom_card_before,
           top_card_after, bottom_card_after;
  observer->task_func[TASK_ADD_COLUMN] (observer->viewmodel, &card_data);
  observer->task_func[TASK_ADD_CARD] (observer->viewmodel, &card_data);
  card1 = kanban_column_store_get_card (viewmodel, card_data.card_id);
  card_data.card_id = 2;
  card_data.priority = 1;
  observer->task_func[TASK_ADD_CARD] (observer->viewmodel, &card_data);
  card2 = kanban_column_store_get_card (viewmodel, card_data.card_id);

  column = kanban_column_store_get_column (viewmodel, card_data.column_id);
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

TEST_F (KanbanColumnStoreTests,
        MoveCard_NewPriorityPassed_CardMovesToPriorityPosition)
{
  KanbanListStore *column;
  gpointer card1, card2, top_card_before, bottom_card_before,
           top_card_after, bottom_card_after;
  observer->task_func[TASK_ADD_COLUMN] (observer->viewmodel, &card_data);
  observer->task_func[TASK_ADD_CARD] (observer->viewmodel, &card_data);
  card1 = kanban_column_store_get_card (viewmodel, card_data.card_id);
  card_data.card_id = 2;
  card_data.priority = 1;
  observer->task_func[TASK_ADD_CARD] (observer->viewmodel, &card_data);
  card2 = kanban_column_store_get_card (viewmodel, card_data.card_id);

  column = kanban_column_store_get_column (viewmodel, card_data.column_id);
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

TEST_F (KanbanColumnStoreTests,
        MoveCard_NewColumnPassed_CardMovesToNewColumn)
{
  KanbanListStore *column1, *column2;
  gint col1_count_before, col2_count_before, col1_count_after, col2_count_after;
  observer->task_func[TASK_ADD_COLUMN] (observer->viewmodel, &card_data);
  observer->task_func[TASK_ADD_CARD] (observer->viewmodel, &card_data);
  column1 = kanban_column_store_get_column (viewmodel, card_data.column_id);
  card_data.column_id++;
  observer->task_func[TASK_ADD_COLUMN] (observer->viewmodel, &card_data);
  column2 = kanban_column_store_get_column (viewmodel, card_data.column_id);

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

