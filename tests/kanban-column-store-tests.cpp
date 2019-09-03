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
  #include "../src/presenters/model-observer-interface.h"
  #include "../src/presenters/kanban-list-store.h"
  #include "../src/presenters/kanban-card-viewmodel.h"
  #include "../src/models/kanban-data.h"

  #include <gtk/gtk.h>
}

#include "gtest/gtest.h"


// Stubs:
extern "C"
{
  void register_kanban_viewmodel_observer   (ModelObserverInterface *observer)
  {
    (void) observer;
  }

  void deregister_kanban_viewmodel_observer (ModelObserverInterface *observer)
  {
    (void) observer;
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
    card_data.heading = g_strdup("card heading!");
    card_data.content = g_strdup("card content.");
    card_data.priority = 0;

    viewmodel = kanban_column_store_new ();
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
TEST_F(KanbanColumnStoreTests, checkKanbanColumnStoreCreated)
{
  ASSERT_NE (viewmodel, nullptr);
}

TEST_F(KanbanColumnStoreTests, checkAddCardStoresInTable)
{
  int card_id_stored;
  char *heading_stored;
  observer->task_func[TASK_ADD_CARD] (observer->viewmodel, &card_data);
  KanbanCardViewModel *card_added;
  card_added = kanban_column_store_get_card (viewmodel, card_data.card_id);

  g_object_get (card_added, "card-id", &card_id_stored, NULL);
  g_object_get (card_added, "heading", &heading_stored, NULL);
  ASSERT_NE (card_added, nullptr);
  EXPECT_EQ (card_id_stored, card_data.card_id);
  EXPECT_STREQ (card_data.heading, heading_stored);

  g_free (heading_stored);
}

TEST_F(KanbanColumnStoreTests, checkEditCardSavesNewContents)
{
  KanbanCardViewModel *card;
  char *heading_stored;
  observer->task_func[TASK_ADD_CARD] (observer->viewmodel, &card_data);
  card = kanban_column_store_get_card (viewmodel, card_data.card_id);
  free (card_data.heading);
  card_data.heading = g_strdup ("new heading");
  observer->task_func[TASK_EDIT_CARD] (observer->viewmodel, &card_data);

  g_object_get (card, "heading", &heading_stored, NULL);
  EXPECT_STREQ (card_data.heading, heading_stored);

  g_free (heading_stored);
}


