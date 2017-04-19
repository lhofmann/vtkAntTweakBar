cmake_minimum_required(VERSION 2.8)

project(vtkAntTweakBar)

set(CMAKE_MODULE_PATH
    "${CMAKE_CURRENT_SOURCE_DIR}/CMake/Modules"
    "${CMAKE_MODULE_PATH}"
)

find_package(VTK REQUIRED)
find_package(AntTweakBar REQUIRED)

include(${VTK_USE_FILE})
include_directories(${ANT_TWEAK_BAR_INCLUDE_PATH})
include_directories("${PROJECT_SOURCE_DIR}/include")

add_library(vtkAntTweakBar STATIC "${PROJECT_SOURCE_DIR}/src/vtkAntTweakBar.cpp")
target_link_libraries(vtkAntTweakBar ${VTK_LIBRARIES} ${ANT_TWEAK_BAR_LIBRARY})

option(BUILD_DEMO "Build vtkAntTweakBar demo program" ON)
if(BUILD_DEMO)
	add_executable(vtkAntTweakBarDemo "${PROJECT_SOURCE_DIR}/demo/main.cpp")
	target_link_libraries(vtkAntTweakBarDemo ${VTK_LIBRARIES} ${ANT_TWEAK_BAR_LIBRARY} vtkAntTweakBar)
endif(BUILD_DEMO)
