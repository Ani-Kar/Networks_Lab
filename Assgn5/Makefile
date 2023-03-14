libmsocket.a: mysocket.c
	gcc -c mysocket.c -o mysocket.o
	ar rcs lib_libmsocket.a mysocket.o

clean: 
	rm -f *.o *.a test client server

test: test.c libmsocket.a
	gcc -o test test.c -L. -l_libmsocket

server: server.c libmsocket.a
	gcc -o server server.c -L. -l_libmsocket

client: client.c libmsocket.a
	gcc -o client client.c -L. -l_libmsocket

