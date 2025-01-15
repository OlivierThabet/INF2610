/*
 * Ecole Polytechnique Montreal - GIGL
 * Hiver 2025
 * SystemCalls - part1.c
 *
 * Ajoutez vos noms, prénoms et matricules
*/
#include<stdio.h> 
#include <fcntl.h> 

int main () {

    int file = open("systemcalls_output2.txt", O_WRONLY | O_TRUNC, 0644);

    const char *message = "Saisissez votre texte suivi de CTRL-D :\n";
    write(STDOUT_FILENO, message, strlen(message));

    char buffer;
    while (read(STDIN_FILENO, &buffer, 1) > 0) {
        
       write(file, &buffer, 1);
    }
    close(file);

    return 0;
}

