##################################################    Project     ##################################################
cmake_minimum_required(VERSION 3.24)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
include(utils)
extract_version()

project(demo VERSION ${DEMO_VERSION} LANGUAGES CXX)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_CXX_STANDARD 20)

include(get_platform)

# 编译选项
if (TARGET_PLATFORM_WINDOWS)
    add_compile_options(/W4)
    add_compile_options(/WX-)
else ()
    add_compile_options(-Wall -Wextra -pedantic)
endif ()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY $<1:${CMAKE_SOURCE_DIR}/bin>)

message(STATUS "Build demo: ${DEMO_VERSION}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Build platform: ${CMAKE_SYSTEM_NAME}")
message(STATUS "Build output: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")

##################################################    Sources     ##################################################
#message(STATUS "Build PROJECT_SOURCE_DIR: ${PROJECT_SOURCE_DIR}")
#message(STATUS "Build CMAKE_SOURCE_DIR: ${CMAKE_SOURCE_DIR}")
file(GLOB_RECURSE PROJECT_HEADERS
        ${PROJECT_SOURCE_DIR}/src/include/*.h
        ${PROJECT_SOURCE_DIR}/src/include/*.hpp
        ${CMAKE_SOURCE_DIR}/include/*.h
        ${CMAKE_SOURCE_DIR}/include/*.hpp
        ${CMAKE_SOURCE_DIR}/include/*.ipp)
file(GLOB_RECURSE PROJECT_SOURCES
        ${PROJECT_SOURCE_DIR}/src/*.c
        ${PROJECT_SOURCE_DIR}/src/*.cc
        ${PROJECT_SOURCE_DIR}/src/*.cxx
        ${PROJECT_SOURCE_DIR}/src/*.cpp
        ${PROJECT_SOURCE_DIR}/src/*.cu)
file(GLOB_RECURSE PROJECT_CMAKE_UTILS cmake/*.cmake)
file(GLOB_RECURSE PROJECT_MISC *.bat *.gitignore *.md *.py *.sh *.txt)
aux_source_directory(./src/controllers PROJECT_SOURCES)
aux_source_directory(./src/filters PROJECT_SOURCES)

if (TARGET_PLATFORM_WINDOWS)
    #      message(STATUS "Build CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}/version.rc.in")
    #      message(STATUS "Build CMAKE_CURRENT_BINARY_DIR: ${CMAKE_CURRENT_BINARY_DIR}")
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/version.rc.in ${CMAKE_CURRENT_BINARY_DIR}/version.rc @ONLY)
    list(APPEND PROJECT_SOURCES ${CMAKE_CURRENT_BINARY_DIR}/version.rc)
    add_compile_definitions(_WINDOWS)
endif ()

set(PROJECT_FILES
        ${PROJECT_HEADERS}
        ${PROJECT_SOURCES}
        ${PROJECT_CMAKE_UTILS}
        ${PROJECT_MISC} src/fp.h)

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

set(BOOST_ROOT ${CMAKE_SOURCE_DIR}/third_party/boost)
#set(Boost_DEBUG ON)
find_package(Boost COMPONENTS program_options)
if (Boost_FOUND)
    #  message(STATUS "Boost_INCLUDE_DIRS = ${Boost_INCLUDE_DIRS}")
    #  message(STATUS "Boost_LIBRARY_DIRS = ${Boost_LIBRARY_DIRS}")
    #  message(STATUS "PROJECT_LIBRARIES = ${PROJECT_LIBRARIES}")

    list(APPEND PROJECT_INCLUDE_DIRS ${Boost_INCLUDE_DIRS})
    list(APPEND PROJECT_LIB_DIR ${Boost_LIBRARY_DIRS})
    list(APPEND PROJECT_LIBRARIES ${Boost_LIBRARIES})
endif ()

if (CMAKE_BUILD_TYPE AND (CMAKE_BUILD_TYPE STREQUAL "Debug"))
    list(APPEND PROJECT_LIB_DIR ${CMAKE_SOURCE_DIR}/third_party/libs/Debug)
elseif (CMAKE_BUILD_TYPE AND (CMAKE_BUILD_TYPE STREQUAL "Release"))
    list(APPEND PROJECT_LIB_DIR ${CMAKE_SOURCE_DIR}/third_party/libs/Release)
endif ()

list(APPEND PROJECT_LIBRARIES
        cxxopts
        libzmq
        drogon
        demo::sharedLib
        range-v3
        #        fmt
        #        spdlog::spdlog
        #        cppcommon
        #        sago::platform_folders
        #        sqlite_orm::sqlite_orm
        #        nlohmann_json::nlohmann_json
        #        Matplot++::matplot
        )

if (TARGET_PLATFORM_WINDOWS)
    list(APPEND PROJECT_LIBRARIES
            #            hid.lib
            ws2_32.lib
            iphlpapi.lib)
endif ()


##################################################    Targets     ##################################################
include_directories(${PROJECT_INCLUDE_DIRS})
link_directories(${PROJECT_LIB_DIR}) # 要在 add_executable 前
link_libraries(${PROJECT_LIBRARIES})

if (TARGET_PLATFORM_WINDOWS)
    # set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")
    add_compile_definitions(UNICODE _UNICODE)
    add_executable(${PROJECT_NAME} WIN32 ${PROJECT_FILES})
else ()
    add_executable(${PROJECT_NAME} ${PROJECT_FILES})
endif ()

add_compile_definitions(ZMQ_STATIC)
target_include_directories(${PROJECT_NAME} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
        $<INSTALL_INTERFACE:include> PRIVATE ..)
#target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_INCLUDE_DIRS})
#target_link_libraries     (${PROJECT_NAME} PUBLIC ${PROJECT_LIBRARIES})

target_compile_definitions(${PROJECT_NAME} PUBLIC ${PROJECT_COMPILE_DEFINITIONS})
target_compile_options(${PROJECT_NAME} PUBLIC ${PROJECT_COMPILE_OPTIONS})
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

if (NOT BUILD_SHARED_LIBS)
    string(TOUPPER ${PROJECT_NAME} PROJECT_NAME_UPPER)
    set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS -D${PROJECT_NAME_UPPER}_STATIC)
endif ()

if (TARGET_PLATFORM_WINDOWS)
    set_target_properties(${PROJECT_NAME} PROPERTIES
            VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
            )
    target_compile_definitions(${PROJECT_NAME} PRIVATE
            # Prevents Windows.h from adding unnecessary includes
            WIN32_LEAN_AND_MEAN
            # Prevents Windows.h from defining min/max as macros
            NOMINMAX)
endif ()

message(STATUS "PROJECT_INCLUDE_DIRS = ${PROJECT_INCLUDE_DIRS}")
message(STATUS "PROJECT_LIB_DIR = ${PROJECT_LIB_DIR}")
message(STATUS "PROJECT_LIBRARIES = ${PROJECT_LIBRARIES}")
message(STATUS "PROJECT_COMPILE_OPTIONS = ${PROJECT_COMPILE_OPTIONS}")


##################################################    Postbuild   ##################################################
# copy third party dll
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:${PROJECT_NAME}> $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMAND_EXPAND_LISTS)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/config
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/config)


##################################################  Installation  ##################################################
#install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}-config
#        RUNTIME DESTINATION ../../bin)
#install(DIRECTORY ../../include DESTINATION ../../include)
#install(EXPORT ${PROJECT_NAME}-config DESTINATION ../../cmake)
#export(TARGETS ${PROJECT_NAME} FILE ${PROJECT_NAME}-config.cmake)
