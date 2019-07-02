#ifndef __EXAMPLEAPP_H
#define __EXAMPLEAPP_H

#include <gtk/gtk.h>

/* We are creating a new GObject, ExampleApp, which is a sub-class of 
   GtkApplication. 
   By convention, there should be an identifiable type (e.g. 'App') and namespace
   (e.g. 'Example').
   We are defining a 'final' type, as opposed to a 'derivable' type, which
   means it cannot be subclassed further.
*/

/* Macro defined for convenience to cast to type ExampleApp.
   Note example_app_get_type() function is defined by G_DEFINE_TYPE in exampleapp.c.
   NOTE: macros all cast to pointer of types not types themselves??
*/
#define EXAMPLE_APP_TYPE (example_app_get_type ())
/* This convenience macro does the following:
    - defines ExampleApp type as a typedef of struct _ExampleApp. This struct
      must be defined in the .c file before G_DEFINE_TYPE is used.
    - defines ExampleAppClass as a struct containing GtkApplicationClass
    - adds g_autoptr() support for ExampleApp type, based on parent class type
   Parameters:
    - Name of new type (in CamelCase)
    - name of new type (in snake_case)
    - name of the module (in ALL_CAPS)
    - name of the module (in ALL_CAPS)
    - name of the parent type (in CamelCase)
*/
G_DECLARE_FINAL_TYPE (ExampleApp, example_app, EXAMPLE, APP, GtkApplication)


ExampleApp     *example_app_new         (void);


#endif /* __EXAMPLEAPP_H */
