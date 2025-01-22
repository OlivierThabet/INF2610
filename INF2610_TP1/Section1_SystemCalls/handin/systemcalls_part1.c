/*
 * Ecole Polytechnique Montreal - GIGL
 * Hiver 2025
 * SystemCalls - part1.c
 *
 * Popovic, Victor (2288035) et Thabet, Olivier (2294559)
*/
#include<stdio.h> 
#include <string.h>
#include <fcntl.h> 
#include <unistd.h>

int main () {

    int file = open("systemcalls_output2.txt", O_WRONLY | O_TRUNC, 0644);

    const char *message = "Saisissez votre texte suivi de CTRL-D :\n";
    write(STDOUT_FILENO, message, strlen(message));

    char buffer;
    while (read(STDIN_FILENO, &buffer, 1) > 0) {
        
       if ( write(file, &buffer, 1) == -1){close(file);
       return 1;}
    }
    
    return 0;
}
