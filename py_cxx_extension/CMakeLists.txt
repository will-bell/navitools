set(PY_CXX_EXTENSION_SRCS,
    "_navipy.cpp"
)

add_library(py_cxx_extension SHARED ${PY_CXX_EXTENSION_SRCS})

set_target_properties(
    py_cxx_extension
    PROPERTIES
        PREFIX ""
        OUTPUT_NAME ${NAVIPY_PYTHON_CXX_MODULE_NAME}
        LINKER_LANGUAGE CXX_STANDARD   
)

if(WIN32)
    set_target_properties(
        py_cxx_extension
        PROPERTIES
        SUFFIX ".pyd"
    )
endif()

target_include_directories(
    py_cxx_extension PUBLIC
    ${PYTHON_INCLUDE_DIRS}
    ${NUMPY_INCLUDE_DIR}
    ${PROJECT_SOURCE_DIR}/src/headers
)

target_link_libraries(py_cxx_extension navipy)

# On Windows, it is required to link to the Python headers
if(WIN32)
    target_link_libraries(py_cxx_extension ${PYTHON_LIBRARIES})