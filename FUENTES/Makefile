# fichero Makefile
#
# nombre del fichero ejecutable
#
TARGETS= ../BIN/practica2

#g++ LUtil.cpp main.cpp -I/usr/local/include -I/opt/X11/include -L/usr/local/lib -I/opt/X11/lib -w -framework OpenGL -lGLUT -o 01_hello_freeglut

# banderas para el compilador (C y C++). Se indican los directorios donde se puenden
# encontrar ficheros para incluir mediante la opcion -I
# la opcion -g sirve para permitir la depuración
#
CFLAGS=    -g -I/usr/include -I.
CXXFLAGS=  -O2 -w -g -I/usr/include -I. -std=c++14



# definicion del compilador
#
CC = g++
C11 = -std=C++14

# definicion de objetos
#
OBJECTS= practica2.o algoritmos.o
# orden que sera ejecutada por defecto
#
default : $(TARGETS)

# regla de ejecucion
#
$(TARGETS) : $(OBJECTS)
	$(CC) -o $@ $(CXXFLAGS) $(OBJECTS)

#
#
clean:
	rm -f *.o
	rm -f $(TARGETS)

#
#
redo:
	touch *.cc
	make

#
#
superclean:
	rm -f *.o
	rm *~
	rm -f $(TARGETS)

#
#
tgz:
	rm -f *.o
	rm *~
	rm -f $(TARGETS)
	tar -zcvf $(TARGETS).tgz *
