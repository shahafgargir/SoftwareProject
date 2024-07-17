import numpy as np
import sys
import symnmfsp


def main():
    np.random.seed(0)
    arg_lst = sys.argv  # gets arguments from command line
    if len(arg_lst) != 4:
        print("An Error Has Occurred")
        return
    k, goal, file_name = int(arg_lst[1]), arg_lst[2], arg_lst[3]
    matrix = create_matrix_from_file(file_name)
    ret_matrix = get_matrix(goal, matrix, k)
    print_matrix(ret_matrix)


def create_matrix_from_file(file):  # read text from file and create points matrix
    with open(file, "r") as f:
        lines = f.readlines()
    points_list = []
    for line in lines:
        line = line.strip()
        if line:
            coordinates = list(map(float, line.split(',')))
            points_list.append(coordinates)
    points_matrix = np.array(points_list)
    return points_matrix


def get_matrix(goal, matrix, k): # return the right matrix according to the goal
    if goal == "symnmf":
        W = get_matrix("norm", matrix, k) # get norm matrix
        H = initialize_H(W, k)
        return np.array(symnmfsp.symnmf(W.tolist(), H.tolist(), k))
    elif goal == "sym":
        return np.array(symnmfsp.sym(matrix.tolist()))
    elif goal == "ddg":
        return np.array(symnmfsp.ddg(matrix.tolist()))
    elif goal == "norm":
        return np.array(symnmfsp.norm(matrix.tolist()))


def initialize_H(W, k): #initialize H matrix
    n = W.shape[0]
    m = np.mean(W)
    upper_bound = 2 * np.sqrt(m / k)
    return np.random.uniform(0, upper_bound, (n, k))

def print_matrix(matrix): # print the matrix according to
    for row in matrix:
        print(','.join(['%.4f' % elem for elem in row]))

if __name__ == "__main__":
    main()