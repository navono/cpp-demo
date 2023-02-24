##################################################    Project     ##################################################
cmake_minimum_required(VERSION 3.12...3.24 FATAL_ERROR)

project(sharedLib VERSION 0.1 LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
include(get_platform)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
#set                   (CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)
set(CMAKE_DEBUG_POSTFIX D)


if (TARGET_PLATFORM_WINDOWS)
    add_compile_options(/W4)
else ()
    add_compile_options(-Wall -Wextra -pedantic)
endif ()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY $<1:${CMAKE_SOURCE_DIR}/bin>)

message(STATUS "Build lib: ${PROJECT_NAME}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Build platform: ${CMAKE_SYSTEM_NAME}")
message(STATUS "Build output: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")


##################################################    Options     ##################################################
option(BUILD_SHARED_LIBS "Build shared (dynamic) libraries." ON)
option(BUILD_TESTS "Build tests." OFF)


##################################################    Sources     ##################################################
file(GLOB_RECURSE PROJECT_HEADERS
        ${PROJECT_SOURCE_DIR}/include/*.h
        ${PROJECT_SOURCE_DIR}/include/*.hpp
        ${PROJECT_SOURCE_DIR}/include/*.ipp
        ${CMAKE_SOURCE_DIR}/include/sharedLib/.h
        ${CMAKE_SOURCE_DIR}/include/sharedLib/*.hpp
        ${CMAKE_SOURCE_DIR}/include/sharedLib/*.ipp)
file(GLOB_RECURSE PROJECT_SOURCES
        ${PROJECT_SOURCE_DIR}/src/*.c
        ${PROJECT_SOURCE_DIR}/src/*.cpp
        ${PROJECT_SOURCE_DIR}/src/*.cu)

file(GLOB_RECURSE PROJECT_CMAKE_UTILS cmake/*.cmake)
file(GLOB_RECURSE PROJECT_MISC *.bat *.gitignore *.md *.py *.sh *.txt)
set(PROJECT_EXPORT_HPP ${CMAKE_SOURCE_DIR}/include/${PROJECT_NAME}/export.hpp)
set(PROJECT_FILES
        ${PROJECT_SOURCES}
        ${PROJECT_HEADERS}
        ${PROJECT_CMAKE_UTILS}
        ${PROJECT_MISC}
        ${PROJECT_EXPORT_HPP})

include(assign_source_group)
assign_source_group(${PROJECT_FILES})

##################################################  Dependencies  ##################################################
include(import_library)

# ADD LIBRARIES HERE.
# Examples:
# - Header Only:
#   find_package  (GLM REQUIRED)
#   import_library(GLM_INCLUDE_DIRS)
# - Identical Debug and Release:
#   find_package  (OpenGL REQUIRED)
#   import_library(OPENGL_INCLUDE_DIR OPENGL_LIBRARIES)
# - Separate Debug and Release:
#   find_package  (Boost REQUIRED iostreams)
#   import_library(Boost_INCLUDE_DIRS Boost_IOSTREAMS_LIBRARY_DEBUG Boost_IOSTREAMS_LIBRARY_RELEASE)
# - You may also set the PROJECT_INCLUDE_DIRS and PROJECT_LIBRARIES instead of using import_library.

list(APPEND PROJECT_LIBRARIES)
list(APPEND PROJECT_INCLUDE_DIRS ${PROJECT_HEADERS})

##################################################    Targets     ##################################################
#message(STATUS "PROJECT_FILES: ${PROJECT_FILES}")
#message(STATUS "PROJECT_INCLUDE_DIRS: ${PROJECT_INCLUDE_DIRS}")
#message(STATUS "PROJECT_LIBRARIES: ${PROJECT_HEADERS}")

add_library(${PROJECT_NAME} SHARED ${PROJECT_FILES})
add_library(demo::sharedLib ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
        $<INSTALL_INTERFACE:include> PRIVATE source/sharedLib)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PUBLIC ${PROJECT_LIBRARIES})
target_compile_definitions(${PROJECT_NAME} PUBLIC ${PROJECT_COMPILE_DEFINITIONS})
target_compile_options(${PROJECT_NAME} PUBLIC ${PROJECT_COMPILE_OPTIONS})
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

if (NOT BUILD_SHARED_LIBS)
    string(TOUPPER ${PROJECT_NAME} PROJECT_NAME_UPPER)
    set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS -D${PROJECT_NAME_UPPER}_STATIC)
endif ()

##################################################    Postbuild   ##################################################
include(GenerateExportHeader)
string(TOUPPER ${PROJECT_NAME} PROJECT_NAME_UPPER)

generate_export_header(${PROJECT_NAME}
        EXPORT_FILE_NAME ${CMAKE_SOURCE_DIR}/include/${PROJECT_NAME}/export.hpp
        EXPORT_MACRO_NAME ${PROJECT_NAME_UPPER}_EXPORT
        STATIC_DEFINE ${PROJECT_NAME_UPPER}_STATIC)

##################################################    Testing     ##################################################
if (BUILD_TESTS)
    enable_testing()
    set(TEST_MAIN_NAME test_main)
    set(TEST_MAIN_SOURCES tests/internal/main.cpp)
    add_library(${TEST_MAIN_NAME} OBJECT ${TEST_MAIN_SOURCES})
    set_property(TARGET ${TEST_MAIN_NAME} PROPERTY FOLDER tests)
    assign_source_group(${TEST_MAIN_SOURCES})

    file(GLOB PROJECT_TEST_CPPS tests/*.c tests/*.cpp tests/*.cu)
    foreach (_SOURCE ${PROJECT_TEST_CPPS})
        get_filename_component(_NAME ${_SOURCE} NAME_WE)
        add_executable(${_NAME} ${_SOURCE} $<TARGET_OBJECTS:${TEST_MAIN_NAME}>)
        target_link_libraries(${_NAME} ${PROJECT_NAME} ${PROJECT_LIBRARIES})
        add_test(${_NAME} ${_NAME})
        set_property(TARGET ${_NAME} PROPERTY FOLDER tests)
        assign_source_group(${_SOURCE})
    endforeach ()
endif ()

##################################################  Installation  ##################################################
#install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}-config
#  ARCHIVE DESTINATION lib
#  LIBRARY DESTINATION lib
#  RUNTIME DESTINATION bin)
#install(DIRECTORY include/ DESTINATION include)
#install(EXPORT  ${PROJECT_NAME}-config DESTINATION cmake)
#export (TARGETS ${PROJECT_NAME}        FILE        ${PROJECT_NAME}-config.cmake)
