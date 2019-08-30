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
  KanbanListStore *kanban_list_store_new (gint col_id)
  {
    (void) col_id;
    return nullptr;
  }

  void kanban_list_store_destroy (KanbanListStore *viewmodel)
  {
    (void) viewmodel;
  }

  void register_kanban_viewmodel_observer   (KanbanColumnStore *viewmodel)
  {
    (void) viewmodel;
  }

  void deregister_kanban_viewmodel_observer (KanbanColumnStore *viewmodel)
  {
    (void) viewmodel;
  }

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
}

// Test Fixtures:
class KanbanColumnStoreTests : public ::testing::Test
{
protected:
  KanbanColumnStore *viewmodel;

  void SetUp() override
  {
    viewmodel = kanban_column_store_new ();
  }

  void TearDown() override
  {
    kanban_column_store_destroy (viewmodel);
  }
};

// Tests:
TEST_F(KanbanColumnStoreTests, checkKanbanColumnStoreCreated)
{
  ASSERT_NE (viewmodel, nullptr);
}


