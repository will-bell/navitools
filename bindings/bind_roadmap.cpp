#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/eigen.h>
#include "roadmap.hpp"

namespace py = pybind11;

struct WrapRoadmap {
    WrapRoadmap() {}

    Roadmap roadmap;

    void add_node(Eigen::Ref<const Eigen::VectorXd> state, Eigen::Ref<const Eigen::MatrixXd> neighbor_states, 
        Eigen::Ref<const Eigen::VectorXd> neighbor_costs) {
        
        roadmap.add_node(state, neighbor_states, neighbor_costs);
    }

    std::vector<RoadmapNode> nodes() {
        return roadmap.nodes();
    }

    RoadmapNode node_at(Eigen::Ref<const Eigen::VectorXd> state) {
        return roadmap.node_at(state);
    }

    RoadmapNode node_nearest(Eigen::Ref<const Eigen::VectorXd> state) {
        return roadmap.node_nearest(state);
    }

    std::vector<RoadmapNode> k_nodes_nearest(Eigen::Ref<const Eigen::VectorXd> state, int k) {
        return roadmap.k_nodes_nearest(state, k);
    }

    Eigen::VectorXd state_nearest(Eigen::Ref<const Eigen::VectorXd> state) {
        return roadmap.state_nearest(state);
    }

    Eigen::MatrixXd k_states_nearest(Eigen::Ref<const Eigen::VectorXd> state, int k) {
        return roadmap.k_states_nearest(state, k);
    }
};

void init_roadmap(py::module_& m)
{
    py::class_<RoadmapNode>(m, "RoadMapNode")
        .def(py::init<>())
        .def_property_readonly("state", &RoadmapNode::getState)
        .def_property_readonly("neighbors", &RoadmapNode::getNeighbors)
        .def_property_readonly("costs", &RoadmapNode::getCosts);

    py::class_<WrapRoadmap>(m, "RoadMap")
        .def(py::init<>())
        .def("add_node", &WrapRoadmap::add_node)
        .def("nodes", &WrapRoadmap::nodes)
        .def("node_at", &WrapRoadmap::node_at)
        .def("node_nearest", &WrapRoadmap::node_nearest)
        .def("k_nodes_nearest", &WrapRoadmap::k_nodes_nearest)
        .def("state_nearest", &WrapRoadmap::state_nearest)
        .def("k_states_nearest", &WrapRoadmap::k_states_nearest);

    py::register_exception<MissingStateRoadmapException>(m, "MissingStateRoadmapException");
}