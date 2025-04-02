/*
 * École Polytechnique Montreal - GIGL
 * Hiver 2025
 *
 * Popovic, Victor (2288035) et Thabet, Olivier (2294559)
*/

#include "merge_sort.h"

 void log_sort_operation(int start, int end) {
    FILE *file = fopen("sorted_array.txt", "a");
        fprintf(file, "Start = %d, End = %d, sorted = [", start, end);
        for (int i = start; i <= end; i++) {
            fprintf(file, "%d%s", shared_data->array[i], (i < end) ? ", " : "");
        }
        fprintf(file, "]\n");
        fclose(file);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <array_size> <num_processes>\n", argv[0]);
        return EXIT_FAILURE;
    }
    
    int size = atoi(argv[1]);
    int num_processes = atoi(argv[2]);
       
    //CODE GENERE PAR CHAT GPT

    shared_data = mmap(NULL, sizeof(SharedData), 
                       PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    
    shared_data->array = mmap(NULL, size * sizeof(int), 
                              PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    //FIN DU CODE GENERE PAR CHAT GPT
    
    shared_data->size = size;
    
    srand(time(NULL));
    for (int i = 0; i < size; i++) {
        shared_data->array[i] = rand() % 100;
    }
    
    FILE *file = fopen("sorted_array.txt", "w");
        fprintf(file, "array = [");
        for (int i = 0; i < size; i++) {
            fprintf(file, "%d%s", shared_data->array[i], (i < size - 1) ? ", " : "]");
        }
        fprintf(file, "\n");
        fclose(file);
    
    
    struct timeval start, end;
    gettimeofday(&start, NULL);
    
    int step = size / num_processes;
    pid_t pids[num_processes];
    
    for (int i = 0; i < num_processes; i++) {
        int left = i * step;
        int right = (i == num_processes - 1) ? size - 1 : (left + step - 1);
        
        if ((pids[i] = fork()) == 0) {
            
            merge_sort(left, right);
            
            log_sort_operation(left, right);
            exit(0);
        }
    }
    
    for (int i = 0; i < num_processes; i++) {
        waitpid(pids[i], NULL, 0);
    }
    
    int mid = step - 1;
    for (int i = 1; i < num_processes; i++) {
        int right = (i + 1) * step - 1;
        if (right >= size) {
            right = size - 1;
        }
        merge(0, mid, right);
        log_sort_operation(0, right);
        mid = right;
    }
    
    gettimeofday(&end, NULL);
    
    double total_time = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;
    printf("temps total : %.6f sec\n", total_time);
    
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