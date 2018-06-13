all: cs350sh
cs350sh: shell.o shell_programs.o
	gcc -o cs350sh shell.o shell_programs.o
shell.o: shell.c shell_programs.h
	gcc -o shell.o -c shell.c
shell_programs.o: shell_programs.c
	gcc -o shell_programs.o -c shell_programs.c
clean:
	rm -f cs350sh *.o *~

