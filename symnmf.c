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

void free_matrix(double** mat){
    free(mat[0]);
    free(mat);
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

double **matrix_multiply(double** A, double** B, int n1, int n2, int n3){
    int i, j, k;

    double **C = create_matrix(n1, n3);

    for(i = 0; i < n1; i++){
        for(j = 0; j < n3; j++){
            C[i][j] = 0;
            for(k = 0; k < n2; k++){
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    return C;
}
double **matrix_subtract(double** A, double** B, int n, int d){
    int i, j;
    double **C = create_matrix(n, d);

    for(i = 0; i < n; i++){
        for(j = 0; j < d; j++){
            C[i][j] = A[i][j] - B[i][j];
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

    double** DA = matrix_multiply(D_half_inv, A, n, n, n);
    W = matrix_multiply(DA, D_half_inv, n, n, n);

    free_matrix(D_half_inv);
    free_matrix(DA);

    return W;
}
double average_matrix(double** mat, int n, int d){
    int i, j;
    double sum = 0;

    for(i = 0; i < n; i++){
        for(j = 0; j < d; j++){
            sum += mat[i][j];
        }
    }
    return sum / (n * d);
}

double **initialize_H(double** W, int n, int k){
    double **H = create_matrix(n, k);
    int i, j;
    double m = average_matrix(W, n, k);


    for(i = 0; i < n; i++){
        for(j = 0; j < k; j++){
            H[i][j] = (double)(rand() % 2 * sqrt(m / k));
        }
    }
    return H;
}
double **transpose_matrix(double** mat, int n, int d){
    int i, j;
    double **transposed = create_matrix(d, n);

    for(i = 0; i < n; i++){
        for(j = 0; j < d; j++){
            transposed[j][i] = mat[i][j];
        }
    }

    return transposed;
}

double **update_H(double** H, double **W, int n, int k){
    int i, j;
    double mul = 0;
    double **new_H = create_matrix(n, k);
    double beta = 0.5;

    double** WH = matrix_multiply(W,H,n,n,k);
    double** HT = transpose_matrix(H, n, k);
    double** HHT = matrix_multiply(H, HT, n, k, n);
    double** HHTH = matrix_multiply(HHT, H, n, n, k);

    for(i = 0; i < n; i++){
        for(j = 0; j < k; j++){
            new_H[i][j] += H[i][j] * (1 - beta + beta * (WH[i][j])/(HHTH[i][j]));
        }
    }
    free_matrix(WH);
    free_matrix(HT);
    free_matrix(HHT);
    free_matrix(HHTH);

    return new_H;
}

double frobenius_norm(double** A, int n, int k){
    int i, j;
    double norm = 0;

    for(i = 0; i < n; i++){
        for(j = 0; j < k; j++){
            norm += (A[i][j]) * (A[i][j]);
        }
    }
    return sqrt(norm);
}

double **convergence(double** W, int n, int k, double epsilon){
    double **H = initialize_H(W, n, k);
    double **new_H = update_H(H, W, n, k);
    double **old_H = H;

    double **HMinusH = matrix_subtract(new_H, old_H, n, k);

    while (forbenuis_norm(HMinusH, n, k) > epsilon){
        free_matrix(old_H);
        old_H = new_H;

        new_H = update_H(old_H, W, n, k);

        free_matrix(HMinusH);
        HMinusH = matrix_subtract(new_H, old_H, n, k);
    }
    free_matrix(HMinusH);
    free_matrix(old_H);

    return new_H;
}