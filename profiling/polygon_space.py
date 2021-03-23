from _navipy import PolygonSpace, Polygon
import numpy as np
from typing import Tuple, List
from math import sqrt, sin, cos, radians, pi, ceil
from random import uniform
import matplotlib.pyplot as plt
import time

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
        angle = uniform(-pi/2, pi/2)
        rot = rotation_matrix(angle)

        points = np.einsum('ij,kj->ki', rot, _BASE_TRIANGLE) + np.array([uniform(*mod_xrange), uniform(*mod_yrange)])
        triangles.append(Polygon(points))

    return triangles


def plot_triangles(ax, triangles: List[Polygon]):
    for triangle in triangles:
        ax.fill(triangle.points[:, 0], triangle.points[:, 1])


def plot_result(samples: np.ndarray, triangles: List[Polygon]):
    _, ax = plt.subplots()
    ax.plot(samples[:, 0], samples[:, 1], 'ob')
    plot_triangles(ax, triangles)
    plt.show()


def profile_sampling_polygon_space(n_samples: int = 1_000, min_n_vertices: int = 1_000,
        xrange: Tuple[float, float] = (0, 10), yrange: Tuple[float, float] = (0, 10), n_trials: int = 10):

    n_triangles = ceil(min_n_vertices / 3)
    triangles = make_random_triangles(n_triangles, xrange, yrange)

    space = PolygonSpace(triangles, xrange, yrange)

    times = []
    for _ in range(n_trials):
        start = time.time()
        samples = space.sample_free_space(n_samples)
        times.append(time.time() - start)

    times = np.array(times)
    print(f'{n_samples} samples in space with {n_triangles * 3} vertices:')
    print('----------------------------------------------------------------------')
    print(f'    Average: {times.mean()}')
    print(f'    Standard deviation: {times.std()}')
    print(f'    Median: {np.median(times)}')
    print(f'    Min: {times.min()}')
    print(f'    Max: {times.max()}')

    plot_result(samples, triangles)


if __name__ == '__main__':
    profile_sampling_polygon_space(1_000, 1_000, (0, 20), (0, 20))
