forking: client fserver

fserver: forking_server.o networking.o headers.h
	gcc -o server forking_server.o networking.o

client: client.o term_col.o networking.o headers.h
	gcc -o client client.o term_col.o networking.o

client.o: client.c headers.h
	gcc -c client.c

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
	rm client
	rm server
