cmake_minimum_required(VERSION 3.13)

project(navipy LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

set(NAVIPY_PY_CXX_MODULE_NAME "_navipy" CACHE STRING "Name of the C++ extension module file")

# Find Eigen so it can be linked to the targets
set(EIGEN3_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/extern/eigen")
find_package(Eigen3 REQUIRED)

# Directory of C++ files for the library
set(CXX_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
add_subdirectory(src)

# Find the right Python interpreter
set(Python_FIND_VIRTUALENV FIRST)
find_package(PythonInterp 3.6 REQUIRED)

# Find the NumPy headers
exec_program(
    ${PYTHON_EXECUTABLE}
    ARGS "-c \"import numpy; print(numpy.get_include())\""
    OUTPUT_VARIABLE NUMPY_INCLUDE_DIR
    RETURN_VALUE NUMPY_NOT_FOUND
)
if(NUMPY_NOT_FOUND)
    message(FATAL_ERROR "NumPy headers not found")
endif()

# Find the Python headers
exec_program(
    ${PYTHON_EXECUTABLE}
    ARGS "-c \"import sysconfig; print(sysconfig.get_paths()['include'])\""
    OUTPUT_VARIABLE PYTHON_INCLUDE_DIRS
    RETURN_VALUE PYTHON_INCLUDE_DIRS_NOT_FOUND
)
if(PYTHON_INCLUDE_DIRS_NOT_FOUND)
    message(FATAL_ERROR "Python headers not found")
endif()

# Using pybind11 to wrap the library to make the Python extension
add_subdirectory(extern/pybind11)

# Create the bindings
add_subdirectory(bindings)