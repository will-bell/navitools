set(
    BINDINGS_SRCS
    ${CXX_SOURCES}
    "bind_vector.cpp"
    "bind_geometry.cpp"
    "bind_kd_tree.cpp"
    )

# Create the module using pybind11's wrapped version of `add_library`
pybind11_add_module(_navipy ${BINDINGS_SRCS} module.cpp)

# Target the C++ source files
target_include_directories(_navipy PUBLIC ${CXX_SOURCE_DIR}/headers)

target_link_libraries(_navipy PUBLIC navipy)

# Pass the Version info from setup.py to the module
target_compile_definitions(_navipy PRIVATE VERSION_INFO=${NAVIPY_VERSION_INFO}) 
