set(NAVIPY_SRCS,
    "..."
)

add_library(navipy STATIC ${NAVIPY_SRCS})
target_include_directories(navipy PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/headers
)