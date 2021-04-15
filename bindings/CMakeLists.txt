set(
    BINDINGS_SRCS
    ${CXX_SOURCES}
    "bind_geometry.cpp"
    "bind_kd_tree.cpp"
    "bind_roadmap.cpp"
    "bind_search_space.cpp"
    )

# Create the module using pybind11's wrapped version of `add_library`
pybind11_add_module(_navitools ${BINDINGS_SRCS} module.cpp)

set_property(TARGET _navitools PROPERTY POSITION_INDEPENDENT_CODE ON)

# Include the C++ source header files
target_include_directories(_navitools PUBLIC ${CXX_SOURCE_DIR}/headers)

# Include the Eigen headers
target_link_libraries(navitools Eigen3::Eigen)

# Link the Python bindings with the source
target_link_libraries(_navitools PUBLIC navitools)

# Pass the Version info from setup.py to the module
target_compile_definitions(_navitools PRIVATE VERSION_INFO=${NAVITOOLS_VERSION_INFO}) 
