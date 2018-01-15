forking: client server

server: forking_server.o networking.o headers.h
	gcc -o server forking_server.o networking.o

color_client: color_client.o term_col.o networking.o headers.h
	gcc -o color_client color_client.o term_col.o networking.o

scroll_client: scroll_client.o scroll_text.o networking.o headers.h
	gcc -o scroll_client scroll_client.o scroll_text.o networking.o

color_client.o: color_client.c headers.h
	gcc -c color_client.c

scroll_client.o: scroll_client.c headers.h
	gcc -c scroll_client.c

term_col.o: term_col.c headers.h
	gcc -c term_col.c

forking_server.o: forking_server.c headers.h
	gcc -c forking_server.c

networking.o: networking.c headers.h
	gcc -c networking.c

scroll_text.o: scroll_text.c headers.h
	gcc -c scroll_text.c

clean:
	rm *.o
	rm color_client
	rm scroll_client
	rm server
