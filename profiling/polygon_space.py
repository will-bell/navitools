import time
from math import ceil
from typing import List, Tuple

import matplotlib.pyplot as plt
import numpy as np
from _navipy import Polygon, PolygonSpace
from navipy.testing import make_random_triangles


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
