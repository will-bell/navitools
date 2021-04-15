from random import choices
from typing import Tuple

import numpy as np
from navitools import PolygonSpace, build_prm, dijkstra


def test_dijkstra(xrange: Tuple[float, float] = (-10, 10), yrange: Tuple[float, float] = (-10, 10)):
    search_space = PolygonSpace([], xrange, yrange)

    roadmap = build_prm(100, 10, 10, search_space)
    start, goal = choices(roadmap.states, k=2)

    path = dijkstra(roadmap, start, goal)

    assert path.size
    assert np.all(path[0] == start)
    assert np.all(path[-1] == goal)
