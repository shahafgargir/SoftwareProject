#include <stdio.h>


struct data_struct{
    double **data;
    int length;
    int dimention;
};

struct data_struct create_data(double **data, int length, int dimention);

void print_data_struct(struct data_struct data);

int get_dim(FILE *input);

struct data_struct parse_file(const char *file_path);

double distance(double* first_vec, double* second_vec, int d);
double **create_matrix(int n, int d);

void free_matrix(double** mat);


struct data_struct similar_matrix(double** mat, int n, int d);

struct data_struct sym(struct data_struct data);
void free_data_struct(struct data_struct data);
struct data_struct diaognal_degree_matrix(double** mat, int n, int d);
struct data_struct ddg(struct data_struct data);
double **matrix_multiply(double** A, double** B, int n1, int n2, int n3);
double **matrix_subtract(double** A, double** B, int n, int d);
struct data_struct normalized_similarity_matrix(double** mat, int n, int d);
struct data_struct norm(struct data_struct data);
double average_matrix(double** mat, int n, int d);
double **initialize_H(double** W, int n, int k);
double **transpose_matrix(double** mat, int n, int d);
double **update_H(double** H, double **W, int n, int k);
double frobenius_norm(double** A, int n, int k);
struct data_struct symnmf(double** W, double** H, int k, int n);
