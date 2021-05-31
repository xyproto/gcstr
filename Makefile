.PHONY: clean fmt lib

CC ?= gcc
CFLAGS ?= -std=c11 -O2 -pipe -fPIC -fstack-protector-strong -D_GNU_SOURCE -Wfatal-errors
STYLE ?= WebKit, ColumnLimit: 99
DESTDIR ?=
PREFIX ?= /usr

NAME := gcstr
SRC := $(wildcard *.c)
HEADERS := $(wildcard *.h)

main: $(SRC)
	$(CC) $(CFLAGS) -o $@ $(SRC) $(shell pkg-config --cflags --libs bdw-gc)

debug: $(SRC)
	$(CC) -std=c11 -D_GNU_SOURCE -Wfatal-errors -g -O1 -o $@ $(SRC) $(shell pkg-config --cflags --libs bdw-gc)

lib: lib$(NAME).so

lib$(NAME).so: $(SRC)
	$(CC) $(CFLAGS) -shared -fpic -o $@ $(SRC) $(shell pkg-config --cflags --libs bdw-gc)

.clang-format:
	clang-format --style='{BasedOnStyle: $(STYLE)}' --dump-config > .clang-format

fmt: .clang-format
	clang-format -i $(SRC) $(HEADERS)

install: lib$(NAME).so
	install -Dm644 $< "$(DESTDIR)$(PREFIX)/lib/$<"
	install -d "$(DESTDIR)$(PREFIX)/include/$(NAME)"
	for h in *.h; do install -Dm644 -t "$(DESTDIR)$(PREFIX)/include/$(NAME)" "$$h"; done
	install -Dm644 $(NAME).pc "$(DESTDIR)$(PREFIX)/lib/pkgconfig/$(NAME).pc"

clean:
	@rm -f *~ *.bak *.swp *.tmp *.o *.so debug main gcstr
