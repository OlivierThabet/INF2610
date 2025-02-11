/*
 * Ecole Polytechnique Montreal - GIGL
 * Hiver 2025
 * Challenges - part2.c
 *
 * Popovic, Victor (2288035) et Thabet, Olivier (2294559)
*/

#include "challenges_part2.h"


// La valeur de MAX_PROCESSES est arbitaire. Nous avons choisi 4 parce que 
// nous croyons que la performance des orinateurs de l'ecole est relativement faible.
// Avec des ordinateurs ayant des meilleurs performances on aurait pu essayer de mettre 8 ou 16 par exemple.
#define MAX_PROCESSES 4 

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
    if (A->cols != B->rows) {
        return NULL;
    }

    MatrixInfos* result = (MatrixInfos*)malloc(sizeof(MatrixInfos));

    result->rows = A->rows;
    result->cols = B->cols;
    result->matrix = (short*)malloc(result->rows * result->cols * sizeof(short));

    int rows_per_process = A->rows / MAX_PROCESSES;
    int remaining_rows = A->rows % MAX_PROCESSES;
    pid_t pids[MAX_PROCESSES];

    // 1. CODE GÉNÉRÉ PAR CHAT GPT
    int fd = shm_open("shared_matrix", O_CREAT | O_RDWR, 0666);
    ftruncate(fd, A->rows * B->cols * sizeof(short));
    short* shared_memory = mmap(NULL, A->rows * B->cols * sizeof(short), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (shared_memory == MAP_FAILED) {
        free(result->matrix);
        free(result);
        return NULL;
    }
    memset(shared_memory, 0, A->rows * B->cols * sizeof(short));
    // 1. FIN DU CODE GÉNÉRÉ PAR CHAT GPT

    for (int proc = 0; proc < MAX_PROCESSES; proc++) {
        int start = proc * rows_per_process;
        int end = start + rows_per_process;
        if (proc == MAX_PROCESSES - 1) {
            end += remaining_rows;
        }
        
        if ((pids[proc] = fork()) == 0) {
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
    
    for (int i = 0; i < MAX_PROCESSES; i++) {
        waitpid(pids[i], NULL, 0);
    }
    
    // 2. CODE GÉNÉRÉ PAR CHAT GPT
    memcpy(result->matrix, shared_memory, A->rows * B->cols * sizeof(short));
    // 2. FIN DU CODE GÉNÉRÉ PAR CHAT GPT

    return result;
}

// This main is to help you in your development. 
// Don't forget to comment it out when you run the tests.
// int main(int argc, char* argv[]) {
//     short A_data[] = {1, 2, 3, 4};
//     short B_data[] = {5, 6, 7, 8};
//     MatrixInfos A = {A_data, 2, 2};
//     MatrixInfos B = {B_data, 2, 2};
//     MatrixInfos* C = multiply(&A, &B);
//     printf("Test 1 Result: \n%d %d\n%d %d\n", C->matrix[0], C->matrix[1], C->matrix[2], C->matrix[3]);
// }

