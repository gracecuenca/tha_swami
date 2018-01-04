all:
	gcc -o color term_col.c

run: all
	./color

gdb: all
	gdb control

clean:
	rm *~
	clear

cleanall:
	rm color
	rm *~
	clear
