# Compiler settings
CC := gcc
CFLAGS := -Wall -Wextra -Werror
DEGUBFLAGS := -gdwarf-4 -g3

# Static analysis settings
LINT := cpplint
DOXYGEN := doxygen
DOXYFILE := Doxyfile

# Directories
SRC_DIR := src/
OBJ_DIR := obj/
OUT_DIR := bin/
RELEASE_DIR := release/
DEBUG_DIR := debug/

# Dependencies
PKGS :=
LIBS := $(if $(PKGS),$(shell pkg-config --cflags --libs $(PKGS)))

# Headers
HEDS := $(shell find $(SRC_DIR) -type f -name '*.h')
INCS := -Isrc

# Sources
SRCS := $(shell find $(SRC_DIR) -type f -name '*.c')

# Entry points (assumes standard format)
ENT_REGEX := '^\s*int\s+main\s*\x28'
ENTS := $(foreach \
	src,\
	$(SRCS), \
	$(if \
		$(shell cpp -P $(src) $(INCS) $(LIBS) | grep -lP $(ENT_REGEX)), \
		$(src) \
	) \
)

# Objects
OBJS_RELEASE := $(foreach \
	src, \
	$(SRCS), \
	$(patsubst \
		$(SRC_DIR)%.c, \
		$(OBJ_DIR)$(RELEASE_DIR)%.o, \
		$(src) \
	) \
)
OBJS_DEBUG := $(foreach \
	src, \
	$(SRCS), \
	$(patsubst \
		$(SRC_DIR)%.c, \
		$(OBJ_DIR)$(DEBUG_DIR)%.o, \
		$(src) \
	) \
)

# Executables produced
OUTS_RELEASE := $(foreach \
	ent, \
	$(ENTS), \
	$(patsubst \
		$(SRC_DIR)%.c, \
		$(OUT_DIR)$(RELEASE_DIR)%, \
		$(ent) \
	) \
)
OUTS_DEBUG := $(foreach \
	ent, \
	$(ENTS), \
	$(patsubst \
		$(SRC_DIR)%.c, \
		$(OUT_DIR)$(DEBUG_DIR)%, \
		$(ent) \
	) \
)

# Objects without entry point
OBJS_NONENTRY_RELEASE := $(filter-out \
	$(foreach out, \
		$(OUTS_RELEASE), \
		$(patsubst \
			$(OUT_DIR)$(RELEASE_DIR)%, \
			$(OBJ_DIR)$(RELEASE_DIR)%.o, \
			$(out) \
		) \
	), \
	$(OBJS_RELEASE) \
)
OBJS_NONENTRY_DEBUG := $(filter-out \
	$(foreach out, \
		$(OUTS_DEBUG), \
		$(patsubst \
			$(OUT_DIR)$(DEBUG_DIR)%, \
			$(OBJ_DIR)$(DEBUG_DIR)%.o, \
			$(out) \
		) \
	), \
	$(OBJS_DEBUG) \
)

# Function to create neccessary directories
define ensure-dir
	@if [ ! -d "$(dir $1)" ]; then \
		mkdir -p "$(dir $1)"; \
		printf "make: \033[1;34m$(dir $1)\033[0m\n"; \
	fi
endef

# Default build target
default: release

# Release build target
release: lint doxygen $(OUTS_RELEASE)

# Debug build target
debug: lint doxygen $(OUTS_DEBUG)

# Remove generated files
clean:
	@rm -rf $(OUTS_RELEASE) $(OBJS_RELEASE) $(OUTS_DEBUG) $(OBJS_DEBUG) $(OBJ_DIR) $(OUT_DIR) doxy

# Linting check
lint:
	@$(LINT) --quiet --filter=-legal/copyright $(SRCS) $(HEDS)

# Doxygen generation
doxygen:
	@$(DOXYGEN) $(DOXYFILE)

# Release - Link objects into executablea
$(OUT_DIR)$(RELEASE_DIR)%: $(OBJ_DIR)$(RELEASE_DIR)%.o $(OBJS_NONENTRY_RELEASE)
	$(call ensure-dir,$@)
	@printf "make: \033[1;32m$@\033[0m\n"
	@$(CC) -o $@ $< $(OBJS_NONENTRY_RELEASE) $(CFLAGS) $(INCS) $(LIBS)

# Release -Compile C source files into object files
$(OBJ_DIR)$(RELEASE_DIR)%.o: $(SRC_DIR)%.c $(HEDS)
	$(call ensure-dir,$@)
	@printf "make: $@\n"
	@$(CC) -c -o $@ $< $(CFLAGS) $(INCS) $(LIBS)

# Debug - Link objects into executablea
$(OUT_DIR)$(DEBUG_DIR)%: $(OBJ_DIR)$(DEBUG_DIR)%.o $(OBJS_NONENTRY_DEBUG)
	$(call ensure-dir,$@)
	@printf "make: \033[1;32m$@\033[0m\n"
	@$(CC) -o $@ $< $(OBJS_NONENTRY_DEBUG) $(CFLAGS) $(DEGUBFLAGS) $(INCS) $(LIBS)

# Debug - Compile C source files into object files
$(OBJ_DIR)$(DEBUG_DIR)%.o: $(SRC_DIR)%.c $(HEDS)
	$(call ensure-dir,$@)
	@printf "make: $@\n"
	@$(CC) -c -o $@ $< $(CFLAGS) $(DEGUBFLAGS) $(INCS) $(LIBS)

.PHONY: default release debug clean lint doxygen

.PRECIOUS: $(OBJS_RELEASE) $(OBJS_DEBUG)