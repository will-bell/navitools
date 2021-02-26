#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <sstream>
#include "vector.hpp"

namespace py = pybind11;

Vector2 foo(std::vector<Vector2> vectors)
{
    double x, y;
    for (auto vector : vectors) {
        x += vector.x();
        y += vector.y();
    }

    return {x, y};
}

std::string to_string_with_precision(const double val, const int n = 6)
{
    std::ostringstream out;
    out.precision(n);
    out << std::fixed << val;
    return out.str();
}

void init_vector(py::module_ &m)
{
    py::class_<Vector2>(m, "Vector2")
        .def(py::init<double, double>())
        .def_property_readonly("x", &Vector2::x)
        .def_property_readonly("y", &Vector2::y)
        .def("__repr__",
            [](const Vector2& v) {
                return "Vector2(" + to_string_with_precision(v.x(), 3) + ", " + to_string_with_precision(v.y(), 3) + ")";
            });

    py::class_<Vector3>(m, "Vector3")
        .def(py::init<double, double, double>())
        .def_property_readonly("x", &Vector3::x)
        .def_property_readonly("y", &Vector3::y)
        .def_property_readonly("z", &Vector3::z)
        .def("__repr__",
            [](const Vector3& v) {
                return "Vector3(" + to_string_with_precision(v.x(), 3) + ", " + to_string_with_precision(v.y(), 3) + ", " + to_string_with_precision(v.z(), 3) + ")";
            });

    m.def("magnitude", py::overload_cast<const Vector2&>(&magnitude));
    m.def("magnitude", py::overload_cast<const Vector3&>(&magnitude));

    m.def("normalize", py::overload_cast<const Vector2&>(&normalize));
    m.def("normalize", py::overload_cast<const Vector3&>(&normalize));

    m.def("foo", &foo);
}