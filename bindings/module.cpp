#include <Python.h>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

namespace py = pybind11;

void init_vector(py::module_ &);

PYBIND11_MODULE(_navipy, m)
{   
    init_vector(m);
}