#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>




struct data_struct{
    double **data;
    int length;
    int dimention;
};

void print_data_struct(struct data_struct data){
    /*
     this funcion gets any data_struct and print the data in table in format of 4 digits fraction
     */
    int i, j;
    
    for (i = 0; i < data.length; i++){
        for (j = 0; j < data.dimention; j++){
            printf("%.4f", data.data[i][j]);
            if(j != data.dimention - 1){
                printf(",");
            }
        }
        printf("\n");
    }
}

int get_dim(FILE *input){
    /*
     The function gets file structure, and by counting the nubmer of ',' in the first line return the dimention of the data.
     */
    
    int dim = 0;
    char c;
    do {
        c = getc(input);
        if (c == ','){
            dim++;
        }
    }while (c != '\n');
    rewind(input);
    if (dim == 0){
        return 0;
    }else{
        return dim + 1;
    }
    
}

struct data_struct create_data(double **data, int length, int dimention){
    struct data_struct data_struct;
    data_struct.data = data;
    data_struct.length = length;
    data_struct.dimention = dimention;
    return data_struct;
}

struct data_struct parse_file(const char *file_path){
    /*
     The function gets file path and return a structure of the data in it including the data, the length and the dimention of the observations
     */

    
    FILE *input = NULL;
    int dim;
    
    int mod = 0;
    int index = 0;
    double **data = (double **)calloc(1, sizeof(double *));
    int length = 1;
    double scaned_double;
    
    char c;
    
    struct data_struct return_data;
    
    input = fopen(file_path,"r");
    if (input == NULL){
        printf("An Error Has Occurred");
        exit(1);
    }
    dim = get_dim(input);
    if (dim == 0){
        printf("An Error Has Occurred");
        exit(1);
    }
    
    while(fscanf(input, "%lf", &scaned_double) == 1){
        if (mod == 0){
            if (index == length){
                data = (double **)realloc(data, length * 2 * sizeof(double *));
                if(data == NULL){
                    printf("An Error Has Occurred");
                    exit(1);
                }
                length *= 2;
            }
            data[index] = (double *)calloc(dim, sizeof(double));
            if(data[index] == NULL){
                printf("An Error Has Occurred");
                exit(1);
            }
        }
        data[index][mod] = scaned_double;
        
        if(mod == dim - 1){
            index++;
        }
        mod = (mod + 1) % dim;
        c = getc(input);
        if((c != '\n') & (c != ',') & (c != EOF)){
            printf("An Error Has Occurred");
            exit(1);
        }
    }
    fclose(input);
    
    return_data.data = data;
    return_data.length = index;
    return_data.dimention = dim;
    
    return return_data;
}


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


struct data_struct similar_matrix(double** mat, int n, int d){
    
    double **A = create_matrix(n, n);
    int i, j;
    struct data_struct data;

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
    data = create_data(A, n , n);
    return data;
}

struct data_struct sym(struct data_struct data){
    return similar_matrix(data.data, data.length, data.dimention);
}


struct data_struct diaognal_degree_matrix(double** mat, int n, int d){
    int i, j;
    double **A = similar_matrix(mat, n ,d).data;
    double **D = create_matrix(n, n);
    struct data_struct data;

    for(i = 0; i < n; i++){
        for(j = 0; j < n; j++){
            D[i][i] += A[i][j];
        }
    }
    data = create_data(D, n, n);
    return data;
}


struct data_struct ddg(struct data_struct data){
    return diaognal_degree_matrix(data.data, data.length, data.dimention);
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

struct data_struct normalized_similarity_matrix(double** mat, int n, int d){
    struct data_struct data; 
    int i;
    double** DA;
    double **A = similar_matrix(mat, n, d).data;
    double **D = diaognal_degree_matrix(mat, n, d).data;
    double **D_half_inv = create_matrix(n, n);
    double **W = create_matrix(n, n);

    for(i = 0; i < n; i++){
        D_half_inv[i][i] = 1.0 / sqrt(D[i][i]);
    }
    DA = matrix_multiply(D_half_inv, A, n, n, n);

    W = matrix_multiply(DA, D_half_inv, n, n, n);
    free_matrix(D_half_inv);
    free_matrix(DA);

    data = create_data(W, n, n);
    return data;
}

struct data_struct norm(struct data_struct data){
    return normalized_similarity_matrix(data.data, data.length, data.dimention);
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
    /*double mul = 0;*/
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

struct data_struct symnmf(double** W, double** H, int k, int n){
    double **HMinusH;
    struct data_struct data;
    double **new_H = update_H(H, W, n, k);
    double **old_H = H;
    int iter;
    iter = 1;

    HMinusH = matrix_subtract(new_H, old_H, n, k);

    while (pow(frobenius_norm(HMinusH, n, k),2) > pow(10, -4) && iter <= 300){
        free_matrix(old_H);
        old_H = new_H;

        new_H = update_H(old_H, W, n, k);

        free_matrix(HMinusH);
        HMinusH = matrix_subtract(new_H, old_H, n, k);
        iter += 1;
    }
    free_matrix(HMinusH);
    free_matrix(old_H);

    data = create_data(new_H, n, k);
    return data;
}

void free_data_struct(struct data_struct data){
    /*
     get data structure and free the 2 dimention array of the data in it.
     */
    int i;
    
    for (i = 0; i<data.length; i++){
        free(data.data[i]);
    }
    free(data.data);
}


int main(int argc, char** argv){
    char* goal, *file_name;
    struct data_struct data, new_data;
    if(argc != 3){
        printf("An Error Has Occurred");
        exit(1);
    }
    goal = argv[1];
    file_name = argv[2];
    data = parse_file(file_name);
    if (strcmp(goal,"sym") == 0){
        new_data = similar_matrix(data.data, data.length, data.dimention);
    }
    else if(strcmp(goal,"ddg") == 0){
        new_data = diaognal_degree_matrix(data.data, data.length, data.dimention);
    }

    else if(strcmp(goal,"norm") == 0){
        new_data = normalized_similarity_matrix(data.data, data.length, data.dimention);
        }
    else{
        printf("An Error Has Occurred");
        exit(1);
    }  
    print_data_struct(new_data);
    free_data_struct(data);
    free_matrix(new_data.data);
    return 0;
}
