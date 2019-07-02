CC ?= gcc
PKGCONFIG = $(shell which pkg-config)
CFLAGS = $(shell $(PKGCONFIG) --cflags gtk+-3.0)
LIBS = $(shell $(PKGCONFIG) --libs gtk+-3.0)
GLIB_COMPILE_RESOURCES = $(shell $(PKGCONFIG) --variable=glib_compile_resources gio-2.0)

SRC = src/kanban-app.c src/kanban-app-win.c src/main.c
BUILT_SRC = src/resources.c

OBJS = $(notdir $(BUILT_SRC:.c=.o) $(SRC:.c=.o))

all: kanban-app

src/resources.c: data/kanban-app.gresource.xml data/window.ui
	$(GLIB_COMPILE_RESOURCES) data/kanban-app.gresource.xml --target=$@ --sourcedir=data/ --generate-source

%.o: src/%.c
	$(CC) -c -o $(@F) $(CFLAGS) $<

kanban-app: $(OBJS)
	$(CC) -o $(@F) $(OBJS) $(LIBS)

clean:
	rm -f $(BUILT_SRC)
	rm -f $(OBJS)
	rm -f kanban-app
