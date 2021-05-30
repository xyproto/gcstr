.PHONY: clean fmt

CC ?= gcc
CFLAGS ?= -std=c11 -O2 -pipe -fPIC -fstack-protector-strong -D_GNU_SOURCE -Wfatal-errors
STYLE ?= WebKit, ColumnLimit: 99
DESTDIR ?=
PREFIX ?= /usr

NAME := sleepycat
SRC := $(wildcard *.c)

$(NAME): $(SRC)
	$(CC) $(CFLAGS) -o $@ $(SRC) $(shell pkg-config --cflags --libs gcstr)

.clang-format:
	clang-format --style='{BasedOnStyle: $(STYLE)}' --dump-config > .clang-format

fmt: .clang-format
	clang-format -i $(SRC)

install: $(NAME)
	install -Dm755 $(NAME) "$(DESTDIR)$(PREFIX)/bin/$(NAME)"

clean:
	@rm -f *~ *.bak *.swp *.tmp *.o $(NAME)
