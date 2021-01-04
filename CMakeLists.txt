cmake_minimum_required(VERSION 3.11)

project(navipy LANGUAGES CXX_STANDARD)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

set(NAVIPY_PYTHON_CXX_MODULE_NAME "_navipy" CACHE STRING "Name of the C++ extension module file")

# Directory of C++ files for the library
add_subdirectory(src)

# Add Python component if requested
if(NAVIPY_PYTHON_SUPPORT)
    set(Python_FIND_VIRTUALENV FIRST)
    find_package(PythonInterp 3.5 REQUIRED)

    # Find NumPy headers
    exec_program(
        ${PYTHON_EXECUTABLE}
        ARGS "-c \"import numpy; print(numpy.get_include())\""
        OUTPUT_VARIABLE NUMPY_INCLUDE_DIR
        RETURN_VALUE NUMPY_NOT_FOUND
    )
    if(NUMPY_NOT_FOUND)
        message(FATAL_ERROR "NumPy headers not found")
    endif()

    # Find Python headers
    exec_program(
        ${PYTHON_EXECUTABLE}
        ARGS "-c \"import sysconfig; print(sysconfig.get_paths()['include'])\""
        OUTPUT_VARIABLE PYTHON_INCLUDE_DIRS
        RETURN_VALUE PYTHON_INCLUDE_DIRS_NOT_FOUND
    )
    if(PYTHON_INCLUDE_DIRS_NOT_FOUND)
        message(FATAL_ERROR "Python headers not found")
    endif()

    # This goes after, since it uses PythonInterp as a hint
    if(WIN32)
        find_package(PythonLibs 3.5 REQUIRED)
    endif()

    # Using pybind11 to wrap the library to make the Python extension
    add_subdirectory(extern/pybind11)
    pybind11_add_module(navipy py_cxx_extension/module.cpp)
endif()
