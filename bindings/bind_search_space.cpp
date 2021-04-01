#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/eigen.h>
#include <pybind11/operators.h>
#include "search_space.hpp"

namespace py = pybind11;

void init_search_space(py::module_ &m)
{
    py::class_<PolygonSpace>(m, "PolygonSpace")
        .def(py::init<std::vector<Polygon>, std::pair<double, double>, std::pair<double, double>>())
        .def("sample_free_space", &PolygonSpace::sample_free_space);
}