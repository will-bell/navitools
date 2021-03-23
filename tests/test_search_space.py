import numpy as np
from _navipy import Polygon, PolygonSpace, inside_polygon


def test_search_space_instantiation():
    square = Polygon(np.array([[0., 0.], [1., 0.], [1., 1.], [0., 1.]]))
    triangle = Polygon(np.array([[2., 2.], [4., 2.], [3., 4.]]))

    PolygonSpace([square, triangle], (0, 5), (0, 5))


def test_search_space_sample_free_sapce(n: int = 100_000):
    square = Polygon(np.array([[0., 0.], [1., 0.], [1., 1.], [0., 1.]]))
    triangle = Polygon(np.array([[2., 2.], [4., 2.], [3., 4.]]))

    space = PolygonSpace([square, triangle], (0, 5), (0, 5))

    samples = space.sample_free_space(n)
    for sample in samples:
        assert not inside_polygon(sample, square)
        assert not inside_polygon(sample, triangle)
