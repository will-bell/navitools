#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/eigen.h>
#include <pybind11/operators.h>
#include "search_space.hpp"


namespace py = pybind11;

class PySearchSpace : public SearchSpace {
public:
    // Inherit constructors
    using SearchSpace::SearchSpace;

    // Trampoline sample_free_space
    Eigen::MatrixXd sample_free_space(int n) const override {
        PYBIND11_OVERRIDE_PURE(
            Eigen::Ref<const Eigen::MatrixXd>,  // Return type
            SearchSpace,                        // Parent class
            sample_free_space,                  // Name of function in C++
            n                                   // Argument(s)
        );
    }

    // Trampoline valid_transition
    bool valid_transition(const Eigen::VectorXd& a, const Eigen::VectorXd& b) const override {
        PYBIND11_OVERRIDE_PURE(
            bool,               // Return type
            SearchSpace,        // Parent class
            valid_transition,   // Name of function in C++
            a,                  // Argument(s)
            b
        );
    }

    // Trampoline transition_cost
    double transition_cost(const Eigen::VectorXd& a, const Eigen::VectorXd& b) const override {
        PYBIND11_OVERRIDE_PURE(
            bool,               // Return type
            SearchSpace,        // Parent class
            transition_cost,    // Name of function in C++
            a,                  // Argument(s)
            b
        );
    }
};

void init_search_space(py::module_ &m)
{
    py::class_<SearchSpace, PySearchSpace>(m, "SearchSpace")
        .def(py::init<int>())
        .def("sample_free_space", &SearchSpace::sample_free_space)
        .def("valid_transition", &SearchSpace::valid_transition)
        .def("transition_cost", &SearchSpace::transition_cost);

    py::class_<PolygonSpace, SearchSpace>(m, "PolygonSpace")
        .def(py::init<std::vector<Polygon>, std::pair<double, double>, std::pair<double, double>>());
}