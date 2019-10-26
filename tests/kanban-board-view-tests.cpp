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
  #include "../src/presenters/kanban-column-observable-interface.h"

  #include <gtk/gtk.h>
}

#include "gtest/gtest.h"

// Stubs:
extern "C"
{
  // Mock Column View
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

  // Mock Column View Model
  #define MOCK_TYPE_COLUMN_VIEWMODEL (mock_column_viewmodel_get_type ())

  G_DECLARE_FINAL_TYPE (MockColumnViewModel, mock_column_viewmodel, MOCK, COLUMN_VIEWMODEL, GObject)

  struct _MockColumnViewModel
  {
    GObject  parent_instance;
    gint     column_id;
  };

  static void g_list_model_iface_init (GListModelInterface *iface);
  static void kanban_column_observable_iface_init (KanbanColumnObservableInterface *iface);

  G_DEFINE_TYPE_WITH_CODE (MockColumnViewModel, mock_column_viewmodel, G_TYPE_OBJECT,
                           G_IMPLEMENT_INTERFACE (G_TYPE_LIST_MODEL,
                                                  g_list_model_iface_init)
                           G_IMPLEMENT_INTERFACE (KANBAN_TYPE_COLUMN_OBSERVABLE,
                                                  kanban_column_observable_iface_init))

  static GType mock_column_viewmodel_get_item_type (GListModel *model)
  {
    (void) model;
    return G_TYPE_NONE;
  }
  
  static guint mock_column_viewmodel_get_n_items (GListModel *model)
  {
    (void) model;
    return 0;
  }
  
  static gpointer mock_column_viewmodel_get_item (GListModel *model,
                                                  guint       i)
  {
    (void) model;
    (void) i;
    return NULL;
  }
  
  static void g_list_model_iface_init (GListModelInterface *iface)
  {
    iface->get_item_type = mock_column_viewmodel_get_item_type;
    iface->get_n_items = mock_column_viewmodel_get_n_items;
    iface->get_item = mock_column_viewmodel_get_item;
  }
  
  static GtkTextBuffer *
  mock_column_viewmodel_get_heading (KanbanColumnObservable *model)
  {
    (void) model;
    return NULL;
  }
  
  static gint mock_column_viewmodel_get_id (KanbanColumnObservable *model)
  {
    return MOCK_COLUMN_VIEWMODEL (model)->column_id;
  }
  
  static void
  kanban_column_observable_iface_init (KanbanColumnObservableInterface *iface)
  {
    iface->get_heading = mock_column_viewmodel_get_heading;
    iface->get_id = mock_column_viewmodel_get_id;
  }
  
  static void mock_column_viewmodel_init (MockColumnViewModel *self)
  {
    (void) self;
  }
  
  static void
  mock_column_viewmodel_class_init (MockColumnViewModelClass *klass)
  {
    (void) klass;
  }
  
  MockColumnViewModel *mock_column_viewmodel_new (gint col_id)
  {
    gpointer obj = g_object_new (MOCK_TYPE_COLUMN_VIEWMODEL, NULL);
    MOCK_COLUMN_VIEWMODEL (obj)->column_id = col_id;
    return MOCK_COLUMN_VIEWMODEL (obj);
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
  int num_columns_before = kanban_board_view_count_columns (board);
  kanban_board_observer_add_column (KANBAN_BOARD_OBSERVER (board), NULL, 0);
  int num_columns_after = kanban_board_view_count_columns (board);
  KanbanColumnView *col_added = kanban_board_view_get_nth_column (board, 0);

  ASSERT_NE (num_columns_after, 0);
  ASSERT_NE (col_added, nullptr);
  EXPECT_EQ (num_columns_after, num_columns_before + 1);
}

TEST_F (KanbanBoardViewTests,
        AddColumn_ParametersValid_ColumnsAddedInPriorityOrder)
{
  KanbanColumnView *column_added1, *column_added2;
  KanbanColumnView *column_pos1, *column_pos2;
  kanban_board_observer_add_column (KANBAN_BOARD_OBSERVER (board), NULL, 0);
  column_added1 = kanban_board_view_get_nth_column (board, 0);
  kanban_board_observer_add_column (KANBAN_BOARD_OBSERVER (board), NULL, 0);
  column_added2 = kanban_board_view_get_nth_column (board, 0);


  column_pos1 = kanban_board_view_get_nth_column (board, 1);
  column_pos2 = kanban_board_view_get_nth_column (board, 0);


  EXPECT_EQ (column_pos1, column_added1);
  EXPECT_EQ (column_pos2, column_added2);
  EXPECT_NE (column_pos1, column_pos2);

}

TEST_F (KanbanBoardViewTests,
        MoveColumn_ValidIdNewPriority_ColumnWidgetMovesToPriorityPosition)
{
  KanbanColumnView *column1_before, *column1_after, *column2_before, *column2_after;
  MockColumnViewModel *viewmodel1 = mock_column_viewmodel_new (11);
  MockColumnViewModel *viewmodel2 = mock_column_viewmodel_new (12);
  kanban_board_observer_add_column (KANBAN_BOARD_OBSERVER (board),
                                    KANBAN_COLUMN_OBSERVABLE (viewmodel1), 0);
  kanban_board_observer_add_column (KANBAN_BOARD_OBSERVER (board),
                                    KANBAN_COLUMN_OBSERVABLE (viewmodel2), 1);
  column1_before = kanban_board_view_get_nth_column (board, 0);
  column2_before = kanban_board_view_get_nth_column (board, 1);

  kanban_board_observer_move_column (KANBAN_BOARD_OBSERVER (board), 11, 1);
  column1_after = kanban_board_view_get_nth_column (board, 0);
  column2_after = kanban_board_view_get_nth_column (board, 1);

  EXPECT_EQ (column1_after, column2_before);
  EXPECT_EQ (column2_after, column1_before);
  EXPECT_NE (column1_after, nullptr);
  EXPECT_NE (column2_after, nullptr);

  g_object_unref (viewmodel1);
  g_object_unref (viewmodel2);
}

