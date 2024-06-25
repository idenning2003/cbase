SHELL := /bin/bash
CC := gcc
CFLAGS := -Wall -Wextra -Werror -gdwarf-4 -g3
LIBS := -lm

SDIR := src/
ODIR := obj/
EDIR := bin/
TDIR := tests/
EXEC := main
TEST := test
TAR := cbase.tar.gz

DEPS := $(shell find $(SDIR) -type f -name '*.h')
SRCS := $(shell find $(SDIR) -type f -name '*.c')
OBJS := $(foreach src,$(SRCS),$(patsubst $(SDIR)%.c,%.o,$(src)))

BUILD-OBJS := $(foreach obj,$(OBJS),$(if $(findstring /test/,$(obj)),,$(ODIR)$(obj)))

TEST-REGEX := '^__attribute__\(\(test\)\)(\s+)(\w+)(\s+)(\w+)\(((\w|\s|,|\*|\[|\])*)\)(\s*)'
TEST-SRCS := $(foreach src,$(SRCS),$(if $(findstring /test/,$(src)),$(src),))
TEST-OBJS := $(foreach src,$(TEST-SRCS),$(patsubst $(SDIR)%.c,$(ODIR)%.o,$(src)))
TEST-EXEC := $(foreach src,$(shell grep -lE $(TEST-REGEX) $(TEST-SRCS)),$(patsubst %.c,$(EDIR)$(TDIR)%,$(notdir $(src))))

all: build test

build: $(EDIR)$(EXEC)

test: $(EDIR)$(TEST) $(TEST-EXEC)

tar: $(TAR)

clean:
	@rm -rf $(ODIR) $(EDIR)
	@rm -f $(TAR)

$(EDIR)$(EXEC): $(filter-out %/test.o,$(BUILD-OBJS))
	@if [ ! -d "$(dir $@)" ]; then \
		mkdir -p $(dir $@); \
		printf "make: \033[1;34m$(dir $@)\033[0m\n"; \
	fi
	@printf "make: \033[1;32m$@\033[0m\n"
	@$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

$(EDIR)$(TEST): $(filter-out %/main.o,$(BUILD-OBJS))
	@if [ ! -d "$(dir $@)" ]; then \
		mkdir -p $(dir $@); \
		printf "make: \033[1;34m$(dir $@)\033[0m\n"; \
	fi
	@printf "make: \033[1;32m$@\033[0m\n"
	@$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

$(EDIR)$(TDIR)%: $(filter-out %/main.o %/test.o,$(BUILD-OBJS)) $(TEST-OBJS)
	@if [ ! -d "$(dir $@)" ]; then \
		mkdir -p $(dir $@); \
		printf "make: \033[1;34m$(dir $@)\033[0m\n"; \
	fi
	@printf "make: \033[1;32m$@\033[0m\n"
	@$(CC) -o $@ \
		$(filter \
			%/$(notdir $@).o,$(TEST-OBJS) \
		) \
		$(filter-out \
			$(foreach src,$(shell grep -lE $(TEST-REGEX) $(TEST-SRCS)),$(patsubst $(SDIR)%.c,$(ODIR)%.o,$(src))), \
			$(TEST-OBJS) \
		) \
		$(foreach obj,$^,$(if $(findstring /test/,$(obj)),,$(obj)) \
	) $(CFLAGS) $(LIBS)

$(ODIR)%.o: $(SDIR)%.c $(DEPS)
	@if [ ! -d "$(dir $@)" ]; then \
		mkdir -p $(dir $@); \
		printf "make: \033[1;34m$(dir $@)\033[0m\n"; \
	fi
	@printf "make: $@\n"
	@if grep -qE $(TEST-REGEX) $<; then \
		printf "# 1 \"$<\"\n" | \
		cat - $< | \
		cat - <(printf "\n\nint main(__attribute__((unused)) int argc, __attribute__((unused)) char* argv[]) {\n  int i = 0, err = 0;\n") | \
		cat - <(grep -onE $(TEST-REGEX) $< | awk -F[\(\)' ':] '{print "  printf(\"%3d RUNNING: %s:%-4d \\x1B[36m" $$8 "()\\x1B[0m\\n\", ++i, __FILE__, " $$1 ");\n  err |= " $$8 "(argc, argv);"}') | \
		cat - <(printf "  return err;\n}\n") | \
		sed -e 's/__attribute__((test)) //g' | \
		$(CC) -c -o $@ -xc - $(CFLAGS) $$(find src/ -type d | sed -e 's/^/-I/'); \
	else \
		$(CC) -c -o $@ $< $(CFLAGS) $$(find src/ -type d | sed -e 's/^/-I/'); \
	fi

$(TAR): $(shell find -type f \! \( -name $(TAR) -o -wholename ./$(ODIR)\* -o -wholename ./$(EDIR)\* \))
	@printf "make: \033[1;31m$@\033[0m\n"
	@tar -czvf $(TAR) --transform 's,^,cbase/,' $(shell find -type f \! \( -name $(TAR) -o -wholename ./$(ODIR)\* -o -wholename ./$(EDIR)\* \)) > /dev/null

.PHONY: all build test tar clean

.PRECIOUS: $(TEST-OBJS) $(BUILD-OBJS)
