cmake_minimum_required(VERSION 3.25)

project(runtests VERSION 0.1 LANGUAGES CXX)

include(GNUInstallDirs)
include(FeatureSummary)

option(USE_STATIC "Link statically with libxmltest" OFF)

add_executable(runtests testmain.cpp)

if(USE_STATIC)
    find_package(xmltest REQUIRED COMPONENTS static)
else()
    find_package(xmltest REQUIRED COMPONENTS shared)
endif()

target_link_libraries(runtests PRIVATE xmltest::xmltest)
