forking: color_client scroll_client matrix_client memory_server color_server

color_server.o: color_server.c headers.h
	gcc -c color_server.c

color_server: color_server.o networking.o headers.h
	gcc -o color_server color_server.o networking.o

color_client.o: color_client.c headers.h
	gcc -c color_client.c

color_client: color_client.o term_col.o networking.o headers.h
	gcc -o color_client color_client.o term_col.o networking.o

memory_server.o: memory_server.c headers.h
	gcc -c memory_server.c

memory_server: memory_server.o networking.o headers.h
	gcc -o memory_server memory_server.o networking.o

matrix_client.o: matrix_client.c headers.h
	gcc -c matrix_client.c

matrix_client: matrix_client.o term_col.o networking.o headers.h
	gcc -o matrix_client matrix_client.o term_col.o networking.o

scroll_client.o: scroll_client.c headers.h
	gcc -c scroll_client.c

scroll_client: scroll_client.o scroll_text.o networking.o headers.h
	gcc -o scroll_client scroll_client.o scroll_text.o networking.o

term_col.o: term_col.c headers.h
	gcc -c term_col.c

networking.o: networking.c headers.h
	gcc -c networking.c

scroll_text.o: scroll_text.c headers.h
	gcc -c scroll_text.c

clean:
	rm *.o
	rm color_client
	rm scroll_client
	rm matrix_client
	rm color_server
	rm memory_server
