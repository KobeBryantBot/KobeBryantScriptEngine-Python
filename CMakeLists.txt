cmake_minimum_required(VERSION 3.15.0)
project(KobeBryantScriptEngine-Python LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED 23)

add_compile_options(
    /EHa
    /utf-8
    /O2
)

add_compile_definitions(
    NOMINMAX
    UNICODE
)

file(GLOB_RECURSE SOURCES "src/*.cpp")

find_package(Python3 3.13.0 COMPONENTS Interpreter Development REQUIRED)

set(RESOURCE_FILE ${PROJECT_SOURCE_DIR}/src/resource.rc)

include_directories(${PROJECT_SOURCE_DIR}/sdk/include)
include_directories(${PROJECT_SOURCE_DIR}/sdk/include/third)
include_directories(${PROJECT_SOURCE_DIR}/third/include)

add_library(${PROJECT_NAME} SHARED ${SOURCES} ${RESOURCE_FILE})

target_link_libraries(${PROJECT_NAME} PRIVATE Python3::Module
    ${PROJECT_SOURCE_DIR}/sdk/lib/KobeBryant.lib
    ${PROJECT_SOURCE_DIR}/sdk/lib/fmt/fmt.lib
)

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${PROJECT_NAME}> ${PROJECT_SOURCE_DIR}/bin/${PROJECT_NAME}/${PROJECT_NAME}.dll
    COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/manifest.json ${PROJECT_SOURCE_DIR}/bin/${PROJECT_NAME}/manifest.json
    COMMENT "Packing ${PROJECT_NAME} to ${PROJECT_SOURCE_DIR}/bin/${PROJECT_NAME}"
    VERBATIM
)
