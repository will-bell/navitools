from _navipy import RoadMap
from test.common import generate_test_points
import numpy as np


def test_roadmap_instantiation():
    roadmap = RoadMap()

    assert not len(roadmap.nodes())


def test_add_nodes():
    points = generate_test_points(n=5)

    roadmap = RoadMap()

    roadmap.add_node(points[0], np.array([]), np.array([]))

    roadmap.add_node(points[1], np.array([points[0]]), np.array([1.]))

    roadmap.add_node(points[2], np.array([points[0], points[1]]), np.array([2., 3.]))

    roadmap.add_node(points[3], np.array([points[2]]), np.array([4.]))

    roadmap.add_node(points[4], np.array([points[3], points[2]]), np.array([5., 6.]))

    assert len(roadmap.nodes()) == 5
