include_directories(./)

file(GLOB_RECURSE SRC_QOPENGL_TEST  *.cpp *.h *.c *.hpp *.ui)

add_executable(
    qopenglw_test
    ${SRC_QOPENGL_TEST}
    texture.qrc
    )
target_link_libraries(qopenglw_test PRIVATE ${QT_OPENGL_DEPS})
