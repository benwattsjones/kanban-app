#include <gtk/gtk.h>

#include "exampleapp.h"

/* "Building Applications"
   A GTK application consists of a number of files:

   The binary     - installed in /usr/bin. 
                    Typically put in builddir/ in source tree if using meson.

   A desktop file - installed in /usr/share/applications.
                    Typically in data/<app_id>.desktop.in in source tree.
                      '.in' file as before formatting (including language)
                      validated with BASH: desktop-file-validate

   An icon        - Installed in /usr/share/icons/hicolor/48x48/apps.
                      NOTE: Typically PNG images (may ALSO be an SVG image set to size).
                            There are other size dirs also.
                            There is also a scalable/apps dir where the original,
                            un-minified SVG may be placed.
                            A symbolic icon (small, monochrome, works at 16x16), 
                            may be placed at symbolic/apps/<app-id>-symbolic.svg if required.
                            Icons in hicolor/ will be viewable regardless of theme,
                            other dirs at this level for specific themes exist.
                    Typically placed in data/ dir in source tree.
                      data/icons/hicolor/scalable/apps/<app-id>.svg (size set to 128x128)
                      data/icons/hicolor/symbolic/apps/<app-id>-symbolic.svg 
                        (size set to 16x16; only if required)
                      PNGs in specific sizes may also exist, or be created in build process

  settings schema - Installed in /usr/share/glib-2.0/schemas
                      Only if application uses GSettings
                    Typically in data/<app-id>.gschema.xml in source tree

  App data        - Installed in /usr/share/appdata/<app-id>.appdata.xml
                    Typically in data/<app-id>.appdata.xml.in in source tree
                      Validated with BASH: appstream-util
                      May also have a screenshots/ dir.
                      Used for displaying info for GNOME software.
                      The downstream distro should generate the AppStream
                      metadata, but in liu of this GNOME Software can work
                      to a limited state with installed AppData files


  You may also see the following in source trees:

  souce code - src/

  UI and CSS - typicall src/ but sometimes data/ - compiled into program with 
               GResource so not installed

  Internationalization - po/

  DOAP       - "Description Of A Project" (https://github.com/ewilderj/doap)
               files give metadata on e.g. maintaines, in XML format.
               <project-name>.doap

  JSON Manifest - <app-id>.json is used by Flatpack.

  docs/ / doc/- consider doxygen, sphinx, breathe, readthedocs
  COPYING / LICENCE
  README
  CONTRIBUTING, HACKING, coc
  AUTHORS
  CHANGELOG / NEWS
*/

int
main (int argc, char *argv[])
{
  /* example_app_new() is fulilling the roles of both gtk_application_new()
     and g_signal_connect() for "activate" signal. This comes from exampleapp.h,
     and makes an ExampleApp object, inheriting GtkApplication. */
  return g_application_run (G_APPLICATION (example_app_new ()), argc, argv);
}
