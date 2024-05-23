#include <math.h>
#include <stdlib.h>

double distance(double* first_vec, double* second_vec, int d){
    int i;
    double dif, sum;
    sum = 0;

    for(i = 0; i < d; i++){
        dif = first_vec[i] - second_vec[i];
        sum += dif * dif;
    }
    return sum;
}

double **create_matrix(int n, int d){
    double *data;
    double **mat;
    int i;

    data = (double*)calloc(n * d, sizeof(double));
    assert(data != NULL);

    mat = (double**)calloc(n, sizeof(double *));
    assert(mat != NULL);

    for(i = 0; i < n; i++){
        mat[i] = data + i * d;
    }
    return mat;
}

double **similar_matrix(double** mat, int n, int d){
    double **A = create_matrix(n, n);
    int i, j;

    for(i = 0; i < n; i++){
        for(j = 0; j < n; j++){
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

double **diaognal_degree_matrix(double** A, int n, int d){
    int i, j;
    double **D = create_matrix(n, n);;

    for(i = 0; i < n; i++){
        for(j = 0; j < n; j++){
            D[i][i] += A[i][j];
        }
    }
    return D;
}

double **matrix_multiply(double** A, double** B, int n){
    int i, j, k;

    double **C = create_matrix(n, n);

    for(i = 0; i < n; i++){
        for(j = 0; j < n; j++){
            C[i][j] = 0;
            for(k = 0; k < n; k++){
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    return C;
}

double **normalized_similarity_matrix(double** A, double** D, int n){
    int i, j;
    double **D_half_inv = create_matrix(n, n);
    double **W = create_matrix(n, n);

    for(i = 0; i < n; i++){
        D_half_inv[i][i] = 1.0 / sqrt(D[i][i]);
    }

    W = matrix_multiply(D_half_inv, A, n);
    W = matrix_multiply(W, D_half_inv, n);

    return W;
}


