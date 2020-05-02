PLATFORM ?= x86_64-linux-gnu

BUILDDIR ?= target/$(PLATFORM)
SRCDIR ?= src

CPPFLAGS ::= -iquotesrc/ $(CPPFLAGS)
CFLAGS ::= -g -std=c99 -Wall -Wextra -Wpedantic -Werror $(CFLAGS)
LDFLAGS ::= $(LDFLAGS)
LDLIBS ::= -L/usr/local/lib -lGL -lGLEW -lglut -lassimp $(LDLIBS)

# ######
# Paths
# ######

sources ::= main.c \
            engine/asset.c \
            engine/logger.c \
            engine/performance.c \
            engine/render.c \
            engine/tga.c \
            engine/ui.c \
            game/level.c \
            game/player.c

srcfiles ::= $(addprefix $(SRCDIR)/, $(sources))

objects ::= $(addprefix $(BUILDDIR)/, $(addsuffix .o, $(srcfiles)))
depfiles ::= $(addprefix $(BUILDDIR)/, $(addsuffix .mk, $(srcfiles)))

# Set executable name for the platform
# TODO Base this on target platform instead of host OS
#ifeq ($(OS),Windows_NT)
#	binext ::= .exe
#else
#	binext ::=
#endif
binary ::= $(BUILDDIR)/shadowclad #$(binext)

# ######
# Main build rules
# ######

# Default rule: build executable
$(binary): $(objects)
	@mkdir -p $(@D)
	@echo "Linking executable"
	@$(CC) $(LDFLAGS) -o $(binary) $^ $(LOADLIBES) $(LDLIBS)

# Build C translation units
$(objects): $(BUILDDIR)/%.c.o: %.c $(BUILDDIR)/%.c.mk
	@mkdir -p $(@D)
	@echo "Building $@"
	@$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

# ######
# Setup
# ######

# Initialise build environment
init:
	@echo "Creating build directory $(BUILDDIR)"
	@mkdir -p $(BUILDDIR)
.PHONY: init

# ######
# Aliases
# ######

# Build and run
run: $(binary)
	@echo
	@LD_LIBRARY_PATH=/usr/local/lib $(binary)
.PHONY: run

# Build executable
shadowclad: $(binary)
.PHONY: shadowclad

# ######
# Prerequisites
# ######

# Generate C prerequisite makefiles
$(depfiles): $(BUILDDIR)/%.c.mk: %.c Makefile
	@mkdir -p $(@D)
	@echo "Generating prerequisites for $<"
	@$(CPP) -MM -MT $(BUILDDIR)/$*.c.o -MF $@ $(CPPFLAGS) $<
# Give the same prerequisites to the prerequisite makefile,
# so that it is regenerated whenever any of said prerequisites change
	@sed -E -i 's|^([^\s:]+)([ :])|\1 $@\2|' $@

# Include generated C prerequisites
include $(foreach depfile, $(depfiles), $(shell [ -r "$(depfile)" ] && echo "$(depfile)"))

# Do not automatically delete generated prerequisites
.SECONDARY: $(depfiles)

# ######
# Cleanup rules
# ######

clean:
	@echo "Removing $(BUILDDIR)"
	@rm -rf $(BUILDDIR)
.PHONY: clean
