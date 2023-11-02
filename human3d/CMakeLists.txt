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
target_include_directories(test_human3d PRIVATE ./)
target_link_libraries(test_human3d PRIVATE ${QT_OPENGL_DEPS}
    )
#
