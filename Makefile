CFLAGS = -Wall -Wextra -pedantic -O2
FICHEIROS = cgi.h estado.c estado.h Roguelike.c Makefile Imagens/*

install: Roguelike
	sudo cp -r Imagens /var/www/html
	sudo cp Roguelike /usr/lib/cgi-bin
	sudo touch /var/www/html/estado
	sudo chmod 666 /var/www/html/estado
	touch install

unistall:
	sudo rm /usr/lib/cgi-bin/Roguelike
	sudo rm /var/www/html/estado
	sudo rm -r /var/www/html/Imagens

Roguelike: Roguelike.o estado.o
	cc -o Roguelike Roguelike.o estado.o

Roguelike.zip: $(FICHEIROS)
	zip -9 Roguelike.zip $(FICHEIROS)

doc:
	doxygen -g
	doxygen

clean:
	rm -rf *.o Roguelike Roguelike.zip Doxyfile Doxyfile.bak latex html install

Roguelike.o: Roguelike.c cgi.h estado.h

estado.o: estado.c estado.h
