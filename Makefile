all: simple-shell

simple-shell : simple-shell.c
	gcc -o simple-shell simple-shell.c

clean :
	rm -f simple-shell