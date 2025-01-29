/*
 * Ecole Polytechnique Montreal - GIGL
 * Hiver 2025
 * Processes - part1.c
 *
 * Popovic, Victor (2288035) et Thabet, Olivier (2294559)
*/
#include "libprocesslab/libprocesslab.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

const int NB_CHILDS_LEVEL_1_3 = 4;
const int NB_CHILDS_LEVEL_0 = 3;


int countChildren() {
    int nChildren = 0;
    int status;

    while (wait(&status) > 0) {
        if (WIFEXITED(status)) {
            nChildren += WEXITSTATUS(status); 
        }
        nChildren++; 
    }

    return nChildren;
}

void question1() {
    pid_t pid1, pid2, pid3;

    registerProc(getpid(), getppid(), 0, 0);

    pid1 = fork();
    if (pid1 == 0) {
        registerProc(getpid(), getppid(), 1, 1);

        pid_t pid1_1 = fork();
        if (pid1_1 == 0) {
            registerProc(getpid(), getppid(), 2, 1);
            exit(0); 
        }
        int nChildren = countChildren(); 
        exit(nChildren); 
    }

    pid2 = fork();
    if (pid2 == 0) {
        registerProc(getpid(), getppid(), 1, 2);

        pid_t pid2_1 = fork();
        if (pid2_1 == 0) {
            registerProc(getpid(), getppid(), 2, 2);
            exit(0); 
        }
        int nChildren = countChildren(); 
        exit(nChildren); 
    }

    pid3 = fork();
    if (pid3 == 0) {
        registerProc(getpid(), getppid(), 1, 3);

        pid_t pid3_1 = fork();
        if (pid3_1 == 0) {
            registerProc(getpid(), getppid(), 2, 3);
            exit(0);
        }

        pid_t pid3_2 = fork();
        if (pid3_2 == 0) {
            registerProc(getpid(), getppid(), 2, 4);
            exit(0);
        }

        pid_t pid3_3 = fork();
        if (pid3_3 == 0) {
            registerProc(getpid(), getppid(), 2, 5);
            exit(0);
        }

        pid_t pid3_4 = fork();
        if (pid3_4 == 0) {
            registerProc(getpid(), getppid(), 2, 6);
            exit(0);
        }

        int nChildren = countChildren();
        exit(nChildren); 
    }

    int total_nChildren = countChildren(); 
    printf("Le processus level0 a %d nChildren.\n", total_nChildren);
    printProcRegistrations(); 
    execlp("ls", "ls", "-l", NULL);
}
