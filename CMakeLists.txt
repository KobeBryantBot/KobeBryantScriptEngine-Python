cmake_minimum_required(VERSION 3.15.0)
project(KobeBryantScriptEngine-Python LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED 23)

add_compile_options(
    /EHa
    /utf-8
    -DNDEBUG
    /O2
)

add_compile_definitions(
    NOMINMAX
    UNICODE
)

file(GLOB_RECURSE SOURCES "src/*.cpp")

include_directories(${PROJECT_SOURCE_DIR}/sdk/include)
include_directories(${PROJECT_SOURCE_DIR}/sdk/include/third)
include_directories(${PROJECT_SOURCE_DIR}/third/include)

add_library(${PROJECT_NAME} SHARED ${SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE
    ${PROJECT_SOURCE_DIR}/sdk/lib/KobeBryant.lib
    ${PROJECT_SOURCE_DIR}/sdk/lib/fmt/fmt.lib
    ${PROJECT_SOURCE_DIR}/third/lib/Python/Python3.lib
    ${PROJECT_SOURCE_DIR}/third/lib/Python/python313.lib
)

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${PROJECT_SOURCE_DIR}/bin
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${PROJECT_NAME}> ${PROJECT_SOURCE_DIR}/bin/DLL/${PROJECT_NAME}.dll
    COMMAND ${PYTHON_EXECUTABLE} ${PROJECT_SOURCE_DIR}/scripts/after_build.py
    COMMENT "Packing ${PROJECT_NAME} to ${PROJECT_SOURCE_DIR}/bin/${PROJECT_NAME}"
    VERBATIM
)
