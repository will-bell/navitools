set(
    BINDINGS_SRCS
    ${CXX_SOURCES}
    "bind_geometry.cpp"
    "bind_kd_tree.cpp"
    "bind_roadmap.cpp"
    "bind_search_space.cpp"
    )

# Create the module using pybind11's wrapped version of `add_library`
pybind11_add_module(_navipy ${BINDINGS_SRCS} module.cpp)

# Include the C++ source header files
target_include_directories(_navipy PUBLIC ${CXX_SOURCE_DIR}/headers)

# Include the Eigen headers
target_link_libraries(navipy Eigen3::Eigen)

# Link the Python bindings with the source
target_link_libraries(_navipy PUBLIC navipy)

# Pass the Version info from setup.py to the module
target_compile_definitions(_navipy PRIVATE VERSION_INFO=${NAVIPY_VERSION_INFO}) 
