/*
 * Ecole Polytechnique Montreal - GIGL
 * Hiver 2025
 * Challenges - part2.c
 *
 * Ajoutez vos noms, prénoms et matricules
*/

#include "challenges_part2.h"

// short calculateValue(short colA, short rowB){
//     short sum = 0;
//     for(int i; i<colA; i++)
//     {
//         for(int j; j<rowB; j++){
//             sum+= A.matrix[][]

//         }}

// }
MatrixInfos* multiply(MatrixInfos* A, MatrixInfos* B)  {
    // TODO
    if(A->cols!=B->rows)
    {return NULL;}

    MatrixInfos* C;
    short nbRowsAnswer = A->rows;
    short nbColsAnswer = B->cols;

    for (int i=0; i<nbRowsAnswer;i++){

        for(int j=0; j<nbColsAnswer;j++){

            for(int k=0; k<A->cols; k++){
                C->matrix[i][j] += A->matrix[j][k]*B->matrix[k][j];}
        }
    }


    return C;
}


// This main is to help you in your development. 
// Don't forget to comment it out when you run the tests.
// int main(int argc, char*argv[])
// {
   
//     return 0;
// }
