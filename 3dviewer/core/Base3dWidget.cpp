#include "Base3dWidget.h"
#include <GL/glu.h>
#include <QDebug>
#include <QMouseEvent>

using namespace h7_3d;

Base3dWidget::Base3dWidget(QWidget *parent) : QGLWidget(parent) {}

Base3dWidget::~Base3dWidget() {}

void Base3dWidget::drawGrid() {
  glPushMatrix();
  GLfloat color[] = {8.0 / 255, 108.0 / 255, 162.0 / 255};
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
  int step = 50;
  int num = 15;
  for (int i = -num; i < num + 1; i++) {
    glBegin(GL_LINES);
    glVertex3f(i * step, -num * step, 0);
    glVertex3f(i * step, num * step, 0);
    glVertex3f(-num * step, i * step, 0);
    glVertex3f(num * step, i * step, 0);
    glEnd();
  }
  glPopMatrix();
}

void Base3dWidget::drawCoordinates() {
  glPushMatrix();
  glLineWidth(2.0f);
  // glScalef(6.0f, 6.0f, 6.0f); //模型缩放函数
  glBegin(GL_LINES);
  setupColor(255, 255, 255);
  //  qglColor(QColor::fromRgbF(1, 0, 0)); // X axis is red.
  glVertex3f(-900, 0, 0);
  glVertex3f(900, 0, 0);
  setupColor(255, 255, 255);
  //  qglColor(QColor::fromRgbF(0, 1, 0)); // Y axis is green.
  glVertex3f(0, -900, 0);
  glVertex3f(0, 900, 0);
  setupColor(255, 255, 255);
  //  qglColor(QColor::fromRgbF(0, 0, 1)); // Z axis is blue.
  glVertex3f(0, 0, 0);
  glVertex3f(0, 0, 2000);
  glEnd();
  // Draw labels
  qglColor(QColor::fromRgbF(1, 1, 1));
  renderText(-900, 0, 0, "-X", QFont("helvetica", 12, QFont::Bold, true));
  renderText(900, 0, 0, "+X", QFont("helvetica", 12, QFont::Bold, true));
  qglColor(QColor::fromRgbF(1, 1, 1));
  renderText(0, -900, 0, "-Y", QFont("helvetica", 12, QFont::Bold, true));
  renderText(0, 900, 0, "+Y", QFont("helvetica", 12, QFont::Bold, true));
  qglColor(QColor::fromRgbF(1, 1, 1));
  renderText(0, 0, 2000, "+Z", QFont("helvetica", 12, QFont::Bold, true));
  glLineWidth(1.0f);
  glPopMatrix();
}

void Base3dWidget::drawSTLCoordinates(int r, int g, int b) {
  glPushMatrix();
  glLineWidth(1.5f);
  setupColor(r, g, b);
  glBegin(GL_LINES);
  glVertex3f(-300, 0, 0);
  glVertex3f(300, 0, 0);

  glVertex3f(0, -300, 0);
  glVertex3f(0, 300, 0);

  glVertex3f(0, 0, 0);
  glVertex3f(0, 0, 500);
  glEnd();
  // Draw labels
  qglColor(QColor::fromRgbF(1, 0, 0));
  renderText(-300, 0, 0, "-X", QFont("helvetica", 12, QFont::Bold, true));
  renderText(300, 0, 0, "+X", QFont("helvetica", 12, QFont::Bold, true));
  qglColor(QColor::fromRgbF(0, 1, 0));
  renderText(0, -300, 0, "-Y", QFont("helvetica", 12, QFont::Bold, true));
  renderText(0, 300, 0, "+Y", QFont("helvetica", 12, QFont::Bold, true));
  qglColor(QColor::fromRgbF(0, 0, 1));
  renderText(0, 0, 500, "+Z", QFont("helvetica", 12, QFont::Bold, true));
  glLineWidth(1.0f);
  glPopMatrix();
}

void Base3dWidget::drawGL() {
  //子类重写这个接口，显示各自的模型
  qDebug() << " RRGLWidget drawGL";
}

