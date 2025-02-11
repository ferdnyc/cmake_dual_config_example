cmake_minimum_required(VERSION 3.25)

project(xmltest VERSION 0.1 LANGUAGES CXX)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)
include(FeatureSummary)

if (NOT DEFINED BUILD_SHARED_LIBS)
    set(BUILD_SHARED_LIBS TRUE)
endif()

if (BUILD_SHARED_LIBS)
    set(lib_type SHARED)
    find_package(tinyxml2 REQUIRED COMPONENTS shared)
else()
    set(lib_type STATIC)
    find_package(tinyxml2 REQUIRED COMPONENTS static)
endif()

add_library(xmltest ${lib_type} xmltest.cpp xmltest.h)
add_library(xmltest::xmltest ALIAS xmltest)

target_link_libraries(xmltest PUBLIC tinyxml2::tinyxml2)

target_include_directories(
    xmltest PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

target_compile_definitions(xmltest PRIVATE BUILDING_XMLTEST)

set_target_properties(
    xmltest PROPERTIES
        VERSION ${PROJECT_VERSION}
        SOVERSION ${PROJECT_VERSION_MAJOR}
)

install(
    TARGETS xmltest EXPORT xmltest-targets
    RUNTIME COMPONENT xmltest_runtime
    LIBRARY COMPONENT xmltest_runtime
    NAMELINK_COMPONENT xmltest_development
    ARCHIVE COMPONENT xmltest_development
    INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)

install(
    FILES xmltest.h
    TYPE INCLUDE
    COMPONENT xmltest_development
)

string(TOLOWER ${lib_type} export_type)

install(
    EXPORT xmltest-targets
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/xmltest
    NAMESPACE xmltest::
    FILE xmltest-${export_type}-targets.cmake
    COMPONENT xmltest_development
)

configure_package_config_file(
    cmake/xmltest-config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/xmltest-config.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/xmltest
)

write_basic_package_version_file(
    xmltest-config-version.cmake
    COMPATIBILITY SameMajorVersion
)
install(
    FILES
        "${CMAKE_CURRENT_BINARY_DIR}/xmltest-config.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/xmltest-config-version.cmake"
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/xmltest
        COMPONENT xmltest_development
)

feature_summary(WHAT ALL)
