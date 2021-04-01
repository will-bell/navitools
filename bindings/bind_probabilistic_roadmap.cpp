#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "probabilistic_roadmap.hpp"

namespace py = pybind11;

void init_probabilistic_roadmap(py::module_& m)
{
    m.def("build_prm", &build_prm);
}