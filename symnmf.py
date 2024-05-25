import numpy as np

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
    return np.random.rand(n, k) * np.sqrt(m / k)

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
