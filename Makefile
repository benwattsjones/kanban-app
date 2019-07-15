CC ?= gcc
PKGCONFIG = $(shell which pkg-config)
CFLAGS = $(shell $(PKGCONFIG) --cflags gtk+-3.0) -fexceptions -fpie -Wl,-pie -fstack-clash-protection -fstack-protector-strong -g -O3 -Wall -Wextra -Wpedantic -std=gnu11
LIBS = $(shell $(PKGCONFIG) --libs gtk+-3.0)
GLIB_COMPILE_RESOURCES = $(shell $(PKGCONFIG) --variable=glib_compile_resources gio-2.0)

SRC = src/presenters/kanban-application.c \
	  src/presenters/model-observer.c \
	  src/views/kanban-window.c \
	  src/models/model-presenter-interface.c \
	  src/main.c
BUILT_SRC = src/resources.c

OBJS = $(notdir $(BUILT_SRC:.c=.o) $(SRC:.c=.o))

all: kanban-app

src/resources.c: src/views/kanban-app.gresource.xml src/views/window.ui
	$(GLIB_COMPILE_RESOURCES) src/views/kanban-app.gresource.xml --target=$@ --sourcedir=src/views/ --generate-source

%.o: src/%.c 
	$(CC) -c -o $(@F) $(CFLAGS) $<

%.o: src/presenters/%.c
	$(CC) -c -o $(@F) $(CFLAGS) $<

%.o: src/views/%.c
	$(CC) -c -o $(@F) $(CFLAGS) $<

%.o: src/models/%.c
	$(CC) -c -o $(@F) $(CFLAGS) $<

%.o: src/storage/%.c
	$(CC) -c -o $(@F) $(CFLAGS) $<

kanban-app: $(OBJS)
	$(CC) -o $(@F) $(OBJS) $(LIBS)

clean:
	rm -f $(BUILT_SRC)
	rm -f $(OBJS)
	rm -f kanban-app

