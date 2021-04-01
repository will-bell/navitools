#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/eigen.h>
#include "roadmap.hpp"
#include "search_space.hpp"
#include "probabilistic_roadmap.hpp"

namespace py = pybind11;

void init_roadmap(py::module_& m)
{
    py::class_<RoadmapNode>(m, "RoadMapNode")
        .def(py::init<>())
        .def_property_readonly("state", &RoadmapNode::getState)
        .def_property_readonly("neighbors", &RoadmapNode::getNeighbors)
        .def_property_readonly("costs", &RoadmapNode::getCosts);

    py::class_<Roadmap>(m, "RoadMap")
        .def(py::init<>())
        .def("add_node", &Roadmap::add_node)
        .def("nodes", &Roadmap::nodes)
        .def("node_at", &Roadmap::node_at)
        .def("node_nearest", &Roadmap::node_nearest)
        .def("k_nodes_nearest", &Roadmap::k_nodes_nearest)
        .def("state_nearest", &Roadmap::state_nearest)
        .def("k_states_nearest", &Roadmap::k_states_nearest);

    m.def("build_prm", &build_prm);
    m.def("testing_polygon_space_prm", &testing_polygon_space_prm);

    py::register_exception<MissingStateRoadmapException>(m, "MissingStateRoadmapException");
}