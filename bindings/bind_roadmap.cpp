#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/eigen.h>
#include "roadmap.hpp"
#include "search_space.hpp"
#include "probabilistic_roadmap.hpp"

namespace py = pybind11;

// struct WrapRoadmap {
//     WrapRoadmap() {}
    
//     WrapRoadmap(const Roadmap& roadmap_to_wrap) {
//         roadmap = roadmap_to_wrap;
//     }

//     Roadmap roadmap;

//     void add_node(Eigen::Ref<const Eigen::VectorXd> state, Eigen::Ref<const Eigen::MatrixXd> neighbor_states, 
//         Eigen::Ref<const Eigen::VectorXd> neighbor_costs) {
        
//         roadmap.add_node(state, neighbor_states, neighbor_costs);
//     }

//     std::vector<RoadmapNode> nodes() {
//         return roadmap.nodes();
//     }

//     RoadmapNode node_at(Eigen::Ref<const Eigen::VectorXd> state) {
//         return roadmap.node_at(state);
//     }

//     RoadmapNode node_nearest(Eigen::Ref<const Eigen::VectorXd> state) {
//         return roadmap.node_nearest(state);
//     }

//     std::vector<RoadmapNode> k_nodes_nearest(Eigen::Ref<const Eigen::VectorXd> state, int k) {
//         return roadmap.k_nodes_nearest(state, k);
//     }

//     Eigen::VectorXd state_nearest(Eigen::Ref<const Eigen::VectorXd> state) {
//         return roadmap.state_nearest(state);
//     }

//     Eigen::MatrixXd k_states_nearest(Eigen::Ref<const Eigen::VectorXd> state, int k) {
//         return roadmap.k_states_nearest(state, k);
//     }
// };

void init_roadmap(py::module_& m)
{
    py::class_<RoadmapNode>(m, "RoadMapNode")
        .def(py::init<>())
        .def_property_readonly("state", &RoadmapNode::getState)
        .def_property_readonly("neighbors", &RoadmapNode::getNeighbors)
        .def_property_readonly("costs", &RoadmapNode::getCosts);

    // py::class_<WrapRoadmap>(m, "RoadMap")
    //     .def(py::init<>())
    //     .def("add_node", &WrapRoadmap::add_node)
    //     .def("nodes", &WrapRoadmap::nodes)
    //     .def("node_at", &WrapRoadmap::node_at)
    //     .def("node_nearest", &WrapRoadmap::node_nearest)
    //     .def("k_nodes_nearest", &WrapRoadmap::k_nodes_nearest)
    //     .def("state_nearest", &WrapRoadmap::state_nearest)
    //     .def("k_states_nearest", &WrapRoadmap::k_states_nearest);

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