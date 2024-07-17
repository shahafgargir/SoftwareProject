import numpy as np
import sys
import symnmf
import kmeans
from sklearn.metrics import silhouette_score


def main():
    np.random.seed(0)
    arg_lst = sys.argv  # gets arguments from command line
    if len(arg_lst) != 3:
        print("An Error Has Occurred")
        return
    k, file_name = int(arg_lst[1]), arg_lst[2]
    points_lst = create_points_from_file(file_name)
    matrix = np.array(points_lst)
    symnmf_matrix = symnmf.get_matrix("symnmf", matrix, k)
    symnmf_labels = labels_by_symnmfmatrix(symnmf_matrix)
    kmeans_center = kmeans.get_center(points_lst, k, 300)
    kmeans_labels = labels_by_center(points_lst, kmeans_center)
    kmeans_silhouette_score = silhouette_score(matrix, kmeans_labels)
    symnmf_silhouette_score = silhouette_score(matrix, symnmf_labels)
    print(f"nmf: {symnmf_silhouette_score:.4f}")
    print(f"kmeans: {kmeans_silhouette_score:.4f}")


def labels_by_center(points_lst, center):
    label = [0 for point in points_lst]
    for i in range(len(points_lst)):  # find closest center for each point and put them in groups
        j = kmeans.closest_index(points_lst[i], center)
        label[i] = j
    return np.array(label)


def create_points_from_file(file):  # read text from file and create points list
    with open(file, "r") as text:
        text = text.read()
        points_lst = text.split("\n")
        points_lst = points_lst[:-1]
        for i in range(len(points_lst)):
            points_lst[i] = points_lst[i].split(',')
            for j in range(len(points_lst[i])):
                points_lst[i][j] = float(points_lst[i][j])
            points_lst[i] = tuple(points_lst[i])
        return points_lst


def labels_by_symnmfmatrix(matrix):
    labels = []
    for row_idx, row in enumerate(matrix):
        cluster = np.argmax(row)
        labels.append(cluster)
    return labels


if __name__ == "__main__":
    main()
