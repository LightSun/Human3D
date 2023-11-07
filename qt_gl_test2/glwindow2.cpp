#include "glwindow2.h"
#include <QMouseEvent>
#include <QOpenGLShaderProgram>
#include <QDebug>

GLWindow2::GLWindow2(QVector<float> vPoints, QWidget *parent)
    : QOpenGLWidget(parent)
{
    m_shaderM.setVertexData(vPoints);
    m_shaderM.setColor(QColor(255, 0, 0));

    m_xRotate = -30.0;
    m_zRotate = 100.0;
    m_xTrans = 0.0;
    m_yTrans = 0.0;
    m_zoom = 45.0;
}

GLWindow2::~GLWindow2()
{
    cleanup();
}

QSize GLWindow2::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize GLWindow2::sizeHint() const
{
    return QSize(800, 600);
}

static void qNormalizeAngle(int &angle)
{
    while (angle < 0)
        angle += 360 * 16;
    while (angle > 360 * 16)
        angle -= 360 * 16;
}

void GLWindow2::setXRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != m_xRotate)
    {
        m_xRotate = angle;
        update();
    }
}

void GLWindow2::setYRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != m_yRotate)
    {
        m_yRotate = angle;
        update();
    }
}

void GLWindow2::setZRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != m_zRotate)
    {
        m_zRotate = angle;
        update();
    }
}

void GLWindow2::cleanup()
{
    makeCurrent();
    m_shaderM.onDestroy();
    doneCurrent();
}

void GLWindow2::initializeGL()
{
    // In this example the widget's corresponding top-level window can change
    // several times during the widget's lifetime. Whenever this happens, the
    // QOpenGLWidget's associated context is destroyed and a new one is created.
    // Therefore we have to be prepared to clean up the resources on the
    // aboutToBeDestroyed() signal, instead of the destructor. The emission of
    // the signal will be followed by an invocation of initializeGL() where we
    // can recreate all resources.
    connect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &GLWindow2::cleanup);

    initializeOpenGLFunctions();

    m_shaderM.onInitGl();
    glClearColor(0, 0, 0, 1);
}

void GLWindow2::paintGL()
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    /*
          为了将坐标从一个坐标系转换到另一个坐标系，需要用到几个转换矩阵，
          分别是模型(Model)、视图(View)、投影(Projection)三个矩阵。
    */
    QMatrix4x4 proj, view, model;
    //透视矩阵变换
    proj.perspective(m_zoom, (float)width() / (float)height(), 1.0f, 100.0f);

    // eye：摄像机位置  center：摄像机看的点位 up：摄像机上方的朝向
    view.lookAt(QVector3D(0.0, 0.0, 50.0),
               QVector3D(0.0, 0.0, 1.0),
               QVector3D(0.0, 1.0, 0.0));

    model.translate(m_xTrans, m_yTrans, 0.0);
    model.rotate(m_xRotate, 1.0, 0.0, 0.0);
    model.rotate(m_yRotate, 0.0, 1.0, 0.0);
    model.rotate(m_zRotate, 0.0, 0.0, 1.0);

    m_shaderM.onDraw(model, view, proj);

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
}

void GLWindow2::resizeGL(int w, int h)
{
     glViewport(0, 0, w, h);
}

void GLWindow2::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
}

void GLWindow2::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->pos().x() - lastPos.x();
    int dy = event->pos().y() - lastPos.y();
    if (event->buttons() & Qt::LeftButton)
    {
        m_xRotate = m_xRotate + 0.3 * dy;
        m_zRotate = m_zRotate + 0.3 * dx;

        if (m_xRotate > 30.0f)
        {
            m_xRotate = 30.0f;
        }
        if (m_xRotate < -120.0f)
        {
            m_xRotate = -120.0f;
        }
        update();
    }
    else if (event->buttons() & Qt::MidButton)
    {
        m_xTrans = m_xTrans + 0.1 * dx;
        m_yTrans = m_yTrans - 0.1 * dy;
        update();
    }
    lastPos = event->pos();
}

void GLWindow2::wheelEvent(QWheelEvent *event)
{
    auto scroll_offest = event->angleDelta().y() / 120;
    m_zoom = m_zoom - (float)scroll_offest;

    if (m_zoom < 1.0f)    /* 放大限制 */
    {
        m_zoom = 1.0f;
    }

    if (m_zoom > 80.0f)
    {
        m_zoom = 80.0f;
    }

    update();
}

void GLWindow2::slot_cam_X(int translateX)
{
    //m_camera_pos.setX(translateX);
    //update();
}

void GLWindow2::slot_cam_Y(int translateY)
{
    //m_camera_pos.setY(translateY);
    //update();
}

void GLWindow2::slot_cam_Z(int translateZ)
{
    //m_camera_pos.setZ(translateZ);
    //update();
}
