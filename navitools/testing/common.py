from math import cos, pi, radians, sin, sqrt
from random import uniform
from typing import List, Tuple

import numpy as np
from navitools import Polygon


def generate_test_points(n: int = 1_000, size: int = 2, min_val: float = -100, max_val: float = 100) -> np.ndarray:
    return np.random.uniform(min_val, max_val, (n, size))


_SIDE_LENGTH = sqrt(2 - 2 * cos(radians(120)))
_HALF_SIDE_LENGTH = _SIDE_LENGTH / 2
_LEG_LENGTH = sqrt(1 - _HALF_SIDE_LENGTH**2)
_BASE_TRIANGLE = np.array([[-_HALF_SIDE_LENGTH, -_LEG_LENGTH], [_HALF_SIDE_LENGTH, -_LEG_LENGTH], [0, 1]])


def rotation_matrix(angle: float) -> np.ndarray:
    return np.array([[cos(angle), -sin(angle)], [sin(angle), cos(angle)]])


def make_random_triangles(n: int, xrange: Tuple[float, float] = (0, 50), yrange: Tuple[float, float] = (0, 50)) \
        -> List[Polygon]:
    assert xrange[1] - xrange[0] > 2., \
        'Ranges need to be sufficiently large for the fixed size equilateral triangles, i.e. > 2'
    assert yrange[1] - yrange[0] > 2., \
        'Ranges need to be sufficiently large for the fixed size equilateral triangles, i.e. > 2'
    mod_xrange = (xrange[0] + 1., xrange[1] - 1.)
    mod_yrange = (yrange[0] + 1., yrange[1] - 1.)

    triangles = []
    for _ in range(n):
        angle = uniform(-pi / 2, pi / 2)
        rot = rotation_matrix(angle)

        points = np.einsum('ij,kj->ki', rot, _BASE_TRIANGLE) + np.array([uniform(*mod_xrange), uniform(*mod_yrange)])
        triangles.append(Polygon(points))

    return triangles
