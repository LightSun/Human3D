#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QMatrix4x4>
#include <QVector2D>
#include <QVector3D>

#include "ShaderManager.h"
//#include <gl/GL.h>

class GLWindow2 : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    GLWindow2(QVector<float> vPoints, QWidget *parent = 0);
    ~GLWindow2();

    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;

public slots:
    void cleanup();
    void setXRotation(int angle);
    void setYRotation(int angle);
    void setZRotation(int angle);
    void slot_cam_X(int translateX);
    void slot_cam_Y(int translateY);
    void slot_cam_Z(int translateZ);

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

private:
    float m_xRotate {0};
    float m_yRotate {0};
    float m_zRotate {0};
    float m_xTrans {0};
    float m_yTrans {0};
    float m_zoom {1};

    QPoint lastPos;
    h7_3d::ShaderManager m_shaderM;

};
