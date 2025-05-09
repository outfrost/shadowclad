# Copyright 2018-2020 Iwo 'Outfrost' Bujkiewicz
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

PLATFORM ?= x86_64-linux-gnu

BUILDDIR ?= target/$(PLATFORM)
SRCDIR ?= src

CPPFLAGS ::= -iquotesrc/ $(CPPFLAGS)
CFLAGS ::= -g -std=c99 -Wall -Wextra -Wpedantic -Werror \
           -Wno-error=unused-function -Wno-error=unused-parameter $(CFLAGS)
LDFLAGS ::= $(LDFLAGS)
LDLIBS ::= -lm -lGL -lGLEW -lglfw -lassimp $(LDLIBS)

# ######
# Paths
# ######

sources ::= main.c \
            engine/asset.c \
            engine/engine.c \
            engine/geometry.c \
            engine/input.c \
            engine/logger.c \
            engine/performance.c \
            engine/render.c \
            engine/scene.c \
            engine/string.c \
            engine/tga.c \
            engine/ui.c \
            game/game.c \
            game/input.c \
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
	@$(binary)
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
