compileargs ::= -g -Wall -Wextra -Wpedantic
linkargs ::=
libraries ::= -L/usr/local/lib -lGL -lglut -lassimp
# Prefix all object file names with the compilation directory
objects ::= $(addprefix out/, \
              main.o debugutil.o glut_janitor.o render.o \
              tga.o level.o performance.o)

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
	$(CC) $(linkargs) -o $(binary) $(objects) $(libraries)

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

# Build each compilation unit

out/main.o : main.c debugutil.h glut_janitor.h render.h level.h | out
	$(CC) $(compileargs) -c -o out/main.o main.c

out/debugutil.o : debugutil.c | out
	$(CC) $(compileargs) -c -o out/debugutil.o debugutil.c

out/glut_janitor.o : glut_janitor.c | out
	$(CC) $(compileargs) -c -o out/glut_janitor.o glut_janitor.c

out/render.o : render.c render.h typedefs.h | out
	$(CC) $(compileargs) -c -o out/render.o render.c

out/tga.o : tga.c tga.h | out
	$(CC) $(compileargs) -c -o out/tga.o tga.c

out/level.o : level.c level.h tga.h | out
	$(CC) $(compileargs) -c -o out/level.o level.c

out/performance.o : performance.c | out
	$(CC) $(compileargs) -c -o out/performance.o performance.c
