#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/eigen.h>
#include <pybind11/operators.h>
#include "search_space.hpp"

namespace py = pybind11;

void init_search_space(py::module_ &m)
{
    py::class_<SearchSpace>(m, "SearchSpace")
        .def(py::init<>())
        .def("sample_free_space", &SearchSpace::sample_free_space);

    py::class_<PolygonSpace, SearchSpace>(m, "PolygonSpace")
        .def(py::init<std::vector<Polygon>, std::pair<double, double>, std::pair<double, double>>());
}