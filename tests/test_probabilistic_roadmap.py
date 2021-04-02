from math import ceil
from typing import Tuple

from _navipy import PolygonSpace, build_prm, inside_polygon, segments_intersect
from navipy.testing import make_random_triangles


def test_build_prm(n_samples: int = 100, n_batch: int = 10, k_neighbors: int = 10, min_n_vertices: int = 100,
                   xrange: Tuple[float, float] = (-10, 10), yrange: Tuple[float, float] = (-10, 10)):

    n_triangles = ceil(min_n_vertices / 3)
    triangles = make_random_triangles(n_triangles, xrange, yrange)

    search_space = PolygonSpace(triangles, xrange, yrange)

    roadmap = build_prm(n_samples, n_batch, k_neighbors, search_space)

    for node in roadmap.nodes:
        for triangle in triangles:
            # Assert no node is inside a polygon
            assert not inside_polygon(node.state, triangle)

            for i in range(3):
                a = triangle[i]
                b = triangle[i + 1]

                for neighbor_state in node.neighbors:
                    # Assert no transition between states collides with a polygon
                    assert not segments_intersect((node.state, neighbor_state), (a, b))
