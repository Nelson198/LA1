CFLAGS = -Wall -Wextra -pedantic -O2
FICHEIROS = cgi.h estado.c estado.h RogueLike.c Makefile Imagens/*

install: RogueLike
	sudo cp -r Imagens /var/www/html
	sudo cp RogueLike /usr/lib/cgi-bin
	sudo touch /var/www/html/estado
	sudo chmod 666 /var/www/html/estado
	touch install

RogueLike: RogueLike.o estado.o
	cc -o RogueLike RogueLike.o estado.o

exemplo.zip: $(FICHEIROS)
	zip -9 RogueLike.zip $(FICHEIROS)

doc:
	doxygen -g
	doxygen

clean:
	rm -rf *.o RogueLike Doxyfile Doxyfile.bak latex html install

RogueLike.o: RogueLike.c cgi.h estado.h

estado.o: estado.c estado.h