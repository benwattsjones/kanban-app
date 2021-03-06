/* tests/kanban-column-viewmodel-tests.cpp
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
  #include "../src/presenters/kanban-column-viewmodel.h"
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
class KanbanColumnViewModelTests : public ::testing::Test
{
protected:
  KanbanData card_data;
  KanbanColumnViewModel *viewmodel;
  const char *column_name = "Column name";

  void SetUp() override
  {
    card_data.card_id = 1;
    card_data.column_id = 2;
    card_data.heading = g_strdup ("card heading!");
    card_data.content = g_strdup ("card content.");
    card_data.priority = 0;

    viewmodel = kanban_column_viewmodel_new (card_data.column_id, column_name);
  }

  void TearDown() override
  {
    kanban_column_viewmodel_destroy ((void *) viewmodel);
    g_free (card_data.heading);
    g_free (card_data.content);
  }
};

// Tests:
TEST_F (KanbanColumnViewModelTests,
        New_ValidColumnIdPassed_NewObjectReturned)
{
  ASSERT_NE (viewmodel, nullptr);
}

TEST_F (KanbanColumnViewModelTests,
        New_NullColumnNamePassed_NewObjectReturnedNoError)
{
  KanbanColumnViewModel *new_column;
  new_column = kanban_column_viewmodel_new (2, NULL);
  ASSERT_NE (viewmodel, nullptr);
  kanban_column_viewmodel_destroy ((void *) new_column);
}

TEST_F (KanbanColumnViewModelTests,
        New_ValidColumnIdPassed_ColumnIdStoredAsRetrievableProperty)
{
  int result_column_id;
  g_object_get (viewmodel, "column-id", &result_column_id, NULL);
  EXPECT_EQ (result_column_id, card_data.column_id);
}

TEST_F (KanbanColumnViewModelTests,
        New_ValidColumnNamePassed_ColumnNameStoredAsRetrievableProperty)
{
  char *result_column_name;
  g_object_get (viewmodel, "column-name", &result_column_name, NULL);
  EXPECT_STREQ (result_column_name, column_name);
  g_free (result_column_name);
}

TEST_F (KanbanColumnViewModelTests,
        SetHeading_ValidColumnNamePassed_ColumnNameUpdatedAsRetrievableProperty)
{
  char *result_column_name;
  const char *new_column_name = "My new column name!";
  kanban_column_viewmodel_set_heading (viewmodel, new_column_name);
  g_object_get (viewmodel, "column-name", &result_column_name, NULL);
  EXPECT_STREQ (result_column_name, new_column_name);
  g_free (result_column_name);
}

TEST_F (KanbanColumnViewModelTests,
        GListModelGetNItems_NewOjectCreated_ItemCountInitiallyZero)
{
  int num_items = g_list_model_get_n_items (G_LIST_MODEL (viewmodel));
  EXPECT_EQ (num_items, 0);
}

TEST_F (KanbanColumnViewModelTests,
        GListModelGetItem_NewObjectCreated_EmptyListReturnsNull)
{
  gpointer item = g_list_model_get_item (G_LIST_MODEL (viewmodel), 0);
  EXPECT_EQ (item, nullptr);
}

TEST_F (KanbanColumnViewModelTests,
        GListModelGetNItems_NewCardCalled_CardCountIncrements)
{
  int num_items_orig = g_list_model_get_n_items (G_LIST_MODEL (viewmodel));
  kanban_column_viewmodel_new_card (viewmodel, &card_data);
  int num_items_new = g_list_model_get_n_items (G_LIST_MODEL (viewmodel));
  EXPECT_EQ (num_items_new, num_items_orig + 1);
}

TEST_F (KanbanColumnViewModelTests,
        GListModelGetItem_NewCardCalled_NewCardIsReturned)
{
  kanban_column_viewmodel_new_card (viewmodel, &card_data);
  KanbanCardViewModel *card = KANBAN_CARD_VIEWMODEL
      (g_list_model_get_item (G_LIST_MODEL (viewmodel), card_data.priority));
  char *content_stored;
  g_object_get (card, "content", &content_stored, NULL);
  EXPECT_STREQ (card_data.content, content_stored);
  free (content_stored);
  g_object_unref (card);
}

TEST_F (KanbanColumnViewModelTests,
        GListModelGetItem_CardRetrievedAndFreed_CardIsNotRemovedFromList)
{
  kanban_column_viewmodel_new_card (viewmodel, &card_data);
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

TEST_F (KanbanColumnViewModelTests,
        NewCard_MultipleCardsAddedPriorityOrder_CardsStoredInPriorityOrder)
{
  int card0_id=42, card1_id=769, card2_id=33;
  card_data.priority = 0;
  card_data.card_id = card0_id;
  kanban_column_viewmodel_new_card (viewmodel, &card_data);
  card_data.priority = 1;
  card_data.card_id = card1_id;
  kanban_column_viewmodel_new_card (viewmodel, &card_data);
  card_data.priority = 2;
  card_data.card_id = card2_id;
  kanban_column_viewmodel_new_card (viewmodel, &card_data);
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

TEST_F (KanbanColumnViewModelTests,
        NewCard_MultipleCardsAdded_CountCorrect)
{
  card_data.priority = 0;
  card_data.card_id = 734;
  kanban_column_viewmodel_new_card (viewmodel, &card_data);
  card_data.priority = 1;
  card_data.card_id = 98087;
  kanban_column_viewmodel_new_card (viewmodel, &card_data);
  card_data.priority = 2;
  card_data.card_id = 223;
  kanban_column_viewmodel_new_card (viewmodel, &card_data);

  int num_items = g_list_model_get_n_items (G_LIST_MODEL (viewmodel));
  EXPECT_EQ (3, num_items);
}

TEST_F (KanbanColumnViewModelTests,
        MoveCard_OldAndNewSame_NoChangeNoErrors)
{
  GSequenceIter *card_iter;
  int num_items_before, num_items_after;
  // Note that in C (not C++) you can implicitly convert void* to actual type
  gpointer first_card_before, first_card_after;
  kanban_column_viewmodel_new_card (viewmodel, &card_data);
  card_data.priority++;
  card_data.card_id++;
  card_iter = kanban_column_viewmodel_new_card (viewmodel, &card_data);

  first_card_before = g_list_model_get_item (G_LIST_MODEL (viewmodel), 0);
  num_items_before = g_list_model_get_n_items (G_LIST_MODEL (viewmodel));
  kanban_column_viewmodel_move_card (viewmodel, viewmodel,
                                     card_iter, card_data.priority);
  num_items_after = g_list_model_get_n_items (G_LIST_MODEL (viewmodel));
  first_card_after = g_list_model_get_item (G_LIST_MODEL (viewmodel), 0);

  ASSERT_NE (first_card_after, nullptr);
  ASSERT_NE (num_items_after, 0);
  EXPECT_EQ (first_card_after, first_card_before);
  EXPECT_EQ (num_items_after, num_items_before);

  g_object_unref (first_card_before);
  g_object_unref (first_card_after);
}

TEST_F (KanbanColumnViewModelTests,
        MoveCard_ColumnSamePositionIncrease_CardMovesCountUnchanged)
{
  GSequenceIter *card_iter;
  int num_items_before, num_items_after;
  gpointer first_card_before, first_card_after, second_card_before, second_card_after;
  card_iter = kanban_column_viewmodel_new_card (viewmodel, &card_data);
  card_data.priority++;
  card_data.card_id++;
  kanban_column_viewmodel_new_card (viewmodel, &card_data);

  first_card_before = g_list_model_get_item (G_LIST_MODEL (viewmodel), 0);
  second_card_before = g_list_model_get_item (G_LIST_MODEL (viewmodel), 1);
  num_items_before = g_list_model_get_n_items (G_LIST_MODEL (viewmodel));
  kanban_column_viewmodel_move_card (viewmodel, viewmodel,
                                     card_iter, card_data.priority);
  num_items_after = g_list_model_get_n_items (G_LIST_MODEL (viewmodel));
  first_card_after = g_list_model_get_item (G_LIST_MODEL (viewmodel), 0);
  second_card_after = g_list_model_get_item (G_LIST_MODEL (viewmodel), 1);

  ASSERT_NE (first_card_after, nullptr);
  ASSERT_NE (num_items_after, 0);
  EXPECT_EQ (first_card_after, second_card_before);
  EXPECT_EQ (second_card_after, first_card_before);
  EXPECT_EQ (num_items_after, num_items_before);

  g_object_unref (first_card_before);
  g_object_unref (first_card_after);
  g_object_unref (second_card_before);
  g_object_unref (second_card_after);
}

TEST_F (KanbanColumnViewModelTests,
        MoveCard_ColumnSamePositionDecrease_CardMovesCountUnchanged)
{
  GSequenceIter *card_iter;
  int num_items_before, num_items_after;
  gpointer first_card_before, first_card_after, second_card_before, second_card_after;
  kanban_column_viewmodel_new_card (viewmodel, &card_data);
  card_data.priority++;
  card_data.card_id++;
  card_iter = kanban_column_viewmodel_new_card (viewmodel, &card_data);

  first_card_before = g_list_model_get_item (G_LIST_MODEL (viewmodel), 0);
  second_card_before = g_list_model_get_item (G_LIST_MODEL (viewmodel), 1);
  num_items_before = g_list_model_get_n_items (G_LIST_MODEL (viewmodel));
  kanban_column_viewmodel_move_card (viewmodel, viewmodel,
                                     card_iter, --card_data.priority);
  num_items_after = g_list_model_get_n_items (G_LIST_MODEL (viewmodel));
  first_card_after = g_list_model_get_item (G_LIST_MODEL (viewmodel), 0);
  second_card_after = g_list_model_get_item (G_LIST_MODEL (viewmodel), 1);

  ASSERT_NE (first_card_after, nullptr);
  ASSERT_NE (num_items_after, 0);
  EXPECT_EQ (first_card_after, second_card_before);
  EXPECT_EQ (second_card_after, first_card_before);
  EXPECT_EQ (num_items_after, num_items_before);

  g_object_unref (first_card_before);
  g_object_unref (first_card_after);
  g_object_unref (second_card_before);
  g_object_unref (second_card_after);
}

TEST_F (KanbanColumnViewModelTests,
        MoveCard_ColumnChanges_CardMovesCountsUpdated)
{
  GSequenceIter *card_iter;
  KanbanColumnViewModel *new_column;
  int num_items_col1_before, num_items_col1_after, num_items_col2_after;
  gpointer card_before, card_after;
  new_column = kanban_column_viewmodel_new (++card_data.column_id, "Column Heading");
  card_iter = kanban_column_viewmodel_new_card (viewmodel, &card_data);

  card_before = g_list_model_get_item (G_LIST_MODEL (viewmodel), card_data.priority);
  num_items_col1_before = g_list_model_get_n_items (G_LIST_MODEL (viewmodel));
  kanban_column_viewmodel_move_card (viewmodel, new_column,
                                     card_iter, card_data.priority);
  num_items_col1_after = g_list_model_get_n_items (G_LIST_MODEL (viewmodel));
  num_items_col2_after = g_list_model_get_n_items (G_LIST_MODEL (new_column));
  card_after = g_list_model_get_item (G_LIST_MODEL (new_column), card_data.priority);

  ASSERT_NE (card_after, nullptr);
  EXPECT_EQ (card_after, card_before);
  EXPECT_EQ (num_items_col2_after, num_items_col1_before);
  EXPECT_EQ (num_items_col1_after, num_items_col1_before - 1);

  g_object_unref (card_before);
  g_object_unref (card_after);
  kanban_column_viewmodel_destroy (new_column);
}

