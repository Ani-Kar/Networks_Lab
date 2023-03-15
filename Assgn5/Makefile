libmsocket.a: mysocket.c
	gcc -c mysocket.c -o mysocket.o
	ar rcs lib_libmsocket.a mysocket.o
	gcc -o server -g server.c -L. -l_libmsocket
	gcc -o client -g client.c -L. -l_libmsocket

clean: 
	rm -f *.o *.a test client server

test: test.c libmsocket.a
	gcc -o test test.c -L. -l_libmsocket

server: server.c libmsocket.a
	

client: client.c libmsocket.a
	

