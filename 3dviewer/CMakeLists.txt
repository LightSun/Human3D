
include_directories(./)

add_compile_definitions(BUILD_GLTOOLS_WITH_QT)
# GLtools
#set(GLTools_DIR /home/heaven7/heaven7/env/linux/GLTools)
#include_directories(${GLTools_DIR}/include)
#SET(GLTools_LIBS ${GLTools_DIR}/libs/libgltools.so)
#
include_directories(${CMAKE_SOURCE_DIR}/GLTools/include)
SET(GLTools_LIBS gltools)

## src
file(GLOB_RECURSE SRC_3DVIEWER  *.cpp *.h *.c *.hpp)

add_executable(
    3dviewer
    ${SRC_3DVIEWER}
    )
target_link_libraries(3dviewer PRIVATE ${QT_OPENGL_DEPS} ${GLTools_LIBS})
