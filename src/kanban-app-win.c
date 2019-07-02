#include <gtk/gtk.h>

#include "kanban-app.h"
#include "kanban-app-win.h"

struct _KanbanAppWindow
{
  GtkApplicationWindow parent_instance;
};

G_DEFINE_TYPE(KanbanAppWindow, kanban_app_window, GTK_TYPE_APPLICATION_WINDOW);

static void
kanban_app_window_init (KanbanAppWindow *win)
{
  gtk_widget_init_template (GTK_WIDGET (win));
}

static void
kanban_app_window_class_init (KanbanAppWindowClass *class)
{
  gtk_widget_class_set_template_from_resource (GTK_WIDGET_CLASS (class),
                                               "/com/benwattsjones/kanban/window.ui");
}

KanbanAppWindow *
kanban_app_window_new (KanbanApp *app)
{
  return g_object_new (KANBAN_APP_WINDOW_TYPE, "application", app, NULL);
}

void
kanban_app_window_open (KanbanAppWindow *win,
                        GFile            *file)
{
}
