#include "BaseGLWidget.h"
#include <GL/glu.h>
#include <QDebug>
#include <QMouseEvent>
 
BaseGLWidget::BaseGLWidget(QWidget *parent)
    : QGLWidget(parent)
{
    m_xRotate = -2276;
    m_yRotate = 1736;
    m_zoom    = -20;
    m_xTrans  = 0;
    m_yTrans  = 0;
}
 
BaseGLWidget::~BaseGLWidget() {}
 
void BaseGLWidget::initializeGL()
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
}
 
void BaseGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); /* 清除屏幕和深度缓存 */
    glPushMatrix();
    glTranslated(0, 0, m_zoom);
    glTranslated(m_xTrans, m_yTrans, 0);
    glRotated(m_xRotate / 16.0, 1.0, 0.0, 0.0); /* 绕x轴旋转 */
    glRotated(m_yRotate / 16.0, 0.0, 1.0, 0.0); /* 绕y轴旋转 */
    glRotated(+90.0, 1.0, 0.0, 0.0);
    drawGL();
    glPopMatrix();
}
 
void BaseGLWidget::resizeGL(int w, int h)
{
    if (w < 0 || h < 0)
    {
        return;
    }
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(35.0, w / float(h), 1.0, 20000.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslated(0.0, 0.0, -40.0);
}
 
void BaseGLWidget::setupColor(float r, float g, float b, float a) {
  /*
   * 参数1的取值可以是GL_FRONT、GL_BACK或GL_FRONT_AND_BACK，指出材质属性将应用于物体的哪面
   * 参数2指出要设置的哪种材质属性,GL_AMBIENT_AND_DIFFUSE材质的环境颜色和散射颜色
   */
  GLfloat color[] = {r, g, b, a};
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
}
void BaseGLWidget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
}
 
void BaseGLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();
    if (event->buttons() & Qt::LeftButton)
    {
        setXRotation(m_xRotate + 4 * dy);
        setYRotation(m_yRotate - 4 * dx);
    }
    else if (event->buttons() & Qt::MidButton)
    {
        setXYTranslate(0.5 * dx, 0.5 * dy);
    }
    lastPos = event->pos();
}
 
void BaseGLWidget::wheelEvent(QWheelEvent *event)
{
    auto scroll_offest = event->angleDelta().y() / 120;
    setZoom(m_zoom + (float)scroll_offest);
}
 
void BaseGLWidget::setGLMaterialColor(int r, int g, int b)
{
    /*
     * 参数1的取值可以是GL_FRONT、GL_BACK或GL_FRONT_AND_BACK，指出材质属性将应用于物体的哪面
     * 参数2指出要设置的哪种材质属性,GL_AMBIENT_AND_DIFFUSE材质的环境颜色和散射颜色
     */
    GLfloat color[] = {r / 255.0f, g / 255.0f, b / 255.0f};
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
}
 
void BaseGLWidget::setXRotation(int angle)
{
    int tangle = angle;
    if (tangle != m_xRotate)
    {
        m_xRotate = tangle;
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        updateGL();
    }
}
 
void BaseGLWidget::setYRotation(int angle)
{
    int tangle = angle;
    if (tangle != m_yRotate)
    {
        m_yRotate = tangle;
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
}
 
void BaseGLWidget::setXYTranslate(int dx, int dy)
{
    m_xTrans += dx;
    m_yTrans -= dy;
    updateGL();
}
 
void BaseGLWidget::setZoom(int zoom)
{
    m_zoom = zoom;
    if(m_zoom < -300)
    {
        m_zoom = -300;
    }
    if(m_zoom > 35)
    {
        m_zoom = 35;
    }
    updateGL();
}
 
void BaseGLWidget::drawMeshline(float size, int count)
{
    glPushMatrix();
    GLfloat color[] = {8.0f / 255, 108.0f / 255, 162.0f / 255};
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
    float start = count * (size / 2);
    float posX = start, posZ = start;
    for (int i = 0; i <= count; ++i)
    {
        glBegin(GL_LINES);
        glVertex3f(posX, start, 0);
        glVertex3f(posX, -start, 0);
        glVertex3f(start, posZ, 0);
        glVertex3f(-start, posZ, 0);
        glEnd();
 
        posX = posX - size;
        posZ = posZ - size;
    }
    glPopMatrix();
}
 
void BaseGLWidget::drawCoordAxis(float length)
{
    glPushMatrix();
 
    glLineWidth(4.0f);
 
    glBegin(GL_LINES);
    setGLMaterialColor(255, 0, 0);  // X axis is red.
    glVertex3f(0, 0, 0);
    glVertex3f(length, 0, 0);
    setGLMaterialColor(0, 255, 0);  // Y axis is green.
 
    glVertex3f(0, 0, 0);
    glVertex3f(0, length, 0);
    setGLMaterialColor(0, 0, 255);   // Z axis is blue.
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, length);
    glEnd();
 
    // Draw labels
    glColor3f(0.0, 1.0, 1.0);
    renderText(0, 0, length, "+Z", QFont("helvetica", 12, QFont::Bold, true));
 
    glColor3f(1.0, 0.0, 0.0);
    renderText(length, 0, 0, "+X", QFont("helvetica", 12, QFont::Bold, true));
 
    glColor3f(0.0, 1.0, 0.0);
    renderText(0, length, 0, "+Y", QFont("helvetica", 12, QFont::Bold, true));
 
    glLineWidth(1.0f);
    glPopMatrix();
}
