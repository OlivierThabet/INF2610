/*
 * École Polytechnique Montreal - GIGL
 * Hiver 2025
 *
 * Popovic, Victor (2288035) et Thabet, Olivier (2294559)
*/

#include "merge_sort.h"
#include <pthread.h>

void parallel_merge_sort(int left, int right, int num_threads);
void *threaded_merge_sort(void *arg);

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
    
    srand(time(NULL));
    for (int i = 0; i < size; i++) {
        shared_data->array[i] = rand() % 100;
    }
    
    struct timeval start, end;
    gettimeofday(&start, NULL);
    
    parallel_merge_sort(0, size - 1, num_threads);
    
    gettimeofday(&end, NULL);
    
    double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;
    printf("Temps d'exec: %.6f sec\n", elapsed);
    
    return EXIT_SUCCESS;
}

void parallel_merge_sort(int left, int right, int num_threads) {
    if (num_threads <= 1 || right - left < 1) {
        merge_sort(left, right);
        return;
    }
    
    int mid = left + (right - left) / 2;
    pthread_t left_thread, right_thread;
    int *left_args = malloc(2 * sizeof(int));
    left_args[0] = left;
    left_args[1] = mid;
    int *right_args = malloc(2 * sizeof(int));
    right_args[0] = mid + 1;
    right_args[1] = right;

    pthread_create(&left_thread, NULL, threaded_merge_sort, left_args);
    pthread_create(&right_thread, NULL, threaded_merge_sort, right_args);
    
    pthread_join(left_thread, NULL);
    free(left_args);
    pthread_join(right_thread, NULL);
    free(right_args);
    
    merge(left, mid, right);
}

void *threaded_merge_sort(void *arg) {
    int *bounds = (int *)arg;
    merge_sort(bounds[0], bounds[1]);
    return NULL;
}

void merge_sort(int left, int right) {
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