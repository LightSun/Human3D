
include_directories(./)

file(GLOB_RECURSE SRC_3DVIEWER  *.cpp *.h *.c *.hpp)

add_executable(
    3dviewer
    ${SRC_3DVIEWER}
    )
target_link_libraries(3dviewer PRIVATE ${QT_OPENGL_DEPS})
