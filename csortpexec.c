
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <wait.h>
#include <stdlib.h>
#include <string.h>

int main(int argv, char* argc[]){
    int status = 1, i;
    char indice;
    pid_t childPid;
    char** command = malloc(sizeof(char*)*(argv+10));
    char* aux;
    aux = malloc(10*sizeof(*aux));
    strcpy(aux, "sort");
    command[0] = aux;
    aux = malloc(10*sizeof(*aux));
    strcpy(aux, "-m");
    command[1] = aux;
    aux = malloc(10*sizeof(*aux));
    strcpy(aux, "-k4");
    command[2] = aux;
    aux = malloc(10*sizeof(*aux));
    strcpy(aux, "-n");
    command[3] = aux;
    aux = malloc(20*sizeof(*aux));
    strcpy(aux, "-k5,6");
    command[4] = aux;
    for(i = 1; i < argv-1; i++){
		indice = i+'0';
		aux = malloc(10*sizeof(*aux));
		strcpy(aux,"tmp");
		strcat(aux, &indice);
 		command[i+4] = aux;
	if((childPid = fork()) < 0){
		perror("fork: ");
		exit(1);
	}
        if(childPid == 0){
		if(execlp("sort", "sort", "-k", "4","-n", "-k5,6", argc[i], "-o", aux, (char*)NULL) == -1){
			perror("Child sort error: ");
			exit(1);
		}
		exit(0);
	}
    }

    for(i = 1; i < argv-1; i++){ wait(&status);}
    aux = malloc(10*sizeof(*aux));
    strcpy(aux, "-o");	
    command[argv+3] = aux;
    command[argv+4] = malloc(sizeof(char)*100);
    strcpy(command[argv+4], argc[argv-1]);
    command[argv+5] = (char*)NULL;

    if(execvp("sort", command ) == -1){
			perror("Parent sort error: ");
			exit(1);
		}
    exit(0);
}
