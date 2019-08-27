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
  #include "../src/models/kanban-cards.h"

  #include <gtk/gtk.h>
}

#include "gtest/gtest.h"

// Stubs:
extern "C"
{
  void register_kanban_viewmodel_observer   (KanbanListStore *viewmodel)
  {
    (void) viewmodel;
  }

  void deregister_kanban_viewmodel_observer (KanbanListStore *viewmodel)
  {
    (void) viewmodel;
  }
}

// Test Fixtures:
class KanbanListStoreTests : public ::testing::Test
{
protected:
  KanbanCard card_data;
  KanbanListStore *viewmodel;

  void SetUp() override
  {
    card_data.card_id = 1;
    card_data.column_id = 2;
    card_data.heading = g_strdup("card heading!");
    card_data.content = g_strdup("card content.");
    card_data.priority = 3;

    viewmodel = initialize_viewmodel ();
  }

  void TearDown() override
  {
    destroy_viewmodel (viewmodel);
    g_free (card_data.heading);
    g_free (card_data.content);
  }
};

// Tests:
TEST_F(KanbanListStoreTests, checkKanbanListStoreCreated)
{
  ASSERT_NE (viewmodel, nullptr);
}

TEST_F(KanbanListStoreTests, checkInitialCountZero)
{
  int num_items = g_list_model_get_n_items (G_LIST_MODEL (viewmodel));
  EXPECT_EQ (num_items, 0);
}

TEST_F(KanbanListStoreTests, checkEmptyListItemReturnsNull)
{
  gpointer item = g_list_model_get_item (G_LIST_MODEL (viewmodel), 0);
  EXPECT_EQ (item, nullptr);
}

TEST_F(KanbanListStoreTests, checkAddCardIncrementsCount)
{
  int num_items_orig = g_list_model_get_n_items (G_LIST_MODEL (viewmodel));
  kanban_list_store_new_card (viewmodel, &card_data);
  int num_items_new = g_list_model_get_n_items (G_LIST_MODEL (viewmodel));
  EXPECT_EQ (num_items_new, num_items_orig + 1);
}



