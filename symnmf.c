#include <math.h>
#include <stdlib.h>

int distance(int* first_vec, int* second_vec, int d){
    int i, dif, sum;
    sum = 0;

    for(i = 0; i < d; i++){
        dif = first_vec[i] - second_vec[i];
        sum += dif * dif;
    }
    return sum;
}


int **similar_matrix(int** mat, int n, int d){
    int *data;
    int **A;
    int i, j;
    
    data = (int*)calloc(n * d, sizeof(int));
    assert(data != NULL);

    A = (int**)calloc(n, sizeof(int *));
    assert(data!= NULL);

    for(i = 0; i < n; i++){
        A[i] = data + i * d;
    }
    for(i = 0; i < n; i++){
        for(j = 0; j < d; j++){
            if(i == j){
                A[i][j] = 0;
            }
            else{
                A[i][j] = exp(-(distance(mat[i], mat[j], d)) / 2);
            }
        }
    }
    return A;
}