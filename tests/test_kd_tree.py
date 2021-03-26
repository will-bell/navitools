from typing import List, Optional

import numpy as np
from _navipy import BadStateSizeException, EmptyTreeException, KD_Tree

from navipy.testing import generate_test_points


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


def square_distance(a: np.ndarray, b: np.ndarray) -> float:
    return (a - b).dot(a - b)


def test_tree_nearest_neighbor():
    for _ in range(1000):
        points = generate_test_points(n=100)

        test_point = np.array([0., 0.])

        # Brute force search for the true nearest neighbor
        nearest = points[0]
        nearest_distance = square_distance(test_point, nearest)
        for p in points[1:]:
            dist = square_distance(test_point, p)

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


class KBestLeaderboard:

    _value_list: List[float]

    _point_list: List[Optional[np.ndarray]]

    def __init__(self, k: int):
        self._k = k
        self._j = 0
        self._value_list = [np.inf] * k
        self._point_list = [None] * k

    def insert(self, value: float, node: np.ndarray) -> None:
        for i in range(self._k):
            if value < self._value_list[i]:
                self._value_list.insert(i, value)
                self._value_list.pop(-1)

                self._point_list.insert(i, node)
                self._point_list.pop(-1)

                self._j = min(self._k, self._j + 1)

                break

    def bottom_value(self) -> float:
        return self._value_list[-1]

    def bottom_point(self) -> np.ndarray:
        return self._point_list[self._j]

    def value_list(self) -> List[float]:
        return self._value_list[:self._j]

    def point_list(self) -> List[np.ndarray]:
        return self._point_list[:self._j]


def brute_force_k_nearest_neighbor(point: np.ndarray, point_list: np.ndarray, k: int) -> np.ndarray:
    leaderboard = KBestLeaderboard(k)

    leaderboard.insert(square_distance(point, point_list[0]), point_list[0])

    for p in point_list[1:]:
        dist = square_distance(point, p)

        if dist < leaderboard.bottom_value():
            leaderboard.insert(dist, p)

    return np.vstack(leaderboard.point_list())


def test_tree_k_nearest_neighbors(k: int = 10):
    for _ in range(1000):
        points = generate_test_points(n=100)

        test_point = np.array([0., 0.])

        k_nearest = brute_force_k_nearest_neighbor(test_point, points, k)

        # With the constructor
        tree = KD_Tree(points)

        tree_k_nearest = tree.k_nearest_neighbors(test_point, k)
        assert np.all(tree_k_nearest == k_nearest)

        # Building iteratively
        tree = KD_Tree(np.array([]))

        for point in points:
            tree.append_state(point)

        tree_k_nearest = tree.k_nearest_neighbors(test_point, k)
        assert np.all(tree_k_nearest == k_nearest)


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
