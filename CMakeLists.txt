cmake_minimum_required(VERSION 3.5)

project(human3D LANGUAGES C CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(./)

find_package(Qt5OpenGL)
find_package(Qt5 COMPONENTS Core Widgets OpenGL LinguistTools REQUIRED)

set(SRCS
    main.cpp
    mainwindow.cpp
    stlfileloader.cpp
    rrglwidget.cpp
    humangl.cpp
    sensorloader.cpp
    rotateparameterform.cpp
    mainwindow.ui
    rotateparameterform.ui
)

set(HEADERS
    mainwindow.h
    stlfileloader.h
    rrglwidget.h
    humangl.h
    sensorloader.h
    rotateparameterform.h
    )

add_executable(test_human3d
    ${HEADERS}
    ${SRCS}
    )

target_link_libraries(test_human3d PRIVATE Qt5::Widgets
    Qt5::OpenGL glut GLU GL pthread
    )
##
add_executable(gltest
    qt_gl_test/PointCloudOpenGLWidget.h
    qt_gl_test/PointCloudOpenGLWidget.cpp
    qt_gl_test/BaseGLWidget.h
    qt_gl_test/BaseGLWidget.cpp
    qt_gl_test/main.cpp
    )
target_link_libraries(gltest PRIVATE Qt5::Widgets
    Qt5::OpenGL glut GLU GL pthread)
