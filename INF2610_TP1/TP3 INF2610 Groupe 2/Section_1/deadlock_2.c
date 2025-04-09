/*
 * Ecole Polytechnique Montreal - GIGL
 * Hiver 2025
 * Challenges - deadlock_2.c
 *
 * Popovic, Victor (2288035) et Thabet, Olivier (2294559)
*/
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

//Conditions d'interblocage:
//1.Exclusion mutuelle: Non respectée: Il 'y a pas de ressource soit allouée à un seul processus soit disponible. Le pipe n'est pas considéré
//comme une ressource puisqu'il ne présente pas de "race condition" entre les processus-> ils s'exécuteront toujours dans le même ordre.
//2.Détention et attente: Non Respectée: il n' y a pas de ressource à la base.
//3.Pas de réquisition: Non respectée: il n'y a pas de ressource à la base.
//4.Attente circulaire: On pourrait considérer le read (ligne 28) comme une attente circulaire puisqu'il doit attendre qu'on write dans le pipe pour le read et le processus parent doit attendre que son enfant se termine pour se terminer avec le WAIT ligne 43.

int main() {
    int fd[2];

    pipe(fd);

    if(!fork()){
        close(fd[1]);
        char* string[6];    
        read(fd[0], string, 6);
        exit(0);
    }

    if(!fork()){
        close(fd[1]);
        close(fd[0]);
        execlp("ls", "ls", "-l", NULL);
        exit(0);
    }


    char string[6] = "Salut!";
    write(fd[1], &string, strlen(string));
    //Il s'agit ici d'un blocage puisque plusieurs conditions ne sont pas respectées.
     while(wait(NULL) > 0); //Solution: cette ligne a été bougée après l'exécution dans le tube. Cela permet de write dans le premier pipe,
     //permettant au premier processus enfant de s'exécuter et donc de ne pas rester pris dans la boucle du while puisqu'on attend
     //à l'infini pour le premier processus enfant puis que le write se trouve après la boucle while.
    printf("Process completed!\n");
    close(fd[0]);
    close(fd[1]);
    return 0;
}