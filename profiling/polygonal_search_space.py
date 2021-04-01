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


def pretty_print_title(title: str):
    title_length = len(title)
    hbar = '-' * title_length

    print(
        '\n'
        f'{title}\n'
        f'{hbar}'
    )


def pretty_print_statistics(runtimes: np.ndarray):
    print(
        f'    Average: {runtimes.mean()}\n'
        f'    Standard deviation: {runtimes.std()}\n'
        f'    Median: {np.median(runtimes)}\n'
        f'    Min: {np.min(runtimes)}\n'
        f'    Max: {np.max(runtimes)}\n'
    )


def profile_sampling(n_samples: int = 1_000, min_n_vertices: int = 1_000, xrange: Tuple[float, float] = (0, 10),
                     yrange: Tuple[float, float] = (0, 10), n_trials: int = 10):

    n_triangles = ceil(min_n_vertices / 3)
    triangles = make_random_triangles(n_triangles, xrange, yrange)

    space = PolygonSpace(triangles, xrange, yrange)

    runtimes = []
    samples = np.array([])
    for _ in range(n_trials):
        start = time.time()
        samples = space.sample_free_space(n_samples)

        runtimes.append(time.time() - start)

    runtimes = np.array(runtimes)

    pretty_print_title(f'Profiling sampling: {n_samples} samples in space with {n_triangles * 3} vertices')
    pretty_print_statistics(runtimes)

    plot_result(samples, triangles)


def profile_build_prm(n_samples: int = 1_000, n_batch: int = 10, k_neighbors: int = 10,
                      xrange: Tuple[float, float] = (0, 10), yrange: Tuple[float, float] = (0, 10), n_trials: int = 10):
    pass


if __name__ == '__main__':
    profile_sampling(1_000, 1_000, (0, 20), (0, 20))
