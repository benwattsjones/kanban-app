/* tests/kanban-column-view-tests.cpp
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
  #include "views/kanban-column-view.h"
  #include "presenters/kanban-column-observable-interface.h"
  #include "presenters/kanban-column-viewmodel.h"
  #include "models/kanban-data.h"

  #include <gtk/gtk.h>
}

#include "gtest/gtest.h"

// Stubs:
extern "C"
{
}

// Test Fixtures:
class KanbanColumnViewTests : public ::testing::Test
{
protected:
  KanbanData card_data;
  KanbanColumnViewModel *viewmodel;
  KanbanColumnView *view;

  void SetUp() override
  {
    gtk_init (0, NULL);
    card_data.card_id = 1;
    card_data.column_id = 2;
    card_data.heading = g_strdup ("card heading!");
    card_data.content = g_strdup ("card content.");
    card_data.priority = 0;

    viewmodel = kanban_column_viewmodel_new (card_data.column_id, "column name");

    view = kanban_column_view_new (KANBAN_COLUMN_OBSERVABLE (viewmodel));
  }

  void TearDown() override
  {
    g_object_ref_sink (view);

    kanban_column_viewmodel_destroy ((void *) viewmodel);
    g_free (card_data.heading);
    g_free (card_data.content);
  }
};


// Tests:
TEST_F (KanbanColumnViewTests,
        New_GtkInitializedNewCalled_ColumnCreatedNotNull)
{
  ASSERT_NE (view, nullptr);
}

TEST_F (KanbanColumnViewTests,
        AddCard_CardAddedToViewModel_CardAddedToView)
{
  int num_cards_before = kanban_column_view_count_cards (view);
  kanban_column_viewmodel_new_card (viewmodel, &card_data);
  int num_cards_after = kanban_column_view_count_cards (view);

  EXPECT_EQ (num_cards_after, num_cards_before + 1);
}

TEST_F (KanbanColumnViewTests,
        MoveCard_CardRemovedFromViewModelList_CardRemovedFromViewList)
{
  KanbanColumnViewModel *viewmodel2;
  GSequenceIter *card_iter;
  viewmodel2 = kanban_column_viewmodel_new (card_data.column_id+1, "Column Heading");
  card_iter = kanban_column_viewmodel_new_card (viewmodel, &card_data);

  int num_cards_before = kanban_column_view_count_cards (view);
  kanban_column_viewmodel_move_card (viewmodel, viewmodel2,
                                     card_iter, card_data.priority);
  int num_cards_after = kanban_column_view_count_cards (view);

  EXPECT_EQ (num_cards_after, num_cards_before - 1);

  kanban_column_viewmodel_destroy (viewmodel2);
}

