#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/eigen.h>
#include "roadmap.hpp"
#include "search_space.hpp"
#include "probabilistic_roadmap.hpp"

namespace py = pybind11;

void init_roadmap(py::module_& m)
{
    py::class_<RoadmapNode>(m, "RoadmapNode")
        .def(py::init<>())
        .def_property_readonly("state", &RoadmapNode::get_state)
        .def_property_readonly("neighbors", &RoadmapNode::get_neighbors)
        .def_property_readonly("costs", &RoadmapNode::get_costs);

    py::class_<Roadmap>(m, "Roadmap")
        .def(py::init<int>())
        .def_property_readonly("state_size", &Roadmap::get_state_size)
        .def_property_readonly("n_states", &Roadmap::get_n_states)
        .def_property_readonly("nodes", &Roadmap::get_nodes)
        .def_property_readonly("states", &Roadmap::get_states)
        .def("add_node", &Roadmap::add_node)
        .def("node_at", &Roadmap::node_at)
        .def("node_nearest", &Roadmap::node_nearest)
        .def("k_nodes_nearest", &Roadmap::k_nodes_nearest)
        .def("state_nearest", &Roadmap::state_nearest)
        .def("k_states_nearest", &Roadmap::k_states_nearest);

    m.def("build_prm", &build_prm);
    m.def("testing_polygon_space_prm", &testing_polygon_space_prm);

    py::register_exception<MissingStateRoadmapException>(m, "MissingStateRoadmapException");
}