/* tests/application-entry-tests.cpp
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
  #include "../src/kanban-application.h"
  #include "../src/presenters/kanban-column-store.h"
  #include "../src/presenters/model-observer-interface.h"
  #include "../src/views/kanban-window.h"
  #include "../src/views/kanban-grid.h"
  #include <config.h>

  #include <gtk/gtk.h>
}

#include "gtest/gtest.h"

// Stubs:
extern "C"
{
  KanbanWindow *kanban_window_new (KanbanApplication *app)
  {
    (void) app;
    return NULL;
  }

  void test_observers()
  {
  }

  void kanban_column_store_finalize (KanbanColumnStore *self)
  {
    (void) self;
  }

  void register_kanban_viewmodel_observer (ModelObserverInterface *observer)
  {
    (void) observer;
  }

  void deregister_kanban_viewmodel_observer (ModelObserverInterface *observer)
  {
    (void) observer;
  }

  KanbanGrid *kanban_grid_new ()
  {
    return NULL;
  }
}


// Test Fixtures:
class PresentersCardsIntergrationTests : public ::testing::Test
{
protected:
  gpointer app;

  void SetUp() override
  {
    app = g_object_new (KANBAN_APPLICATION_TYPE,
                        "application-id", APPLICATION_ID,
                        "flags", G_APPLICATION_HANDLES_OPEN,
                        NULL);
    gboolean registered = g_application_register (G_APPLICATION (app), NULL, NULL);
    ASSERT_EQ (true, registered);
    g_application_activate (G_APPLICATION (app));
  }

  void TearDown() override
  {
    g_application_quit (G_APPLICATION (app));
    g_object_unref (app);
  }
};


// Tests:
TEST_F(PresentersCardsIntergrationTests, checkApplicationNotNull)
{
  ASSERT_NE (app, nullptr);
}


