/*
 * École Polytechnique Montreal - GIGL
 * Hiver 2025
 *
 * Popovic, Victor (2288035) et Thabet, Olivier (2294559)
*/

#include <pthread.h>
#include "merge_sort.h"

#include <semaphore.h>
extern sem_t *mutex;

void *parallel_merge_sort(void *arg) {
    int *bounds = (int *)arg;
    int left = bounds[0];
    int right = bounds[1];
    
    struct timeval local_start, local_end;
    gettimeofday(&local_start, NULL);
    merge_sort(left, right);
    gettimeofday(&local_end, NULL);
    double elapsed = (local_end.tv_sec - local_start.tv_sec) + (local_end.tv_usec - local_start.tv_usec) / 1000000.0;
    printf("Thread sorted [%d - %d] in %.6f seconds\n", left, right, elapsed);
    free(arg);
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <array_size> <num_threads>\n", argv[0]);
        return EXIT_FAILURE;
    }
    
    int size = atoi(argv[1]);
    int num_threads = atoi(argv[2]);
    
    SharedData local_data;
    shared_data = &local_data;
    
    int local_array[size];
    shared_data->array = local_array;
    
    shared_data->size = size;
    
    if (sem_init(mutex, 0, 1) != 0) {
        perror("pthread_mutex_init failed");
        return EXIT_FAILURE;
    }
    srand(time(NULL));
    for (int i = 0; i < size; i++) {
        shared_data->array[i] = rand() % 100;
    }
    
    struct timeval start, end;
    gettimeofday(&start, NULL);
    
    int step = size / num_threads;
    pthread_t threads[num_threads];
    
    for (int i = 0; i < num_threads; i++) {
        int *bounds = malloc(2 * sizeof(int));
        bounds[0] = i * step;
        bounds[1] = (i == num_threads - 1) ? size - 1 : (bounds[0] + step - 1);
        if (pthread_create(&threads[i], NULL, parallel_merge_sort, bounds) != 0) {
            perror("pthread_create failed");
            free(bounds);
            return EXIT_FAILURE;
        }
    }
    
    for (int i = 0; i < num_threads; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("pthread_join failed");
            return EXIT_FAILURE;
        }
    }
    
    int mid = step - 1;
    for (int i = 1; i < num_threads; i++) {
        int right = (i + 1) * step - 1;
        if (right >= size) {
            right = size - 1;
        }
        merge(0, mid, right);
        mid = right;
    }
    
    gettimeofday(&end, NULL);
    
    double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;
    printf("Total execution time: %.6f seconds\n", elapsed);
    show_array();
    
    if (sem_destroy(mutex) != 0) {
        perror("pthread_mutex_destroy failed");
    }
    
    return EXIT_SUCCESS;
}

void merge_sort( int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        merge_sort(left, mid);
        merge_sort(mid + 1, right);
        merge(left, mid, right);
    }
}

void merge(int left, int mid, int right) {
    int i, j, k;
    int n1 = mid - left + 1;
    int n2 = right - mid;

    int L[n1], R[n2];

    for (i = 0; i < n1; i++)
        L[i] = shared_data->array[left + i];
    for (j = 0; j < n2; j++)
        R[j] = shared_data->array[mid + 1 + j];

    i = 0;
    j = 0;
    k = left;

    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            shared_data->array[k] = L[i];
            i++;
        } else {
            shared_data->array[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        shared_data->array[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        shared_data->array[k] = R[j];
        j++;
        k++;
    }
}

void show_array(){
    printf("Sorted array: ");
    for (int i = 0; i < shared_data->size; i++) {
        printf("%d ", shared_data->array[i]);
    }
    printf("\n");
}
