cmake_minimum_required(VERSION 3.14)

project(dataAnalysis VERSION 1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(Threads REQUIRED)

find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/../.git")
    # Update submodules as needed
    option(GIT_SUBMODULE "Check submodules during build" ON)
    if(GIT_SUBMODULE)
        message(STATUS "Submodule update")
        execute_process(
            COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            RESULT_VARIABLE GIT_SUBMOD_RESULT
        )
        if(NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(
                FATAL_ERROR
                "git submodule update --init failed with ${GIT_SUBMOD_RESULT}, please checkout submodules"
            )
        endif()
    endif()
else()
    # Error when not cloned via git
    ## TODO: Find solution how to download the submodule even though the source code is from a tarball
    message(FATAL_ERROR "Not a git repository or git was not found")
endif()

add_subdirectory(fmt)

add_executable(${PROJECT_NAME} main.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE fast-cpp-csv-parser)
target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt Threads::Threads)
