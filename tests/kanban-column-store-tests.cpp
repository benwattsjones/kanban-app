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
  #include "../src/presenters/kanban-list-store.h"
  #include "../src/presenters/kanban-card-viewmodel.h"

  #include <gtk/gtk.h>
}

#include "gtest/gtest.h"


// Stubs:
extern "C"
{
/*
  KanbanListStore *kanban_list_store_new (gint col_id)
  {
    (void) col_id;
    return nullptr;
  }

  void kanban_list_store_destroy (KanbanListStore *viewmodel)
  {
    (void) viewmodel;
  }
*/
  void register_kanban_viewmodel_observer   (KanbanColumnStore *viewmodel)
  {
    (void) viewmodel;
  }

  void deregister_kanban_viewmodel_observer (KanbanColumnStore *viewmodel)
  {
    (void) viewmodel;
  }
/*
  void kanban_card_viewmodel_update_contents (KanbanCardViewModel *card,
                                              const gchar         *heading,
                                              const gchar         *content)
  {
    (void) card;
    (void) heading;
    (void) content;
  }

  GSequenceIter *kanban_list_store_new_card (KanbanListStore  *self,
                                             const KanbanCard *card_data)
  {
    (void) self;
    (void) card_data;
    return NULL;
  }
*/
}

// Test Fixtures:
class KanbanColumnStoreTests : public ::testing::Test
{
protected:
  KanbanColumnStore *viewmodel;
  KanbanCard card_data;

  void SetUp() override
  {
    card_data.card_id = 1;
    card_data.column_id = 2;
    card_data.heading = g_strdup("card heading!");
    card_data.content = g_strdup("card content.");
    card_data.priority = 0;

    viewmodel = kanban_column_store_new ();
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
  kanban_column_store_add_card (viewmodel, &card_data);
  KanbanCardViewModel *card_added = kanban_column_store_get_card (viewmodel,
                                        card_data.card_id);
  g_object_get (card_added, "card-id", &card_id_stored, NULL);
  g_object_get (card_added, "heading", &heading_stored, NULL);
  ASSERT_NE (card_added, nullptr);
  EXPECT_EQ (card_id_stored, card_data.card_id);
  EXPECT_STREQ (heading_stored, card_data.heading);
  g_free (heading_stored);
}

