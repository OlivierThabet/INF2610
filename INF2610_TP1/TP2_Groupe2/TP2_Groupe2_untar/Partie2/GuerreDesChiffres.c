/*
 * Ecole Polytechnique Montreal - GIGL
 * Hiver 2025
 * 
 * Guerre des chiffres
 *
 * Popovic, Victor (2288035) et Thabet, Olivier (2294559)
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>
#include <semaphore.h>

#define TAILLE_MAX_TAMPON 100

int *tampon;
int taille_tampon;
int index_production = 0;
int index_consommation = 0;
int nb_producteurs;
int nb_consommateurs;
int somme_produite = 0; 
int somme_consomme = 0;
int nb_chiffres_produits = 0;
int nb_chiffres_consommes = 0;
int flag = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
sem_t sem_production;
sem_t sem_consommation;

void action(int signal) {
    flag = 1;
}

void* producteur(void* pid) {
    int somme_locale = 0;
    while (!flag) {
        int chiffre = (rand() % 9) + 1; 

        sem_wait(&sem_production);
        pthread_mutex_lock(&mutex);
        
        tampon[index_production] = chiffre;
        index_production = (index_production + 1) % taille_tampon;
        somme_produite += chiffre;
        nb_chiffres_produits++;
        somme_locale += chiffre;
        
        pthread_mutex_unlock(&mutex);
        sem_post(&sem_consommation);
    }
    pthread_exit((void*)(long)somme_locale);
}

void* consommateur(void* cid) {
    int somme_locale = 0;
    while (1) {
        sem_wait(&sem_consommation);
        pthread_mutex_lock(&mutex);
        
        int chiffre = tampon[index_consommation];
        index_consommation = (index_consommation + 1) % taille_tampon;
        somme_consomme += chiffre;
        if (chiffre !=0) {
            nb_chiffres_consommes++;
        }
        somme_locale += chiffre;

        pthread_mutex_unlock(&mutex);
        sem_post(&sem_production);
        
        if (chiffre == 0) break;
    }
    pthread_exit((void*)(long)somme_locale);
}

int main(int argc, char* argv[]) {
    
    nb_producteurs = atoi(argv[1]);
    nb_consommateurs = atoi(argv[2]);
    taille_tampon = atoi(argv[3]);
    
    if (taille_tampon > TAILLE_MAX_TAMPON) {
        return EXIT_FAILURE;
    }
    
    tampon = malloc(taille_tampon * sizeof(int));
    pthread_t producteurs[nb_producteurs];
    pthread_t consommateurs[nb_consommateurs];
    int ids[nb_producteurs > nb_consommateurs ? nb_producteurs : nb_consommateurs]; 

    sem_init(&sem_production, 0, taille_tampon);
    sem_init(&sem_consommation, 0, 0);

    signal(SIGALRM, action);
    alarm(1);
    
    for (int i = 0; i < nb_producteurs; i++) {
        ids[i] = i;
        pthread_create(&producteurs[i], NULL, producteur, &ids[i]);
    }
    for (int i = 0; i < nb_consommateurs; i++) {
        ids[i] = i;
        pthread_create(&consommateurs[i], NULL, consommateur, &ids[i]);
    }
    
    int somme_totale_produite = 0, somme_totale_consomme = 0;
    for (int i = 0; i < nb_producteurs; i++) {
        void* retour;
        pthread_join(producteurs[i], &retour);
        somme_totale_produite += (long)retour;
    }
    
    for (int i = 0; i < nb_consommateurs; i++) {
        sem_wait(&sem_production);
        pthread_mutex_lock(&mutex);
        tampon[index_production] = 0;
        index_production = (index_production + 1) % taille_tampon;
        pthread_mutex_unlock(&mutex);
        sem_post(&sem_consommation);
    }
    
    for (int i = 0; i < nb_consommateurs; i++) {
        void* consomme_dans_thread;
        pthread_join(consommateurs[i], &consomme_dans_thread);
        somme_totale_consomme += (long)consomme_dans_thread;
    }
    
    printf("Nombre de chiffres produits: %d\n", nb_chiffres_produits);
    printf("Nombre de chiffres consommés: %d\n", nb_chiffres_consommes);
    printf("Somme totale produite: %d\n", somme_totale_produite);
    printf("Somme totale consommée: %d\n", somme_totale_consomme);

    sem_destroy(&sem_production);
    sem_destroy(&sem_consommation);
    free(tampon);
    return 0;
}
