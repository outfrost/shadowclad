compileargs = -Wall -Wextra -Wpedantic
linkargs = -lGL -lglut
objects = out/main.o out/debugutil.o out/glut_janitor.o out/render.o \
          out/tga.o out/level.o

shadowclad : $(objects)
	gcc -o out/shadowclad $(objects) $(linkargs)

run : shadowclad
	out/shadowclad

out/main.o : main.c debugutil.h glut_janitor.h render.h
	gcc -c -o out/main.o main.c $(compileargs)

out/debugutil.o : debugutil.c
	gcc -c -o out/debugutil.o debugutil.c $(compileargs)

out/glut_janitor.o : glut_janitor.c
	gcc -c -o out/glut_janitor.o glut_janitor.c $(compileargs)

out/render.o : render.c render.h typedefs.h
	gcc -c -o out/render.o render.c $(compileargs)

out/tga.o : tga.c tga.h
	gcc -c -o out/tga.o tga.c $(compileargs)

out/level.o : level.c level.h tga.h
	gcc -c -o out/level.o level.c $(compileargs)
