all: chi
chi: chi.c chi.h
	gcc -Wall -DCHI_MAIN -o chi chi.c 
test: chi.c test.c chi.h
        cd libtap
        make
        cd ..
	gcc -Wall -DDEBUG -o test test.c chi.c -ltap -Ilibtap -Llibtap
	test
clean:
	rm *.o test
