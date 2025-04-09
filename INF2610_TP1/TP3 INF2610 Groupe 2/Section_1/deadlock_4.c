/*
 * Ecole Polytechnique Montreal - GIGL
 * Hiver 2025
 * Challenges - deadlock_4.c
 *
 * Popovic, Victor (2288035) et Thabet, Olivier (2294559)
*/
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
//Les 4 conditions pour un interblocage:
//1.Exclusion mutuelle: Ligne 19 le mutex est la ressource soit disponible soit allouée à un thread.
//2.Détention et attente: Ligne 37, 40 Un thread verrouille le left du mutex et attend pour verrouiller le right.
//3.Pas de réquisition: Ligne 37, 40, 44, 45 Le thread verrouille et déverrouille le mutex avec lock et unlock de left et right.
//4.Attente circulaire: Ligne 40 Chaque thread peu attendre apres un autre (le right de l'un peu être le left de l'autre).
#define NUM_THREADS 5
#define MAX_ROUNDS 3

pthread_mutex_t locks[NUM_THREADS];

void* cons(void* arg) {
    int id = *(int*)arg;
    int rounds = 0;

    while (rounds < MAX_ROUNDS) {
        sleep(1);
        int left = id;
        int right = (id + 1) % NUM_THREADS;
            if (left > right) {//solution: On impose un ordre global sur les locks chaque thread doit toujours acquérir en premier le lock avec l’index le plus petit,
                            //puis celui avec l’index le plus grand.
            int temp =left;
            left = right;
            right = temp;
        }

        //Section problématique
        pthread_mutex_lock(&locks[left]); //il s'agit ici d 'interblocage (problème des philosophes mangeurs). Tous les threads lock left 
        //Et attendent une seconde avant de lock right, mais le left de certains représente le right de d'autres et se retrouvent donc blockés au moment du lock right.
        sleep(1);
        pthread_mutex_lock(&locks[right]);
        //fin de la section problématique
        sleep(1);
        rounds++;
        pthread_mutex_unlock(&locks[left]);
        pthread_mutex_unlock(&locks[right]);
    }

    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    int ids[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_mutex_init(&locks[i], NULL);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        ids[i] = i;
        pthread_create(&threads[i], NULL, cons, &ids[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}