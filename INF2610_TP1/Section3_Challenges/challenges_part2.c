/*
 * Ecole Polytechnique Montreal - GIGL
 * Hiver 2025
 * Challenges - part2.c
 *
 * Popovic, Victor (2288035) et Thabet, Olivier (2294559)
*/

#include "challenges_part2.h"

#define MAX_PROCESSES 4 // Nombre maximum de processus enfant

typedef struct {
    short* A;
    short* B;
    short* C;
    int row;
    int A_cols;
    int B_cols;
} ThreadData;

void* multiply_row(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    for (int j = 0; j < data->B_cols; j++) {
        data->C[data->row * data->B_cols + j] = 0;
        for (int k = 0; k < data->A_cols; k++) {
            data->C[data->row * data->B_cols + j] += 
                data->A[data->row * data->A_cols + k] * data->B[k * data->B_cols + j];
        }
    }
    return NULL;
}

MatrixInfos* multiply(MatrixInfos* A, MatrixInfos* B) {
    if (A->cols != B->rows) return NULL; // Vérification de validité

    MatrixInfos* result = (MatrixInfos*)malloc(sizeof(MatrixInfos));
    if (!result) return NULL;

    result->rows = A->rows;
    result->cols = B->cols;
    result->matrix = (short*)malloc(result->rows * result->cols * sizeof(short));
    if (!result->matrix) {
        free(result);
        return NULL;
    }

    int rows_per_process = A->rows / MAX_PROCESSES;
    int remaining_rows = A->rows % MAX_PROCESSES;
    pid_t pids[MAX_PROCESSES];
    int fd = shm_open("shared_matrix", O_CREAT | O_RDWR, 0666);
    ftruncate(fd, A->rows * B->cols * sizeof(short));
    short* shared_memory = mmap(NULL, A->rows * B->cols * sizeof(short), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    for (int p = 0; p < MAX_PROCESSES; p++) {
        int start = p * rows_per_process;
        int end = start + rows_per_process;
        if (p == MAX_PROCESSES - 1) end += remaining_rows;
        
        if ((pids[p] = fork()) == 0) { // Processus enfant
            pthread_t threads[end - start];
            ThreadData thread_data[end - start];
            
            for (int i = start; i < end; i++) {
                thread_data[i - start] = (ThreadData){
                    .A = A->matrix,
                    .B = B->matrix,
                    .C = shared_memory,
                    .row = i,
                    .A_cols = A->cols,
                    .B_cols = B->cols
                };
                pthread_create(&threads[i - start], NULL, multiply_row, &thread_data[i - start]);
            }
            for (int i = start; i < end; i++) {
                pthread_join(threads[i - start], NULL);
            }
            exit(0);
        }
    }
    
    for (int p = 0; p < MAX_PROCESSES; p++) {
        waitpid(pids[p], NULL, 0);
    }
    
    memcpy(result->matrix, shared_memory, A->rows * B->cols * sizeof(short));
    munmap(shared_memory, A->rows * B->cols * sizeof(short));
    shm_unlink("shared_matrix");

    return result;
}


// This main is to help you in your development. 
// Don't forget to comment it out when you run the tests.
// int main(int argc, char* argv[]) {
//     // Test 1: Multiplication de matrices 2x2
//     short A_data[] = {1, 2, 3};
//     short B_data[] = {5, 6, 7, 8};
//     MatrixInfos A = {A_data, 1, 3};
//     MatrixInfos B = {B_data, 2, 2};
//     MatrixInfos* C = multiply(&A, &B);
//     printf("Test 1 Result: %d %d\n                 %d %d\n", C->matrix[0], C->matrix[1], C->matrix[2], C->matrix[3]);
//     free(C->matrix);
//     free(C);

//     // Test 2: Multiplication de matrices 3x2 et 2x3
//     short A_data2[] = {1, 2, 3, 4, 5, 6};
//     short B_data2[] = {7, 8, 9, 10, 11, 12};
//     MatrixInfos A2 = {A_data2, 3, 2};
//     MatrixInfos B2 = {B_data2, 2, 3};
//     MatrixInfos* C2 = multiply(&A2, &B2);
//     printf("Test 2 Result: %d %d %d\n                 %d %d %d\n                 %d %d %d\n", 
//            C2->matrix[0], C2->matrix[1], C2->matrix[2], 
//            C2->matrix[3], C2->matrix[4], C2->matrix[5], 
//            C2->matrix[6], C2->matrix[7], C2->matrix[8]);
//     free(C2->matrix);
//     free(C2);

//     return 0;
// }
