/* tests/kanban-board-observer-interface-tests.cpp
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
  #include "../src/presenters/kanban-board-observer-interface.h"
  #include "../src/presenters/kanban-list-viewer-interface.h"

  #include <gtk/gtk.h>
}

#include "gtest/gtest.h"

static int add_column_called;

extern "C"
{
  #define MOCK_TYPE_IFACE_OBJECT (mock_iface_object_get_type ())
  G_DECLARE_FINAL_TYPE (MockIfaceObject, mock_iface_object, MOCK, IFACE_OBJECT, GObject)

  struct _MockIfaceObject
  {
    GObject parent_instance;
  };

  static void kanban_board_observer_iface_init (KanbanBoardObserverInterface *iface);

  G_DEFINE_TYPE_WITH_CODE (MockIfaceObject, mock_iface_object, G_TYPE_OBJECT,
                           G_IMPLEMENT_INTERFACE (KANBAN_TYPE_BOARD_OBSERVER,
                                                  kanban_board_observer_iface_init))

  static void
  mock_iface_object_add_column (KanbanBoardObserver     *self,
                                KanbanColumnObservable  *new_column,
                                gint                     priority)
  {
    ++add_column_called;
  }

  static void
  kanban_board_observer_iface_init (KanbanBoardObserverInterface *iface)
  {
    iface->add_column = mock_iface_object_add_column;
  }

  static void
  mock_iface_object_init (MockIfaceObject *self)
  {
    (void) self;
  }

  static void
  mock_iface_object_class_init (MockIfaceObjectClass *klass)
  {
    (void) klass;
  }
}

// Test Fixtures:
class KanbanBoardObserverInterfaceTests : public ::testing::Test
{
protected:
  gpointer mock_board_observer;

  void SetUp() override
  {
    mock_board_observer = g_object_new (MOCK_TYPE_IFACE_OBJECT, NULL);
    add_column_called = 0;
  }

  void TearDown() override
  {
    g_object_unref (mock_board_observer);
  }
};

// Tests:
TEST_F (KanbanBoardObserverInterfaceTests,
        AddColumn_InterfaceArguementIsNull_ImmediateReturnNoErrorsNoSideEffects)
{
  kanban_board_observer_add_column (NULL, NULL, 0);
  SUCCEED ();
}

TEST_F (KanbanBoardObserverInterfaceTests,
        AddColumn_InterfaceArguementIsValid_ArguementsAddColumnFunctionisCalled)
{
  kanban_board_observer_add_column (KANBAN_BOARD_OBSERVER (mock_board_observer),
                                    NULL, 0);
  EXPECT_EQ (add_column_called, 1);
}


