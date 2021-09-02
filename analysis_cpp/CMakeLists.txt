cmake_minimum_required(VERSION 3.14)

project(dataAnalysis VERSION 1 LANGUAGES CXX)

# Set important paths
set(PROJECT_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(INCLUDE_DIR "${PROJECT_DIR}/include")
set(SOURCE_DIR "${PROJECT_DIR}")
set(VENDOR_DIR_DATE "${PROJECT_DIR}/date")
set(VENDOR_DIR_FMT "${PROJECT_DIR}/fmt")
set(VENDOR_DIR_CSV_PARSER "${PROJECT_DIR}/fast-cpp-csv-parser")

# Show or hide exact makefile commands
set(CMAKE_VERBOSE_MAKEFILE on)

# Packages to find
find_package(Threads REQUIRED)
find_package(Git QUIET)

# Update git submodules when in git repository
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

# Add git subdirectories
add_subdirectory(${VENDOR_DIR_FMT} bin_fmt EXCLUDE_FROM_ALL)
add_subdirectory(${VENDOR_DIR_DATE} bin_date EXCLUDE_FROM_ALL)

# Find all source/header files
# > Header files
file(
    GLOB_RECURSE PROJECT_HEADER_FILES
    "${INCLUDE_DIR}/*.h"
    "${INCLUDE_DIR}/*.hpp"
    "${SOURCE_DIR}/*.h"
    "${SOURCE_DIR}/*.hpp"
)
# > Remove header files from the build/binary directory and print the other ones
message(STATUS "Project header files:")
foreach(PROJECT_HEADER_FILE ${PROJECT_HEADER_FILES})
    string(FIND ${PROJECT_HEADER_FILE} ${PROJECT_BINARY_DIR} EXCLUDE_DIR_FOUND_BIN)
    string(FIND ${PROJECT_HEADER_FILE} ${VENDOR_DIR_DATE} EXCLUDE_DIR_FOUND_DATE)
    string(FIND ${PROJECT_HEADER_FILE} ${VENDOR_DIR_FMT} EXCLUDE_DIR_FOUND_FMT)
    string(FIND ${PROJECT_HEADER_FILE} ${VENDOR_DIR_CSV_PARSER} EXCLUDE_DIR_FOUND_CSV_PARSER)
    if ((NOT ${EXCLUDE_DIR_FOUND_BIN} EQUAL -1) OR (NOT ${EXCLUDE_DIR_FOUND_DATE} EQUAL -1) OR (NOT ${EXCLUDE_DIR_FOUND_FMT} EQUAL -1) OR (NOT ${EXCLUDE_DIR_FOUND_CSV_PARSER} EQUAL -1))
        list(REMOVE_ITEM PROJECT_HEADER_FILES ${PROJECT_HEADER_FILE})
    else()
        message(STATUS "- ${PROJECT_HEADER_FILE}")
    endif()
endforeach()
# > Source files
set(PROJECT_MAIN_SOURCE_FILE "${SOURCE_DIR}/main.cpp")
file(
    GLOB_RECURSE PROJECT_SOURCE_FILES
    "${SOURCE_DIR}/*.cpp"
)
# > Remove source files from the build/binary directory and print the other ones
message(STATUS "Project source files:")
foreach(PROJECT_SOURCE_FILE ${PROJECT_SOURCE_FILES})
    string(FIND ${PROJECT_SOURCE_FILE} ${PROJECT_BINARY_DIR} EXCLUDE_DIR_FOUND_BIN)
    string(FIND ${PROJECT_SOURCE_FILE} ${VENDOR_DIR_DATE} EXCLUDE_DIR_FOUND_DATE)
    string(FIND ${PROJECT_SOURCE_FILE} ${VENDOR_DIR_FMT} EXCLUDE_DIR_FOUND_FMT)
    string(FIND ${PROJECT_SOURCE_FILE} ${VENDOR_DIR_CSV_PARSER} EXCLUDE_DIR_FOUND_CSV_PARSER)
    if ((NOT ${EXCLUDE_DIR_FOUND_BIN} EQUAL -1) OR (NOT ${EXCLUDE_DIR_FOUND_DATE} EQUAL -1) OR (NOT ${EXCLUDE_DIR_FOUND_FMT} EQUAL -1) OR (NOT ${EXCLUDE_DIR_FOUND_CSV_PARSER} EQUAL -1))
        list(REMOVE_ITEM PROJECT_SOURCE_FILES ${PROJECT_SOURCE_FILE})
    else()
        message(STATUS "- ${PROJECT_SOURCE_FILE}")
    endif()
endforeach()
# > Extract main source file
get_filename_component(
    PROJECT_MAIN_SOURCE_FILE
    ${PROJECT_MAIN_SOURCE_FILE}
    ABSOLUTE
)
list(REMOVE_ITEM PROJECT_SOURCE_FILES "${PROJECT_MAIN_SOURCE_FILE}")

# Option: Statically check source code
option(CHECK_CODE "Statically check source code" OFF)
message(STATUS "Statically check source code: CHECK_CODE=${CHECK_CODE}")
find_program(CPPCHECK_EXECUTABLE "cppcheck")
if(CHECK_CODE)
    if(CPPCHECK_EXECUTABLE)
        list(
            APPEND
            CPPCHECK_ARGS
            --enable=warning,style,performance,portability,unusedFunction
            --std=c++20
            --verbose
            --error-exitcode=1
            --language=c++
            -I
            "${INCLUDE_DIR}"
            -I
            "${SOURCE_DIR}"
            ${PROJECT_MAIN_SOURCE_FILE}
            ${PROJECT_SOURCE_FILES}
            ${PROJECT_HEADER_FILES}
        )
        add_custom_target(
            "${PROJECT_NAME}-check-code"
            ALL
            COMMAND
            ${CPPCHECK_EXECUTABLE}
            ${CPPCHECK_ARGS}
            WORKING_DIRECTORY
            ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT
            "Statically lint C++ code with CppCheck"
            VERBATIM
        )
    else()
        message(
            FATAL_ERROR
            "The program CppCheck (cppcheck or cppcheck.exe) was not found but is required"
        )
    endif()
endif()

# Option: Format source code
option(FORMAT_CODE "Format source code" OFF)
message(STATUS "Format source code: FORMAT_CODE=${FORMAT_CODE}")
find_program(ASTYLE_EXECUTABLE "astyle")
if(FORMAT_CODE)
    if(ASTYLE_EXECUTABLE)
        list(
            APPEND
            ASTYLE_ARGS
            --style=1tbs
            --lineend=linux
            --suffix=none
            --pad-oper
            --unpad-paren
            --break-closing-brackets
            --align-pointer=name
            --align-reference=name
            --indent-preproc-define
            --indent-switches
            --indent-col1-comments
            --keep-one-line-statements
            --keep-one-line-blocks
            --pad-header
            --convert-tabs
            --min-conditional-indent=0
            --indent=spaces=4
            --close-templates
            --add-brackets
            --break-after-logical
            --max-code-length=100
            ${PROJECT_MAIN_SOURCE_FILE}
            ${PROJECT_SOURCE_FILES}
            ${PROJECT_HEADER_FILES}
        )
        add_custom_target(
            "${PROJECT_NAME}-format-code"
            ALL
            COMMAND
            ${ASTYLE_EXECUTABLE}
            ${ASTYLE_ARGS}
            WORKING_DIRECTORY
            ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT
            "Format C++ code with Astyle"
            VERBATIM
        )
    else()
        message(
            FATAL_ERROR
            "The program Astyle (astyle or astyle.exe) was not found but is required"
        )
    endif()
endif()

add_executable(${PROJECT_NAME} ${PROJECT_MAIN_SOURCE_FILE} ${PROJECT_SOURCE_FILES} ${PROJECT_HEADER_FILES})
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
