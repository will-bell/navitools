#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "vector.hpp"

namespace py = pybind11;

void init_vector(py::module_ &m)
{
    py::class_<Vector2>(m, "Vector2")
        .def(py::init<double, double>())
        .def_readwrite("x", &Vector2::x)
        .def_readwrite("y", &Vector2::y);

    py::class_<Vector3>(m, "Vector3")
        .def(py::init<double, double, double>())
        .def_readwrite("x", &Vector3::x)
        .def_readwrite("y", &Vector3::y)
        .def_readwrite("z", &Vector3::z);

    m.def("magnitude", py::overload_cast<const Vector2&>(&magnitude));
    m.def("magnitude", py::overload_cast<const Vector3&>(&magnitude));

    m.def("normalize", py::overload_cast<const Vector2&>(&normalize));
    m.def("normalize", py::overload_cast<const Vector3&>(&normalize));
}