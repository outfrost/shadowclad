CFLAGS ::= -g -std=c99 -Wall -Wextra -Wpedantic $(CFLAGS)
LDFLAGS ::= $(LDFLAGS)
LDLIBS ::= -L/usr/local/lib -lGL -lGLEW -lglut -lassimp $(LDLIBS)

# Prefix all object file names with the compilation directory
objects ::= $(addprefix out/, \
              main.o level.o logger.o \
              performance.o render.o tga.o ui.o)

# Set executable extension for the platform
ifeq ($(OS),Windows_NT)
	binext ::= .exe
else
	binext ::=
endif
binary ::= out/shadowclad$(binext)

# Default target: build executable
$(binary) : $(objects) | out
	@echo "###### Linking executable..."
	$(CC) $(LDFLAGS) -o $(binary) $(objects) $(LOADLIBES) $(LDLIBS)

# Alias for default target
shadowclad : $(binary)
.PHONY : shadowclad

# Build and run
run : $(binary)
	@echo
	LD_LIBRARY_PATH=/usr/local/lib $(binary)
.PHONY : run

# Create compilation directory
out :
	mkdir out/

# Alias for 'out'
init : out
.PHONY : init

# Generate dependencies
out/%.make : %.c Makefile | out
	$(CPP) -MM -MT out/$*.o -MF $@ $(CPPFLAGS) $<

# Include generated rules
-include $(addsuffix .make, $(basename $(objects)))

# Build compilation units
out/%.o : %.c out/%.make | out
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

# Delete build output
clean : out
	rm -f $(binary) out/*.o out/*.make
	rm -d out/
.PHONY : clean
