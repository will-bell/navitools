from typing import List

import numpy as np
from navitools import Polygon


def plot_triangles(ax, triangles: List[Polygon]):
    for triangle in triangles:
        ax.fill(triangle.points[:, 0], triangle.points[:, 1])


def plot_points(ax, points: np.ndarray):
    ax.plot(points[:, 0], points[:, 1], 'ob')

    
def plot_connections(ax, roadmap):
    for state in roadmap.states:
        node = roadmap.node_at(state)
        for neighbor_state in node.neighbors:
            ax.plot([state[0], neighbor_state[0]], [state[1], neighbor_state[1]], '-b')


def plot_roadmap(ax, roadmap):
    plot_points(ax, roadmap.states)
    plot_connections(ax, roadmap)
