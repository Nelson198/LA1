CFLAGS = -Wall -Wextra -pedantic -O2
FICHEIROS = cgi.h estado.c estado.h RogueLike.c Makefile

install: RogueLike
	sudo cp RogueLike cgi-bin
	sudo mkdir -p ficheiros
	sudo chmod a+rw *
	touch install

RogueLike: RogueLike.o estado.o
	cc -o RogueLike RogueLike.o estado.o

RogueLike.o: RogueLike.c cgi.h estado.h

exemplo.zip: $(FICHEIROS)
	zip -9 RogueLike.zip $(FICHEIROS)

estado.o: estado.c estado.h

doc:
	doxygen -g
	doxygen

clean:
	rm -rf *.o RogueLike Doxyfile latex html install