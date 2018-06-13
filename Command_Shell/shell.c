#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include "shell_programs.h"

//INPUT_BYTES size received from professor's parser.c code
//#define INPUT_BYTES 50 
#define INPUT_BYTES 100

//function to install handler for signal from professor's sigchld.c
typedef void(*sighandler_t)(int);

sighandler_t signal(int signo, sighandler_t handler){
	struct sigaction act, oact;

	act.sa_handler = handler;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;

	if(signo == SIGALRM){
#ifdef SA_INTERRUPT
		act.sa_flags |= SA_INTERRUPT; /*SunOS 4.x*/
#endif
	}
	else{
#ifdef SA_RESTART
		act.sa_flags |= SA_RESTART; /* SVR4, 4.4BSD */
#endif
	}

	if(sigaction(signo, &act, &oact) < 0){
		return(SIG_ERR);
	}

	return(oact.sa_handler);
}

//cmd_pieces size received from professor's parser.c
char* cmd_pieces[20];
int length;

//SIGCHLD Handler
void int_handler(int sig){
	pid_t pid;
	int stat;
	
	pid = waitpid(WAIT_ANY, &stat, WNOHANG);
	completeJob(pid);
}

//parser code recieved from professor's parser.c example
void parser(char* shell_input){
	char* word;
	int i = 0;
	if(shell_input != NULL){
		while((word = strsep(&shell_input, " ")) != NULL){	
		       	cmd_pieces[i] = word;
			i++;
		}
		//Strtok() added by me to remove \n from last argument (added on by fgets automatically)
		strtok(cmd_pieces[i-1], "\n");
		length = i;
		cmd_pieces[i] = NULL;
	}
}

void cs350shell(){
	int status;
        pid_t pid;
	char buffer[INPUT_BYTES];
	char* shell_input = buffer;
	signal(SIGCHLD, int_handler);
	while(1){
		//shell_input size received from professor's parser.c code
		//shell_input = malloc(INPUT_BYTES * sizeof(int));
		printf("cs350sh>");
		fgets(shell_input,INPUT_BYTES, stdin);
		parser(shell_input);
		if(strcmp(cmd_pieces[0], "fg") == 0){
		        char* ptr;
			long test;
			pid_t validId;
			if(cmd_pieces[1] != NULL){
				//strtol converts string to number which we use to find the process to bring to foreground
				test = strtol(cmd_pieces[1], &ptr, 10);
			}
		        if(test == 0){
				fprintf(stderr, "INVALID CMD: no process id given\n");
			}
			else{
				validId = findJob(test);
				if(validId == 0){
					fprintf(stderr, "ERROR: No process with given ID currently running\n");
				}
				else{
					waitpid(test, &status, 0);
					completeJob(pid);
				}
			}
		}
		else if(strcmp(cmd_pieces[0], "listjobs") == 0){
			printJobs();
		}
		else{
			if((pid = fork()) < 0){
				fprintf(stderr, "fork failed\n");
				exit(1);
			}
			if(pid == 0){

				/* 
				   ADD COMMAND CALLING FUNCTIONALITY HERE
				*/

				functCall(cmd_pieces, length);
				exit(0);
			}
			else{
				if(length == 1 || strcmp(cmd_pieces[1], "&") != 0){
					waitpid(pid, &status, 0);
				}
				else{
					addJob(pid);
				}
			}
		}
		//free(shell_input);
	}
}

int main(){
	cs350shell();
	return(0);
}
