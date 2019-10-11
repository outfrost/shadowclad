PLATFORM ?= x86_64-linux-gnu

BUILDDIR ?= target/$(PLATFORM)
SRCDIR ?= src

CFLAGS ::= -g -std=c99 -Wall -Wextra -Wpedantic $(CFLAGS)
LDFLAGS ::= $(LDFLAGS)
LDLIBS ::= -L/usr/local/lib -lGL -lGLEW -lglut -lassimp $(LDLIBS)

# ######
# Paths
# ######

vpath %.c $(SRCDIR)
vpath %.h $(SRCDIR)

sources ::= main.c \
            asset.c \
            level.c \
            logger.c \
            performance.c \
            player.c \
            render.c \
            tga.c \
            ui.c

objects ::= $(addprefix $(BUILDDIR)/, $(addsuffix .o, $(sources)))
depfiles ::= $(addprefix $(BUILDDIR)/, $(addsuffix .mk, $(sources)))

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
$(binary): $(objects) | $(BUILDDIR)
	@echo "###### Linking executable..."
	$(CC) $(LDFLAGS) -o $(binary) $^ $(LOADLIBES) $(LDLIBS)

# Build C translation units
$(BUILDDIR)/%.c.o: %.c $(BUILDDIR)/%.c.mk | $(BUILDDIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

# ######
# Setup
# ######

# Create build directory
$(BUILDDIR):
	mkdir -p $(BUILDDIR)

# ######
# Aliases
# ######

# Build and run
run: $(binary)
	@echo
	LD_LIBRARY_PATH=/usr/local/lib $(binary)
.PHONY: run

# Initialise build environment
init: $(BUILDDIR)
.PHONY: init

# Build executable
shadowclad: $(binary)
.PHONY: shadowclad

# ######
# Prerequisites
# ######

# Generate C prerequisite makefiles
$(BUILDDIR)/%.c.mk: %.c Makefile | $(BUILDDIR)
	@echo "Generating prerequisites for $<"
	@$(CPP) -MM -MT $(BUILDDIR)/$*.c.o -MF $@ $(CPPFLAGS) $<
	@sed -E -i 's|^([^\s:]+)([ :])|\1 $@\2|' $@

# Include generated C prerequisites
include $(foreach depfile, $(depfiles), $(shell [ -r "$(depfile)" ] && echo "$(depfile)"))

# Do not automatically delete generated prerequisites
.SECONDARY: $(depfiles)

# ######
# Cleanup rules
# ######

clean:
	rm -rf $(BUILDDIR)
.PHONY: clean
