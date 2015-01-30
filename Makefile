chi: chi.c
	gcc -Wall -DCHI_MAIN -o chi chi.c 
test: chi.c test.c
	gcc -Wall -DDEBUG -o test test.c chi.c -ltap -Ilibtap -Llibtap
clean:
	rm *.o test
