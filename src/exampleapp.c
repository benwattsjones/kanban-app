#include <gtk/gtk.h>

#include "exampleapp.h"
#include "exampleappwin.h"

/* This struct is typedef'd to ExampleApp type by G_DECLARE_FINAL_TYPE in
   header. It is the instance structure of our type. This must start with
   a copy of the base class to enable it to be accessed as follows (ish):
     ExampleApp app;
     GtkApplication *base_class_instance = (GtkApplication *)app;
*/
struct _ExampleApp
{
  GtkApplication parent_instance;
};

/* G_DEFINE_TYPE macro is a convenience macro for type implementations.
   Its parameters are:
    - The name of the new type in CamalCase
    - The name of the new type in snake_case
    - the GType of the parent type
  
  In this case, the macro will expand to forward declare the following:
    static void example_app_init(ExampleApp *self);
    static void example_app_class_init(ExampleAppClass *klass);
  As well as setting variable
    static gpointer example_app_parent_class
  pointing to the new types parent class. it will also define the function:
    GType example_app_get_type()
  Calling this function (done via macro in header) registers the init functions
  with g_type_register_static_simple() call
*/
G_DEFINE_TYPE(ExampleApp, example_app, GTK_TYPE_APPLICATION);

/* Registered with G_DEFINE_TYPE (specifically with _get_type call). 
   Called by g_object_new(). Used to initialize ojbect before construction
   properties are set.
*/
static void
example_app_init (ExampleApp *app)
{
}

static void
example_app_activate (GApplication *app)
{
  ExampleAppWindow *win;

  win = example_app_window_new (EXAMPLE_APP (app));
  gtk_window_present (GTK_WINDOW (win));
}

static void
example_app_open (GApplication  *app,
                  GFile        **files,
                  gint           n_files,
                  const gchar   *hint)
{
  GList *windows;
  ExampleAppWindow *win;
  int i;

  windows = gtk_application_get_windows (GTK_APPLICATION (app));
  if (windows)
    win = EXAMPLE_APP_WINDOW (windows->data);
  else
    win = example_app_window_new (EXAMPLE_APP (app));

  for (i = 0; i < n_files; i++)
    example_app_window_open (win, files[i]);

  gtk_window_present (GTK_WINDOW (win));
}

/* Registered via G_DEFINE_TYPE; called via g_object_new().
   Used to initialize/override class methods.
   EXTRA: No constructed vfuncs are being altered here, but if the contructed
   function of the defined type is overrided, it must be chained to a
   call to the parent type's constructed function. 
   (See 'The GObject base class')
*/
static void
example_app_class_init (ExampleAppClass *class)
{
  G_APPLICATION_CLASS (class)->activate = example_app_activate;
  G_APPLICATION_CLASS (class)->open = example_app_open;
}

/* Called in main.c */
ExampleApp *
example_app_new (void)
{
  return g_object_new (EXAMPLE_APP_TYPE,
                       "application-id", "org.gtk.exampleapp.app2",
                       "flags", G_APPLICATION_HANDLES_OPEN,
                       NULL);
}
