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
  #include "../src/presenters/kanban-column-viewer-interface.h"
  #include "../src/views/kanban-window.h"
  #include "../src/views/kanban-grid.h"
  #include <config.h>

  #include <gtk/gtk.h>
}

#include "gtest/gtest.h"

// Stubs:
static struct _FunctionCallTracker
{
  int func_call_counter = 0;
  int initialize_kanban_view_func_count = 0;
  int initialize_viewmodel_func_count = 0;
  int destroy_viewmodel_func_count = 0;
} FunctionCallTracker, FunctionCallTrackerReset;

extern "C"
{
  KanbanWindow *kanban_window_new (KanbanApplication *app)
  {
    (void) app;
    FunctionCallTracker.initialize_kanban_view_func_count =
        ++FunctionCallTracker.func_call_counter;
    return NULL;
  }

  void test_observers()
  {
  }

  KanbanColumnStore *kanban_column_store_new (KanbanColumnViewer *view_observer)
  {
    (void) view_observer;
    FunctionCallTracker.initialize_viewmodel_func_count =
        ++FunctionCallTracker.func_call_counter;
    return NULL;
  }

  void kanban_column_store_destroy (KanbanColumnStore *self)
  {
    (void) self;
    FunctionCallTracker.destroy_viewmodel_func_count =
        ++FunctionCallTracker.func_call_counter;
  }

  KanbanGrid *kanban_grid_new()
  {
    return NULL;
  }
}

// Test Fixtures:
class ApplicationEntryTests : public ::testing::Test
{
protected:
  gpointer app;
  int argc_stub = 0;
  char **argv_stub = NULL;
  static gchar *printed_string;

  static void redirect_gprint(const gchar *string)
  {
    g_free (printed_string);
    printed_string = g_strdup (string);
  }

  void SetUp() override
  {
    printed_string = NULL;
    g_set_print_handler (redirect_gprint);
    FunctionCallTracker = FunctionCallTrackerReset;
    app = g_object_new (KANBAN_APPLICATION_TYPE,
                        "application-id", APPLICATION_ID,
                        "flags", G_APPLICATION_HANDLES_OPEN,
                        NULL);
  }

  void TearDown() override
  {
    g_object_unref (app);
    g_free (printed_string);
  }
};

gchar *ApplicationEntryTests::printed_string = NULL;

// Tests:
TEST_F(ApplicationEntryTests, checkExitCodeSuccess)
{
  int status = g_application_run (G_APPLICATION (app), argc_stub, argv_stub);
  EXPECT_EQ(status, 0);
}

TEST_F(ApplicationEntryTests, checkVersionOptionPrints)
{
  char *prog_name = g_strdup("prog");
  char *cmd_options = g_strdup("--version");
  char *opt[] = {prog_name, cmd_options};
  int status = g_application_run (G_APPLICATION (app), 2, opt);
  EXPECT_EQ(status, 0);
  EXPECT_STREQ(printed_string, PACKAGE_NAME " " PACKAGE_VERSION "\n");
  g_free(prog_name);
  g_free(cmd_options);
}

TEST_F(ApplicationEntryTests, checkInitializationCallOrder)
{
  int status = g_application_run (G_APPLICATION (app), argc_stub, argv_stub);
  EXPECT_EQ(status, 0);
  EXPECT_EQ(FunctionCallTracker.initialize_viewmodel_func_count, 1);
  EXPECT_EQ(FunctionCallTracker.initialize_kanban_view_func_count, 2);
  EXPECT_EQ(FunctionCallTracker.destroy_viewmodel_func_count, 3);
}

TEST_F(ApplicationEntryTests, checkInitializationFunctionReturnsSuccess)
{
  int status = initialize_kanban_application (argc_stub, argv_stub);
  EXPECT_EQ(status, 0);
}

