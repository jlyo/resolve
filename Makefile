PREFIX ?= /usr/local
BINDIR ?= $(PREFIX)/bin

INSTALL ?= install
CC ?= gcc
CFLAGS ?= -Wall -Wextra -pedantic -std=gnu90 -g

all: resolve

install: resolve
	$(INSTALL) -Dm755 resolve $(DESTDIR)$(BINDIR)/resolve

clean:
	-rm -f resolve *.o

.PHONY: all install clean

