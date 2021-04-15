import numpy as np
from navipy import Polygon, inside_polygon, segments_intersect


def test_polygon_instantiation():
    points = np.array([[0., 0.], [1., 0.], [1., 1.], [0., 1.]])
    polygon = Polygon(points)

    assert np.all(points == polygon.points)

    for i in range(polygon.n_points):
        assert np.all(points[i] == polygon[i])

    assert polygon.is_solid

    assert np.all(polygon[4] == points[0])
    assert np.all(polygon[-1] == points[-1])

    points = np.array([[0., 0.], [0., 1.], [1., 1.], [1., 0.]])
    polygon = Polygon(points)

    assert polygon.is_hollow


def test_inside_polygon():
    points = np.array([[0., 0.], [1., 0.], [1., 1.], [0., 1.]])
    polygon = Polygon(points)

    assert inside_polygon(np.array([.5, .5]), polygon)
    assert not inside_polygon(np.array([-.5, -.5]), polygon)
    assert not inside_polygon(np.array([-.5, 1.5]), polygon)
    assert not inside_polygon(np.array([1.5, 1.5]), polygon)
    assert not inside_polygon(np.array([1.5, -.5]), polygon)

    points = np.array([[0., 0.], [0., 1.], [1., 1.], [1., 0.]])
    polygon = Polygon(points)

    assert not inside_polygon(np.array([.5, .5]), polygon)
    assert inside_polygon(np.array([-.5, -.5]), polygon)
    assert inside_polygon(np.array([-.5, 1.5]), polygon)
    assert inside_polygon(np.array([1.5, 1.5]), polygon)
    assert inside_polygon(np.array([1.5, -.5]), polygon)


def test_segments_intersect():
    ab = (np.array([0., 0.]), np.array([1., 1.]))
    cd = (np.array([1., 0.]), np.array([0., 1.]))
    assert segments_intersect(ab, cd) == 1

    ab = (np.array([0., 0.]), np.array([.5, .5]))
    cd = (np.array([1., 0.]), np.array([0., 1.]))
    assert segments_intersect(ab, cd) == 1

    ab = (np.array([0., 1.]), np.array([1., 1.]))
    cd = (np.array([0., 0.]), np.array([1., 0.]))
    assert segments_intersect(ab, cd) == 0

    ab = (np.array([0., 1.]), np.array([1., 1.]))
    cd = (np.array([0., 1.]), np.array([.5, 1.]))
    assert segments_intersect(ab, cd) == 2

    ab = (np.array([0., 1.]), np.array([.4, 1.]))
    cd = (np.array([.5, 1.]), np.array([1., 1.]))
    assert segments_intersect(ab, cd) == 0
