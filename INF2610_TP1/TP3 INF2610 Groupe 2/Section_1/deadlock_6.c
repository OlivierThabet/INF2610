/*
 * Ecole Polytechnique Montreal - GIGL
 * Hiver 2025
 * Challenges - deadlock_6.c
 *
 * Popovic, Victor (2288035) et Thabet, Olivier (2294559)
*/
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

#define N_THREADS 5
//Condition 1 non respectée: Le flag n'est pas considérée comme une ressource allouée puisqu'il est utilisé de manière indépendante aux threads. Il n'est 
//alloué ni libéré par les threads.
//Il n'y a pas d'interblocage dans ce fichier, aucune des 4 conditions n'est respectée puisque nous n'avons pas de ressources à la base. Les threads ne faisait qu'attendre 
// à cause d'une erreur mathématique et sont relativement indépendants les uns des autres (a part pour le changement du flag). Il y avait donc un blocage mathématique.
int flag = 0;

void* function_that_does_something(void* arg) {
    while (1) {
        if (flag)
            break;
    }
    pthread_exit(NULL);
}

void changeFlag(){
        //Solution: on enlève cette ligne, car elle ne sera jamais true (quelque chose %10 n'est jamais égal à 10)
        flag = 1;
    printf("Signal called\n");
}

int main() {
    pthread_t threads[N_THREADS];
 
    signal(SIGALRM, &changeFlag);      

    for(int i = 0; i < N_THREADS; i++){
        pthread_create(&threads[i], NULL, function_that_does_something, NULL);
    }

    alarm(1);

    for(int i = 0; i < N_THREADS; i++){
        pthread_join(threads[i], NULL);
    }

    return 0;
}