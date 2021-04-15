from math import ceil
from typing import List, Tuple

try:
    import matplotlib.pyplot as plt
except ImportError:
    raise ImportError('Currently using matplotlib to plot results, please install')
    
import numpy as np
from _navipy import Polygon, PolygonSpace, Roadmap, build_prm
from navipy.testing import make_random_triangles

from reporting import (pretty_print_statistics, pretty_print_title,
                       profile_function)


def plot_triangles(ax, triangles: List[Polygon]):
    for triangle in triangles:
        ax.fill(triangle.points[:, 0], triangle.points[:, 1])


def plot_points(ax, points: np.ndarray):
    ax.plot(points[:, 0], points[:, 1], 'ob')


def profile_sampling(n_samples: int = 1_000, min_n_vertices: int = 1_000, xrange: Tuple[float, float] = (-10, 10),
                     yrange: Tuple[float, float] = (-10, 10), n_trials: int = 10, plot: bool = False):

    n_triangles = ceil(min_n_vertices / 3)
    triangles = make_random_triangles(n_triangles, xrange, yrange)

    search_space = PolygonSpace(triangles, xrange, yrange)

    def run_sample_free_space():
        return search_space.sample_free_space(n_samples)

    runtimes = profile_function(n_trials, run_sample_free_space)

    pretty_print_title(f'Profiling sampling: {n_samples} samples in space with {n_triangles * 3} vertices')
    pretty_print_statistics(runtimes)

    if plot:
        samples = run_sample_free_space()

        _, ax = plt.subplots()
        plot_points(ax, samples)
        plot_triangles(ax, triangles)
        plt.show()


def plot_connections(ax, roadmap: Roadmap):
    for state in roadmap.states:
        node = roadmap.node_at(state)
        for neighbor_state in node.neighbors:
            ax.plot([state[0], neighbor_state[0]], [state[1], neighbor_state[1]], '-b')


def profile_build_prm(n_samples: int = 100, n_batch: int = 10, k_neighbors: int = 10, min_n_vertices: int = 100,
                      xrange: Tuple[float, float] = (-10, 10), yrange: Tuple[float, float] = (-10, 10),
                      n_trials: int = 10, plot: bool = False):

    n_triangles = ceil(min_n_vertices / 3)
    triangles = make_random_triangles(n_triangles, xrange, yrange)

    search_space = PolygonSpace(triangles, xrange, yrange)

    runtimes = profile_function(n_trials, build_prm, (n_samples, n_batch, k_neighbors, search_space))

    pretty_print_title(
        f'Profiling building PRM in polygon space: {n_samples} samples in space with {n_triangles * 3} vertices')
    pretty_print_statistics(runtimes)

    if plot:
        roadmap = build_prm(n_samples, n_batch, k_neighbors, search_space)

        _, ax = plt.subplots()
        plot_points(ax, roadmap.states)
        plot_triangles(ax, triangles)
        plot_connections(ax, roadmap)
        plt.show()


if __name__ == '__main__':
    profile_sampling(plot=False)
    profile_build_prm(n_samples=1000, plot=False)
