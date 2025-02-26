/*
 * Ecole Polytechnique Montreal - GIGL
 * Hiver 2025
 * tubes - nommés
 *
 * Popovic, Victor (2288035) et Thabet, Olivier (2294559)
*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>

int main() {
    pid_t pid1, pid2, pid3;
    mkfifo("fifo1", 0600);
    mkfifo("fifo2", 0600);

    pid1 = fork();
    if (pid1 == 0) { 
        int file = open("In.txt", O_RDONLY);
        int fd_out = open("fifo1", O_WRONLY);
        dup2(file, STDIN_FILENO);
        close(file);
        dup2(fd_out, STDOUT_FILENO);
        close(fd_out);
        execlp("rev", "rev", NULL);
    }

    pid2 = fork();
    if (pid2 == 0) {
        int fd_in = open("fifo1", O_RDONLY);
        int fd_out = open("fifo2", O_WRONLY);
        dup2(fd_in, STDIN_FILENO);
        close(fd_in);
        dup2(fd_out, STDOUT_FILENO);
        close(fd_out);
        execlp("rev", "rev", NULL);
    }

    pid3 = fork();
    if (pid3 == 0) {
        int fd_in = open("fifo2", O_RDONLY);
        dup2(fd_in, STDIN_FILENO);
        close(fd_in);
        execlp("diff", "diff", "-", "In.txt", "-s", NULL);
    }

    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
    waitpid(pid3, NULL, 0);

    unlink("fifo1");
    unlink("fifo2");

    return EXIT_SUCCESS;
}
