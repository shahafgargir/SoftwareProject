import numpy as np
import sys
import mysymnmfsp

np.random.seed(0)


def main():
    np.random.seed(0)
    arg_lst = sys.argv  # gets arguments from command line
    if len(arg_lst) != 4:
        print("An Error Has Occurred")
        return
    k, goal, file_name = arg_lst[1], arg_lst[2], arg_lst[3]
    matrix = create_matrix_from_file(file_name)
    ret_matrix = get_matrix(goal, matrix, k)
    print_matrix(ret_matrix)


def create_matrix_from_file(file):  # read text from file and create points list
    text = open(file, "r")
    text = text.read()
    points_lst = text.split("\n")
    points_lst = points_lst[:-1]
    for i in range(len(points_lst)):
        points_lst[i] = points_lst[i].split(',')
        for j in range(len(points_lst[i])):
            points_lst[i][j] = float(points_lst[i][j])
        points_lst[i] = tuple(points_lst[i])
    return np.array(points_lst)


def get_matrix(goal, matrix, k):
    if goal == "symnmf":
        W = get_matrix("norm", matrix, k)
        H = initialize_H(W, k)
        return mysymnmfsp.symnmf(W, H, k)
    elif goal == "sym":
        return mysymnmfsp.sym(matrix)
    elif goal == "ddg":
        return mysymnmfsp.ddg(matrix)
    elif goal == "norm":
        return mysymnmfsp.norm(matrix)


def distance(first_vec, second_vec):
    return np.sum((first_vec - second_vec) ** 2)


def create_matrix(n, d):
    return np.zeros((n, d))


def similar_matrix(X):
    n = X.shape[0]
    A = np.zeros((n, n))
    for i in range(n):
        for j in range(n):
            if i != j:
                A[i][j] = np.exp(-distance(X[i], X[j]) / 2)
    return A


def diagonal_degree_matrix(A):
    return np.diag(np.sum(A, axis=1))


def normalized_similarity_matrix(A, D):
    D_half_inv = np.linalg.inv(np.sqrt(D))
    W = np.dot(np.dot(D_half_inv, A), D_half_inv)
    return W


def initialize_H(W, k):
    n = W.shape[0]
    m = np.mean(W)
    upper_bound = np.sqrt(m / k)
    return np.random.uniform(0, upper_bound, (n, k))


def update_H(H, W):
    beta = 0.5
    WH = W @ H
    HHTH = H @ H.T @ H

    return H * (1 - beta + beta * WH / HHTH)


def convergence(W, k, epsilon):
    H = initialize_H(W, k)

    new_H = update_H(H, W)
    old_H = H

    while np.linalg.norm(new_H - old_H) > epsilon:
        old_H = new_H
        new_H = update_H(old_H, W)

    return new_H

def print_matrix(matrix):
    for row in matrix:
        print(','.join(['%.4f' % elem for elem in row]))

if __name__ == "__main__":
    main()