void Base3dWidget::setupColor(float r, float g, float b, float a) {
  /*
   * 参数1的取值可以是GL_FRONT、GL_BACK或GL_FRONT_AND_BACK，指出材质属性将应用于物体的哪面
   * 参数2指出要设置的哪种材质属性,GL_AMBIENT_AND_DIFFUSE材质的环境颜色和散射颜色
   */
  GLfloat color[] = {r, g, b, a};
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
}

void Base3dWidget::setXRotation(int angle) {
  int tangle = angle; // normalizeAngle(angle);
  if (tangle != xRot) {
    xRot = tangle;
    emit xRotationChanged(angle);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    updateGL();
  }
}

void Base3dWidget::setYRotation(int angle) {
  int tangle = angle; // normalizeAngle(angle);
  if (tangle != yRot) {
    yRot = tangle;
    emit yRotationChanged(angle);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }
}

void Base3dWidget::setXYTranslate(int dx, int dy) {
  xTran += 3.0 * dx;
  yTran -= 3.0 * dy;
  updateGL();
}

int Base3dWidget::normalizeAngle(int angle) {
  int tangle = angle;
  while (tangle < 0) {
    tangle += 360 * 16;
  }
  while (tangle > 360 * 16) {
    tangle -= 360 * 16;
  }
  return tangle;
}

void Base3dWidget::setZoom(int zoom) {
  z_zoom = zoom;
  updateGL();
}

//镜面辅助颜色
//h7:    glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
//      执行纹理贴图是才有用，可使效果逼真，其原理是：把镜面光从主颜色中抽离出来，在纹理贴图后，再把镜面光加进去
//恢复默认：   glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SINGLE_COLOR);

void Base3dWidget::initializeGL() {
  //用来初始化这个OpenGL窗口部件的，可以在里面设定一些有关选项
  GLfloat ambientLight[] = {0.7, 0.7, 0.7, 1.0}; //光源环境光强度数组
  GLfloat diffuseLight[] = {0.7, 0.8, 0.8, 1.0}; //光源散射光强度数组
  GLfloat specularLight[] = {0.4, 0.4, 0.4, 1.0}; //光源镜面反射光强度数组
  GLfloat positionLight[] = {20.0, 20.0, 20.0, 0.0}; //光源位置数组

  glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight); //设置0号光源的环境光属性
  glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight); //设置0号光源的散射光属性
  glLightfv(GL_LIGHT0, GL_SPECULAR,
            specularLight); //设置0号光源的镜面反射光属性

  //glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 1.0); //ok双面光照
  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);//双面光照

  glLightfv(GL_LIGHT0, GL_POSITION, positionLight); //设置0号光源的位置属性

  glEnable(GL_LIGHTING);   //启用光照
  glEnable(GL_LIGHT0);     //打开光源
  glEnable(GL_DEPTH_TEST); //隐藏表面消除
  glEnable(GL_NORMALIZE);

  glClearColor(0.0, 0.0, 0.0, 1.0);
}

void Base3dWidget::paintGL() {
  //called every update
}

void Base3dWidget::resizeGL(int w, int h) {
  //after this will auto refresh ui.
  if (w < 0 || h < 0) {
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

void Base3dWidget::mousePressEvent(QMouseEvent *event) { lastPos = event->pos(); }

void Base3dWidget::mouseMoveEvent(QMouseEvent *event) {
  int dx = event->x() - lastPos.x();
  int dy = event->y() - lastPos.y();
  //  qDebug() << dx << dy;
  if (event->buttons() & Qt::LeftButton)
  {
    setXRotation(xRot + 4 * dy);
    setYRotation(yRot - 4 * dx);
  } else if (event->buttons() & Qt::RightButton) {
    setZoom(z_zoom + 5.0 * dy);
  } else if (event->buttons() & Qt::MidButton) {
    setXYTranslate(dx, dy);
  }
  lastPos = event->pos();
}
