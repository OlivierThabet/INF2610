#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

#define BUFFER_SIZE 5
#define N_THREADS 1
#define N_THREADS_2 10

int buffer[BUFFER_SIZE];
int ip;
int ic;

sem_t sem_initial;
sem_t sem_busy;
sem_t sem_critical; //condition 3 - Pas de réquisition : sem_critical ne peut être réémis que par le producer/consommateur qui l'a pris à la base.

int flag = 0;

void* producer(void* arg) {
    while (1) {
        sem_wait(&sem_initial);     // Condition 2 - Détention et attente (obtient sem_initial, puis attend sem_critical)
        sem_wait(&sem_critical);    // Condition 1 - Exclusion mutuelle (section critique pour écrire dans le buffer)
        buffer[ip] = rand() % 9 + 1;
        ip = (ip + 1) % BUFFER_SIZE;
        sem_post(&sem_critical);
        sem_post(&sem_busy);
        if (flag)
            break;
    }
    pthread_exit(NULL);
}
//Condition 4 - Attente circulaire: Le producer attend sem_initial produit par le consumer et le consumer attends sem_busy produit par le producer.
void* consumer(void* arg) {
    while (1) {
        sem_wait(&sem_busy);        //  Condition 2 - Détention et attente (obtient sem_busy, puis attend sem_critical)
        sem_wait(&sem_critical);    // Condition 1 - Exclusion mutuelle (lecture du buffer)
        int x = buffer[ic];
        ic = (ic + 1) % BUFFER_SIZE;
        sem_post(&sem_critical);
        sem_post(&sem_initial);
        if(x == 0){
            break;
        }
    }
    pthread_exit(NULL);
}

void changeFlag(){
    flag = 1;
    printf("Flag changed\n");
}

int main() {
    pthread_t prod_thread[N_THREADS], cons_thread[N_THREADS_2];

    sem_init(&sem_initial, 0, BUFFER_SIZE);  
    sem_init(&sem_busy, 0, 0);               
    sem_init(&sem_critical, 0, 1);          
    signal(SIGALRM, &changeFlag);    

    for(int i = 0; i < N_THREADS; i++){
        pthread_create(&prod_thread[i], NULL, producer, NULL);
    }

    for(int i = 0; i < N_THREADS_2; i++){
        pthread_create(&cons_thread[i], NULL, consumer, NULL);
    }

    alarm(1); 

    for(int i = 0; i < N_THREADS; i++){
        pthread_join(prod_thread[i], NULL);
    }

    
   //section problématique 
    for(int i = 0; i < N_THREADS_2; i++){
        buffer[ip] = 0;
        ip = (ip + 1) % BUFFER_SIZE;
        sem_post(&sem_busy); //solution: c'est la solution, car elle assure que les conso aient accès à 
        //sem_busy après le flag car la production de sem_busy sera stoppée à ce moment.
    }
    //section problématique


    for(int i = 0; i < N_THREADS_2; i++){
        pthread_join(cons_thread[i], NULL);
    }

    sem_destroy(&sem_initial);
    sem_destroy(&sem_busy);
    sem_destroy(&sem_critical);

    return 0;
}