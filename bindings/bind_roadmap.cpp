#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/eigen.h>
#include "roadmap.hpp"

namespace py = pybind11;

struct WrapRoadmap {
    WrapRoadmap() {}

    void add_node(Eigen::Ref<const Eigen::VectorXd> state, Eigen::Ref<const Eigen::MatrixXd> neighbor_states, 
        Eigen::Ref<const Eigen::VectorXd> neighbor_costs) {
        
        roadmap.add_node(state, neighbor_states, neighbor_costs);
    }

    std::vector<RoadmapNode> nodes() {
        return roadmap.nodes();
    }

    Roadmap roadmap;
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
        .def("nodes", &WrapRoadmap::nodes);

    py::register_exception<MissingStateRoadmapException>(m, "MissingStateRoadmapException");
}