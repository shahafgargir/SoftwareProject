#include <math.h>


int distance(int* first_vec, int* second_vec){
    int n = sizeof(first_vec)/ sizeof(int);
    int i, dif, sum;
    sum = 0;
    for(i = 0; i < n; i++){
        dif = first_vec[i] - second_vec[i];
        sum += dif*dif;
    }
    return sum;
}


int **similar_matrix(int** mat, int n, int d){
    int *p;
    int **a;
    int i, j;
    p = (int*)calloc(n*n, sizeof(int));
    assert(p != NULL);
    a = (int**)calloc(n, sizeof(int *));
    assert(a!= NULL);
    for(i = 0; i < n; i++){
        a[i] = p + i*n;
    }
    for(i = 0; i <n; i++){
        for(j = 0; j < n; j++){
            if(i == j){
                a[i][j] = 0;
            }
            else{
                a[i][j] = exp(-(distance(mat[i], mat[j]))/2);
            }
        }
    }
    return a;
}