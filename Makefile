all: chi
chi: chi.c chi.h
	gcc -Wall -DCHI_MAIN -o chi chi.c 
test: chi.c test.c chi.h
	cd libtap; make
	gcc -static -Wall -DDEBUG -o test test.c chi.c -ltap -Ilibtap -Llibtap
clean:
	rm *.o test
