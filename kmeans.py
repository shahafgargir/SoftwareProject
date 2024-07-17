import math
import sys


def main():
    arg_lst = sys.argv  # gets arguments from command line
    legality, k, N, d, iter, file = check_arguments(arg_lst)  # check legality of arguments
    if not legality:
        return
    points_lst = create_points_from_file(file)  # create points from file path
    n = len(points_lst)
    if int(k) >= n:
        print("Invalid number of clusters!")
        return
    center_lst = get_center(points_lst, k, iter)  # get final list of centers
    print_center(center_lst)  # print centers by format
    return


def check_arguments(args):  # checks legality of arguments and return them
    if len(args) != 5 and len(args) != 6:  # check number of arguments
        print("An Error Has Occurred")
        return False, 0, 0, 0, 0, 0
    if len(args) == 5:
        k = args[1]
        N = args[2]
        d = args[3]
        file = args[4]
        iter = 200  # initialize iter to 200 if not exist
    else:
        k = args[1]
        N = args[2]
        d = args[3]
        iter = args[4]
        file = args[5]
        if not iter.isnumeric() or int(iter) <= 1 or int(iter) >= 1000:  # check legality of iter
            print("Invalid maximum iteration!")
            return False, 0, 0, 0, 0, 0
    if not N.isnumeric() or int(N) <= 1:
        print("Invalid number of points!")
        return False, 0, 0, 0, 0, 0
    if not k.isnumeric() or int(k) <= 1 or int(k) >= int(N):  # check legality of k
        print("Invalid number of clusters!")
        return False, 0, 0, 0, 0, 0
    if not d.isnumeric() or int(d) <= 0:
        print("Invalid dimension of point!")
        return False, 0, 0, 0, 0, 0
    return True, int(k), int(N), int(d), int(iter), file  # return arguments with casting


def create_points_from_file(file):  # read text from file and create points list
    text = open(file, "r")
    text = text.read()
    points_lst = text.split("\n")
    points_lst = points_lst[:-1]
    for i in range(len(points_lst)):
        points_lst[i] = points_lst[i].split(',')
        for j in range(len(points_lst[i])):
            points_lst[i][j] = float(points_lst[i][j])
        points_lst[i] = tuple(points_lst[i])
    return points_lst


def distance(a, b):  # calculate distance between 2 points
    sum = 0
    for i in range(len(a)):
        sum += (a[i] - b[i]) ** 2
    return math.sqrt(sum)


def closest_index(x, lst):  # find index of the closest center to point x
    index = 0
    min_distance = sys.float_info.max
    for i in range(len(lst)):
        dist = distance(x, lst[i])
        if dist < min_distance:
            index = i
            min_distance = dist
    return index


def avg(lst):  # calculate centroid of points
    n = len(lst)
    a = lst[0]
    avarage = [0 for k in range(len(a))]
    for i in range(len(a)):
        for j in range(n):
            avarage[i] += lst[j][i]
        avarage[i] = avarage[i] / n
    return tuple(avarage)


def epsilon_change(center, last_center):  # checks if center changes enough
    e = 0.001
    for i in range(len(center)):
        if distance(center[i], last_center[i]) < e:
            continue
        return True
    return False


def get_center(points_lst, k, iter):  # calculate final centers given a list of points, k and number of iterations
    last_center = points_lst[:k]  # initialize center
    center = points_lst[:k]
    clusters = [[] for i in range(k)]
    for iteration in range(iter):  # update centers for each iteration
        
        for point in points_lst:  # find closest center for each point and put them in groups
            i = closest_index(point, last_center)
            clusters[i] += [point]
        for j in range(k):
            center[j] = avg(clusters[j])  # find new center of the groups
        if not epsilon_change(center, last_center):  # checks if center changes enough
            break
    
        for m in range(k):
            last_center[m] = center[m]
            clusters[m] = []
    return center  # return final center


def print_center(center):  # print centers in format
    for point in center:
        for i in range(len(point) - 1):
            print('{:.4f}'.format(point[i]), end=",")
        print('{:.4f}'.format(point[-1]))


if __name__ == "__main__":
    main()
