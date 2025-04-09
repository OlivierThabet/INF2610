/*
 * Ecole Polytechnique Montreal - GIGL
 * Hiver 2025
 * Challenges - deadlock_45c
 *
 * Popovic, Victor (2288035) et Thabet, Olivier (2294559)
*/
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

//Les 4 conditions pour un interblocage:
//1.Exclusion mutuelle: Ligne 23 le sémaphore sem_critical est la ressource soit disponible soit allouée à un thread.
//2.Détention et attente: non-respectée: Le thread s'alloue le sémaphore et n'attend pas pour une autre ressource ensuite.
//3.Pas de réquisition: Ligne 36, 41 ou 39 (après solution): Chaque thread prend le sémaphore et le libère lui-même par la suite avec post.
//4.Attente circulaire: Non-respectée: Une fois que le thread obtient la ressource, il fonctionne indépendemment des autres.
#define ENTRY_CODE 7
#define N_THREADS 5

sem_t sem_critical;

int try_lock(int key){
    if(key == ENTRY_CODE){
        printf("Door unlocked\n");
        return 1;
    }
    sleep(1);
    return 0;
}

void* door(void* args){
    while(1){
        sem_wait(&sem_critical);
        int key = rand() % 30;
        //section problématique: Si un thread réussi à débloquer la porte et entre dans le if, il exit sans libérer la ressource et les autres threads attendent donc à l'infini.
        if(try_lock(key)){//Il s'agit don d'un blocage puisque pas toute les conditions d'interblocage sont respectées et le thread qui a fini son exécution n'attend pas pour une ressource des autres threads.
            sem_post(&sem_critical);//Solution: on libère la ressource quand on déverrouille la porte pour débloquer un des threads qui attend après celle-ci.
            pthread_exit(NULL);}
        //section problématique
        sem_post(&sem_critical);
    }
}
int main() {

    sem_init(&sem_critical, 0, 1); 
    pthread_t threads[N_THREADS];

    for(int i = 0; i < N_THREADS; i++){
        pthread_create(&threads[i], NULL, door, NULL);
    }

    for(int i = 0; i < N_THREADS; i++){
        pthread_join(threads[i], NULL);
    }

    return 0;
}