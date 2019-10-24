/* tests/kanban-board-view-tests.cpp
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
  #include "../src/views/kanban-board-view.h"
  #include "../src/views/kanban-column-view.h"
  #include "../src/presenters/kanban-board-observer-interface.h"

  #include <gtk/gtk.h>
}

#include "gtest/gtest.h"

// Stubs:
extern "C"
{
  struct _KanbanColumnView
  {
    GtkBox  parent_instance;
  };

  G_DEFINE_TYPE (KanbanColumnView, kanban_column_view, GTK_TYPE_BOX)

  static void
  kanban_column_view_init (KanbanColumnView *self)
  {
    (void) self;
  }

  static void
  kanban_column_view_class_init (KanbanColumnViewClass *klass)
  {
    (void) klass;
  }

  KanbanColumnView *kanban_column_view_new (KanbanColumnObservable *column_data)
  {
    (void) column_data;
    return KANBAN_COLUMN_VIEW (g_object_new (KANBAN_TYPE_COLUMN_VIEW, NULL));
  }
}

// Test Fixtures:
class KanbanBoardViewTests : public ::testing::Test
{
protected:
  KanbanBoardView *board;

  void SetUp() override
  {
    gtk_init (0, NULL);
    board = kanban_board_view_new ();
  }

  void TearDown() override
  {
    g_object_ref_sink (board);
  }
};


// Tests:
TEST_F (KanbanBoardViewTests,
        New_GtkInitializedNewCalled_BoardCreatedNotNull)
{
  ASSERT_NE (board, nullptr);
}

TEST_F (KanbanBoardViewTests,
        New_ObjectCreated_CanBeCastToObserverInterface)
{
  KanbanBoardObserver *iface = KANBAN_BOARD_OBSERVER (board);
  ASSERT_NE (iface, nullptr);
  EXPECT_TRUE (KANBAN_IS_BOARD_OBSERVER (iface));
}

TEST_F (KanbanBoardViewTests,
        AddColumn_ParametersValid_NewColumnAdded)
{
  int num_columns_before = kanban_board_count_columns (board);
  kanban_board_observer_add_column (KANBAN_BOARD_OBSERVER (board), NULL, 0);
  int num_columns_after = kanban_board_count_columns (board);

  ASSERT_NE (num_columns_after, 0);
  EXPECT_EQ (num_columns_after, num_columns_before + 1);
}

