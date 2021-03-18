import numpy as np
from _navipy import RoadMap

from tests.common import generate_test_points


def test_roadmap_instantiation():
    roadmap = RoadMap()

    assert not len(roadmap.nodes())


def test_add_nodes_with_known_neighbors():
    points = generate_test_points(n=5)

    roadmap = RoadMap()

    roadmap.add_node(points[0], np.array([]), np.array([]))
    roadmap.add_node(points[1], np.array([points[0]]), np.array([1.]))
    roadmap.add_node(points[2], np.array([points[0], points[1]]), np.array([2., 3.]))
    roadmap.add_node(points[3], np.array([points[2]]), np.array([4.]))
    roadmap.add_node(points[4], np.array([points[2], points[3]]), np.array([5., 6.]))

    assert len(roadmap.nodes()) == 5

    node0 = roadmap.node_at(points[0])
    assert np.all(node0.state == points[0])
    assert np.all(node0.neighbors == np.array([points[1], points[2]]))
    assert np.all(node0.costs == np.array([1., 2.]))

    node1 = roadmap.node_at(points[1])
    assert np.all(node1.state == points[1])
    assert np.all(node1.neighbors == np.array([points[0], points[2]]))
    assert np.all(node1.costs == np.array([1., 3.]))

    node2 = roadmap.node_at(points[2])
    assert np.all(node2.state == points[2])
    assert np.all(node2.neighbors == np.array([points[0], points[1], points[3], points[4]]))
    assert np.all(node2.costs == np.array([2., 3., 4., 5.]))

    node3 = roadmap.node_at(points[3])
    assert np.all(node3.state == points[3])
    assert np.all(node3.neighbors == np.array([points[2], points[4]]))
    assert np.all(node3.costs == np.array([4., 6.]))

    node4 = roadmap.node_at(points[4])
    assert np.all(node4.state == points[4])
    assert np.all(node4.neighbors == np.array([points[2], points[3]]))
    assert np.all(node4.costs == np.array([5., 6.]))


def test_add_nodes_with_discovered_neighbors(k: int = 5):
    points = generate_test_points(n=101)

    new_state = points[0]
    states = points[1:]

    roadmap = RoadMap()

    for state in states:
        roadmap.add_node(state, np.array([]), np.array([]))

    # Make lists of the nearest states and nearest nodes
    nearest_states = roadmap.k_states_nearest(new_state, k)
    nearest_nodes = roadmap.k_nodes_nearest(new_state, k)

    # Assert these lists are the same
    for node, state in zip(nearest_nodes, nearest_states):
        assert np.all(node.state == state)

    # Get the distances to the neighbors and make them costs
    dists = nearest_states - new_state
    costs = np.einsum('ij,ij->i', dists, dists)

    # Add this new node to the roadmap
    roadmap.add_node(new_state, nearest_states, costs)

    # Assert we can find this new node in the map
    new_node = roadmap.node_at(new_state)
    assert np.all(new_node.state == new_state)
    assert np.all(new_node.neighbors == nearest_states)
    assert np.all(new_node.costs == costs)

    # Assert that the neighbors really are the closest in the roadmap
    all_dists = states - new_state
    all_costs = np.einsum('ij,ij->i', all_dists, all_dists)
    all_costs_inds = all_costs.argsort()
    all_costs_ascnd = all_costs[all_costs_inds]
    assert np.all(costs == all_costs_ascnd[:k])
