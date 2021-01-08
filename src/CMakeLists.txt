# Make a list of all the source files to add to the library
set(CXX_SOURCES
    "graph.cpp"
    "vector.cpp"
)

# Create the library
add_library(navipy STATIC ${CXX_SOURCES})

# Headers are in the subdirectory
target_include_directories(navipy PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/headers)
