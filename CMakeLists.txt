cmake_minimum_required(VERSION 3.5)

project(human3D LANGUAGES C CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#include_directories(./)

find_package(Qt5OpenGL)
find_package(Qt5 COMPONENTS Core Widgets OpenGL LinguistTools REQUIRED)

SET(QT_OPENGL_DEPS
    Qt5::Widgets Qt5::OpenGL glut GLU GL pthread
    )

## test
add_executable(gltest
    qt_gl_test/PointCloudOpenGLWidget.h
    qt_gl_test/PointCloudOpenGLWidget.cpp
    qt_gl_test/BaseGLWidget.h
    qt_gl_test/BaseGLWidget.cpp
    qt_gl_test/main.cpp
    )
target_link_libraries(gltest PRIVATE ${QT_OPENGL_DEPS})

##
add_subdirectory(human3d/)
add_subdirectory(3dviewer/)




