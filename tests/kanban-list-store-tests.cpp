/* tests/kanban-list-store-tests.cpp
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
  #include "../src/presenters/kanban-list-store.h"
  #include "../src/presenters/kanban-card-viewmodel.h"
  #include "../src/models/kanban-data.h"

  #include <gtk/gtk.h>
}

#include "gtest/gtest.h"


// Stubs:
extern "C"
{
}

// Test Fixtures:
class KanbanListStoreTests : public ::testing::Test
{
protected:
  KanbanData card_data;
  KanbanListStore *viewmodel;
  const char *column_name = "Column name";

  void SetUp() override
  {
    card_data.card_id = 1;
    card_data.column_id = 2;
    card_data.heading = g_strdup ("card heading!");
    card_data.content = g_strdup ("card content.");
    card_data.priority = 0;

    viewmodel = kanban_list_store_new (card_data.column_id, column_name);
  }

  void TearDown() override
  {
    kanban_list_store_destroy ((void *) viewmodel);
    g_free (card_data.heading);
    g_free (card_data.content);
  }
};

// Tests:
TEST_F (KanbanListStoreTests,
        New_ValidColumnIdPassed_NewObjectReturned)
{
  ASSERT_NE (viewmodel, nullptr);
}

TEST_F (KanbanListStoreTests,
        New_ValidColumnIdPassed_ColumnIdStoredAsRetrievableProperty)
{
  int result_column_id;
  g_object_get (viewmodel, "column-id", &result_column_id, NULL);
  EXPECT_EQ (result_column_id, card_data.column_id);
}

TEST_F (KanbanListStoreTests,
        GListModelGetNItems_NewOjectCreated_ItemCountInitiallyZero)
{
  int num_items = g_list_model_get_n_items (G_LIST_MODEL (viewmodel));
  EXPECT_EQ (num_items, 0);
}

TEST_F (KanbanListStoreTests,
        GListModelGetItem_NewObjectCreated_EmptyListReturnsNull)
{
  gpointer item = g_list_model_get_item (G_LIST_MODEL (viewmodel), 0);
  EXPECT_EQ (item, nullptr);
}

TEST_F (KanbanListStoreTests,
        GListModelGetNItems_NewCardCalled_CardCountIncrements)
{
  int num_items_orig = g_list_model_get_n_items (G_LIST_MODEL (viewmodel));
  kanban_list_store_new_card (viewmodel, &card_data);
  int num_items_new = g_list_model_get_n_items (G_LIST_MODEL (viewmodel));
  EXPECT_EQ (num_items_new, num_items_orig + 1);
}

TEST_F (KanbanListStoreTests,
        GListModelGetItem_NewCardCalled_NewCardIsReturned)
{
  kanban_list_store_new_card (viewmodel, &card_data);
  KanbanCardViewModel *card = KANBAN_CARD_VIEWMODEL
      (g_list_model_get_item (G_LIST_MODEL (viewmodel), card_data.priority));
  char *content_stored;
  g_object_get (card, "content", &content_stored, NULL);
  EXPECT_STREQ (card_data.content, content_stored);
  free (content_stored);
  g_object_unref (card);
}

TEST_F (KanbanListStoreTests,
        GListModelGetItem_CardRetrievedAndFreed_CardIsNotRemovedFromList)
{
  kanban_list_store_new_card (viewmodel, &card_data);
  KanbanCardViewModel *card1 = KANBAN_CARD_VIEWMODEL
      (g_list_model_get_item (G_LIST_MODEL (viewmodel), card_data.priority));
  g_object_unref (card1);
  KanbanCardViewModel *card2 = KANBAN_CARD_VIEWMODEL
      (g_list_model_get_item (G_LIST_MODEL (viewmodel), card_data.priority));
  char *content_stored;
  g_object_get (card2, "content", &content_stored, NULL);
  EXPECT_STREQ (card_data.content, content_stored);
  free (content_stored);
  g_object_unref (card2);
}

TEST_F (KanbanListStoreTests,
        NewCard_MultipleCardsAddedPriorityOrder_CardsStoredInPriorityOrder)
{
  int card0_id=42, card1_id=769, card2_id=33;
  card_data.priority = 0;
  card_data.card_id = card0_id;
  kanban_list_store_new_card (viewmodel, &card_data);
  card_data.priority = 1;
  card_data.card_id = card1_id;
  kanban_list_store_new_card (viewmodel, &card_data);
  card_data.priority = 2;
  card_data.card_id = card2_id;
  kanban_list_store_new_card (viewmodel, &card_data);
  int card0_id_result, card1_id_result, card2_id_result;
  KanbanCardViewModel *card;

  card = KANBAN_CARD_VIEWMODEL (g_list_model_get_item (G_LIST_MODEL (viewmodel), 0));
  g_object_get (card, "card-id", &card0_id_result, NULL);
  g_object_unref (card);
  card = KANBAN_CARD_VIEWMODEL (g_list_model_get_item (G_LIST_MODEL (viewmodel), 2));
  g_object_get (card, "card-id", &card2_id_result, NULL);
  g_object_unref (card);
  card = KANBAN_CARD_VIEWMODEL (g_list_model_get_item (G_LIST_MODEL (viewmodel), 1));
  g_object_get (card, "card-id", &card1_id_result, NULL);
  g_object_unref (card);

  EXPECT_EQ (card0_id, card0_id_result);
  EXPECT_EQ (card1_id, card1_id_result);
  EXPECT_EQ (card2_id, card2_id_result);
}

TEST_F (KanbanListStoreTests,
        NewCard_MultipleCardsAdded_CountCorrect)
{
  card_data.priority = 0;
  card_data.card_id = 734;
  kanban_list_store_new_card (viewmodel, &card_data);
  card_data.priority = 1;
  card_data.card_id = 98087;
  kanban_list_store_new_card (viewmodel, &card_data);
  card_data.priority = 2;
  card_data.card_id = 223;
  kanban_list_store_new_card (viewmodel, &card_data);

  int num_items = g_list_model_get_n_items (G_LIST_MODEL (viewmodel));
  EXPECT_EQ (3, num_items);
}

