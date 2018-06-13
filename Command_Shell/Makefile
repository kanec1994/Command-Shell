all: shell
shell: shell.o shell_programs.o
	gcc -o shell shell.o shell_programs.o
shell.o: shell.c shell_programs.h
	gcc -o shell.o -c shell.c
shell_programs.o: shell_programs.c
	gcc -o shell_programs.o -c shell_programs.c
clean:
	rm -f shell *.o *~

