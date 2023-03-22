cmake_minimum_required(VERSION 3.24)
project(low_level_programming_2 C)

if (CMAKE_HOST_SYSTEM_NAME MATCHES "Darwin")
    execute_process(
            COMMAND brew --prefix bison
            RESULT_VARIABLE BREW_BISON
            OUTPUT_VARIABLE BREW_BISON_PREFIX
            OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    if (BREW_BISON EQUAL 0 AND EXISTS "${BREW_BISON_PREFIX}")
        message(STATUS "Found Bison keg installed by Homebrew at ${BREW_BISON_PREFIX}")
        set(BISON_EXECUTABLE "${BREW_BISON_PREFIX}/bin/bison")
    endif()

    execute_process(
            COMMAND brew --prefix flex
            RESULT_VARIABLE BREW_FLEX
            OUTPUT_VARIABLE BREW_FLEX_PREFIX
            OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    if (BREW_FLEX EQUAL 0 AND EXISTS "${BREW_FLEX_PREFIX}")
        message(STATUS "Found Flex keg installed by Homebrew at ${BREW_FLEX_PREFIX}")
        set(FLEX_EXECUTABLE "${BREW_FLEX_PREFIX}/bin/flex")
    endif()
endif()

set(SRC_DIR "/Users/matthew/CLionProjects/low_level_programming_2")
set(PARSER_DIR "${CMAKE_CURRENT_BINARY_DIR}")

find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)

set(LEXER_OUT "${PARSER_DIR}/lexer.c")
set(PARSER_OUT "${PARSER_DIR}/parser.c")

BISON_TARGET(BISON "${SRC_DIR}/parser.y" "${PARSER_OUT}" DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/parser.h)
FLEX_TARGET(LEXER "${SRC_DIR}/lexer.l" "${LEXER_OUT}" DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/lexer.h)

ADD_FLEX_BISON_DEPENDENCY(LEXER BISON)
set(CMAKE_C_STANDARD 99)

add_executable(low_level_programming_2
        "${SRC_DIR}/main.c"
        "${LEXER_OUT}" "${PARSER_OUT}"
        constant/constant.c struct/map_entry.c ast/ast.c
        cmake-build-debug)

#target_compile_options(low_level_programming_2 PRIVATE "--debug")

target_include_directories(low_level_programming_2 PRIVATE "${PARSER_DIR}")
