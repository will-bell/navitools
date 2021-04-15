# Make a list of all the source files to add to the library
set(CXX_SOURCES
    "geometry.cpp"
    "kd_tree.cpp"
    "roadmap.cpp"
    "search_space.cpp"
    "probabilistic_roadmap"
    "search_roadmap"
)

# Create the library
add_library(navipy STATIC ${CXX_SOURCES})

set_property(TARGET navipy PROPERTY POSITION_INDEPENDENT_CODE ON)

# Headers are in the subdirectory
target_include_directories(navipy PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/headers)

# Include the Eigen headers
target_link_libraries(navipy Eigen3::Eigen)