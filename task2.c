
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    FILE *fp;
    int i;
    char buffer[255];
    char command[256];
    command[0]='\0';
    pid_t pid;

    int pipefd[2];


    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    for (int i = 1; i < argc; i++) {
        strcat(command, argv[i]);
        strcat(command, " ");
    }

    pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {

        
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO); 
        close(pipefd[1]); 

        execlp("/bin/sh", "sh", "-c", command, (char *) NULL);
        perror("execlp"); 
        exit(EXIT_FAILURE);


    } else {

        close(pipefd[1]);
        while (read(pipefd[0], buffer, sizeof(buffer) - 1) != 0) {
            printf("%s", buffer);
        }
        close(pipefd[0]);
        wait(NULL);

    }




}