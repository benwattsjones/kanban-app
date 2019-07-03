#include <gtk/gtk.h>

#include "kanban-app.h"
#include "kanban-app-win.h"

struct _KanbanApp
{
  GtkApplication parent_instance;
};

G_DEFINE_TYPE(KanbanApp, kanban_app, GTK_TYPE_APPLICATION);

/* Registered with G_DEFINE_TYPE (specifically with _get_type call). 
   Called by g_object_new(). Used to initialize ojbect before construction
   properties are set.
*/
static void
kanban_app_init (KanbanApp *app)
{
}

static void
kanban_app_activate (GApplication *app)
{
  KanbanAppWindow *win;

  win = kanban_app_window_new (KANBAN_APP (app));
  gtk_window_present (GTK_WINDOW (win));
}

static void
kanban_app_open (GApplication  *app,
                  GFile        **files,
                  gint           n_files,
                  const gchar   *hint)
{
  GList *windows;
  KanbanAppWindow *win;
  int i;

  windows = gtk_application_get_windows (GTK_APPLICATION (app));
  if (windows)
    win = KANBAN_APP_WINDOW (windows->data);
  else
    win = kanban_app_window_new (KANBAN_APP (app));

  for (i = 0; i < n_files; i++)
    kanban_app_window_open (win, files[i]);

  gtk_window_present (GTK_WINDOW (win));
}

static void
kanban_app_class_init (KanbanAppClass *class)
{
  G_APPLICATION_CLASS (class)->activate = kanban_app_activate;
  G_APPLICATION_CLASS (class)->open = kanban_app_open;
}

KanbanApp *
kanban_app_new (void)
{
  return g_object_new (KANBAN_APP_TYPE,
                       "application-id", "com.benwattsjones.kanban",
                       "flags", G_APPLICATION_HANDLES_OPEN,
                       NULL);
}

