forking: color_client color_server scroll_client scroll_server matrix_client memory_server

color_server.o: color_server.c headers.h
	gcc -c color_server.c

color_server: color_server.o networking.o headers.h
	gcc -o color_server color_server.o networking.o

color_client.o: color_client.c headers.h
	gcc -c color_client.c

color_client: color_client.o color_func.o networking.o headers.h
	gcc -o color_client color_client.o color_func.o networking.o

scroll_server.o: scroll_server.c headers.h
	gcc -c scroll_server.c

scroll_server: scroll_server.o networking.o headers.h
	gcc -o scroll_server scroll_server.o networking.o

scroll_client.o: scroll_client.c headers.h
	gcc -c scroll_client.c

scroll_client: scroll_client.o scroll_func.o networking.o headers.h
	gcc -o scroll_client scroll_client.o scroll_func.o networking.o

memory_server.o: memory_server.c headers.h
	gcc -c memory_server.c

memory_server: memory_server.o networking.o headers.h
	gcc -o memory_server memory_server.o networking.o

matrix_client.o: matrix_client.c headers.h
	gcc -c matrix_client.c

matrix_client: matrix_client.o color_func.o networking.o headers.h
	gcc -o matrix_client matrix_client.o color_func.o networking.o

color_func.o: color_func.c headers.h
	gcc -c color_func.c

networking.o: networking.c headers.h
	gcc -c networking.c

scroll_func.o: scroll_func.c headers.h
	gcc -c scroll_func.c

clean:
	rm *.o
	rm color_client
	rm color_server
	rm scroll_client
	rm scroll_server
	rm matrix_client
	rm memory_server
