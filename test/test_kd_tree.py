from _navipy import KD_Tree, BadStateSizeException, EmptyTreeException
from typing import List
import numpy as np
import math


def generate_test_points(n: int = 1_000, min_val: float = -100, max_val: float = 100) -> np.ndarray:
    return np.random.uniform(min_val, max_val, (n, 2))


def test_build_tree():
    points = generate_test_points(n=100)

    # With the constructor
    tree = KD_Tree(points)

    assert tree.count_states() == 100

    # Building iteratively
    tree = KD_Tree(np.array([]))

    for point in points:
        tree.append_state(point)

    assert tree.count_states() == 100


def test_tree_nearest_neighbor():
    points = generate_test_points(n=1_000)

    test_point = [0., 0.]

    def distance(point: List[float]) -> float:
        return math.sqrt((test_point[0] - point[0])**2 + (test_point[1] - point[1])**2)

    # Brute force search for the true nearest neighbor
    nearest = points[0]
    nearest_distance = distance(nearest)
    for p in points[1:]:
        dist = distance(p)

        if dist < nearest_distance:
            nearest = p
            nearest_distance = dist

    # With the constructor
    tree = KD_Tree(points)

    tree_nearest = tree.nearest_neighbor(test_point)
    assert np.all(tree_nearest == nearest)

    # Building iteratively
    tree = KD_Tree(np.array([]))

    for point in points:
        tree.append_state(point)

    tree_nearest = tree.nearest_neighbor(test_point)
    assert np.all(tree_nearest == nearest)


def test_tree_exceptions():
    points = generate_test_points(n=10)
    tree = KD_Tree(points)

    try:
        tree.nearest_neighbor(np.array([1., 2., 3.]))
        assert False, "Failed to catch BadStateSizeException"

    except(BadStateSizeException):
        pass

    try:
        tree.append_state(np.array([1., 2., 3.]))
        assert False, "Failed to catch BadStateSizeException"

    except(BadStateSizeException):
        pass

    tree = KD_Tree(np.array([]))

    try:
        tree.nearest_neighbor(np.array([1., 2.]))
        assert False, "Failed to catch EmptyTreeException"

    except EmptyTreeException:
        pass
