cmake_minimum_required(VERSION 4.2)
project(Practice)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE SRC_FILES "src/*.c" "src/*.h")

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK REQUIRED IMPORTED_TARGET gtk4)

find_program(GLIB_COMPILE_RESOURCES glib-compile-resources REQUIRED)
set(RESOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/resources")
set(RESOURCES_XML "${CMAKE_CURRENT_SOURCE_DIR}/resources/resources.gresource.xml")
set(RESOURCES_SRC "${CMAKE_CURRENT_BINARY_DIR}/resources.c")
execute_process(
    COMMAND ${GLIB_COMPILE_RESOURCES} --sourcedir=${RESOURCE_DIR} --generate-dependencies ${RESOURCES_XML}
    OUTPUT_VARIABLE RESOURCE_DEPENDENCIES
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
string(REPLACE "\n" ";" RESOURCE_DEPENDENCIES "${RESOURCE_DEPENDENCIES}")
add_custom_command(
    OUTPUT ${RESOURCES_SRC}
    COMMAND ${GLIB_COMPILE_RESOURCES}
            --target=${RESOURCES_SRC}
            --sourcedir=${RESOURCE_DIR}
            --generate-source
            ${RESOURCES_XML}
    DEPENDS ${RESOURCES_XML} ${RESOURCE_DEPENDENCIES}
    COMMENT "Компиляция ресурсов GTK..."
)

add_executable(EXE ${SRC_FILES} ${RESOURCES_SRC})

target_link_libraries(EXE PRIVATE PkgConfig::GTK)
