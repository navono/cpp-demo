add_library(foo SHARED foo.cpp)

add_definitions(-DDYNALO_EXPORT_SYMBOLS)

# state that anybody linking to us needs to include the current source dir
target_include_directories(foo
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/../include
)
