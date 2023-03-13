libmsocket.a: mysocket.c
	gcc -c mysocket.c -o mysocket.o
	ar rcs lib_libmsocket.a mysocket.o

clean: 
	rm -f *.o *.a test

test:
	gcc -o test test.c -L. -l_libmsocket

