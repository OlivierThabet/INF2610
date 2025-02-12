#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#include <fcntl.h>

int main() {
    int pipe1[2], pipe2[2];
    pid_t pid1, pid2, pid3;

    pipe(pipe1);
    pid1 = fork();

    if (pid1 == 0) { 
        int fd_in = open("In.txt", O_RDONLY);
        dup2(fd_in, STDIN_FILENO);
        close(fd_in);

        dup2(pipe1[1], STDOUT_FILENO);
        close(pipe1[0]);
        close(pipe1[1]);
        
        //execlp("rev", "rev", NULL);
        exit(EXIT_FAILURE);
    }

    close(pipe1[1]);
    pipe(pipe2);
    pid2 = fork();

    if (pid2 == 0) {
        dup2(pipe1[0], STDIN_FILENO);
        close(pipe1[0]);

        dup2(pipe2[1], STDOUT_FILENO);
        close(pipe2[0]);
        close(pipe2[1]);

        //execlp("rev", "rev", NULL);
        exit(EXIT_FAILURE);
    }
    close(pipe1[0]);
    close(pipe2[1]);

    pid3 = fork();
    if (pid3 == 0) {
        dup2(pipe2[0], STDIN_FILENO);
        close(pipe2[0]);

        execlp("diff", "diff", "-", "In.txt", "-s", NULL);
        exit(EXIT_FAILURE);
    }

    close(pipe2[0]);
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
    waitpid(pid3, NULL, 0);

    return EXIT_SUCCESS;
}