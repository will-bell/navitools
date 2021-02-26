from _navipy import KD_Tree, Vector2
from typing import List
import numpy as np
import math


def generate_test_points(n: int = 1_000, min_val: float = -100, max_val: float = 100) -> List[Vector2]:
    return [list(point) for point in np.random.uniform(min_val, max_val, (n, 2))]


def test_build_tree():
    point_list = generate_test_points(n=1_000)
    tree = KD_Tree(point_list)

    assert tree.count_states() == 1_000


def test_tree_nearest_neighbor():
    point_list = generate_test_points(n=1_000)

    tree = KD_Tree(point_list)

    test_point = [0., 0.]

    def distance(point: List[float]) -> float:
        return math.sqrt((test_point[0] - point[0])**2 + (test_point[1] - point[1])**2)

    # Brute force search for the true nearest neighbor
    nearest = point_list[0]
    nearest_distance = distance(nearest)
    for p in point_list[1:]:
        dist = distance(p)

        if dist < nearest_distance:
            nearest = p
            nearest_distance = dist

    # KD Tree nearest neighbor search
    tree_nearest = tree.nearest_neighbor(test_point)

    assert tree_nearest == nearest
