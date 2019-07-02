CC ?= gcc
PKGCONFIG = $(shell which pkg-config)
CFLAGS = $(shell $(PKGCONFIG) --cflags gtk+-3.0)
LIBS = $(shell $(PKGCONFIG) --libs gtk+-3.0)
GLIB_COMPILE_RESOURCES = $(shell $(PKGCONFIG) --variable=glib_compile_resources gio-2.0)

SRC = src/exampleapp.c src/exampleappwin.c src/main.c
BUILT_SRC = src/resources.c

OBJS = $(notdir $(BUILT_SRC:.c=.o) $(SRC:.c=.o))

all: exampleapp.exe

src/resources.c: data/exampleapp.gresource.xml data/window.ui
	$(GLIB_COMPILE_RESOURCES) data/exampleapp.gresource.xml --target=$@ --sourcedir=data/ --generate-source

%.o: src/%.c
	$(CC) -c -o $(@F) $(CFLAGS) $<

exampleapp.exe: $(OBJS)
	$(CC) -o $(@F) $(OBJS) $(LIBS)

clean:
	rm -f $(BUILT_SRC)
	rm -f $(OBJS)
	rm -f exampleapp.exe
