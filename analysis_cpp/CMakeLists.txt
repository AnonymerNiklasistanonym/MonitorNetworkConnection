cmake_minimum_required(VERSION 3.14)

project(dataAnalysis VERSION 1 LANGUAGES CXX)

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
    message(FATAL_ERROR "Not a git repository or git was not found")
endif()

add_subdirectory(fmt bin_fmt EXCLUDE_FROM_ALL)
add_subdirectory(date bin_date EXCLUDE_FROM_ALL)

add_executable(${PROJECT_NAME} main.cpp csv_file_reader.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE fast-cpp-csv-parser)
target_include_directories(${PROJECT_NAME} PRIVATE date/include)
target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt date::date Threads::Threads)

# Set library source files compilation flags for different compilers
target_compile_options(
    ${PROJECT_NAME}
    PRIVATE
    $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
    -pedantic
    -Wall
    -Werror
    # Disable errors for unused functions and parameters but still show warnings
    -Wno-error=unused-function
    -Wno-error=unused-parameter
    -Wextra
    -Wformat
    >
    $<$<CXX_COMPILER_ID:MSVC>:
    /Wall
    /pedantic
    /W4
    >
)

# Set C++ version for the project
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 23)
target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_23)

# Set Debug post fix to differentiate a debug and release executable
set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX "d")
