

include_directories(./)
file(GLOB_RECURSE SRC_qt_gltest2  *.cpp *.h *.c *.hpp *.ui)

add_executable(
    qt_gltest2
    ${SRC_qt_gltest2}
    )
target_link_libraries(qt_gltest2 PRIVATE ${QT_OPENGL_DEPS}
    )
