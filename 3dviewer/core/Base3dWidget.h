#pragma once

#include "core/stlfileloader.h"
#include <QGLWidget>
#include <QOpenGLWidget>
#include <QWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>

//#define USE_OLD_GL

namespace h7_3d {

#ifdef USE_OLD_GL
typedef QGLWidget _QSuperGLWidget;
#else
typedef QOpenGLWidget _QSuperGLWidget;
#endif

/**
 * 提供了鼠标旋转、方法缩小、平移、网格、坐标系等基本功能
 */
class Base3dWidget : public _QSuperGLWidget, public QOpenGLFunctions_3_3_Core {

  Q_OBJECT

public:
  Base3dWidget(QWidget *parent = 0);
  ~Base3dWidget();

  void drawGrid();

  void drawCoordinates();

  //draw every after Coordinates changed. if need
  void drawSTLCoordinates(int r, int g, int b);

  //draw 3d models
  virtual void drawGL();

  void setupColor(float r, float g, float b, float a = 1.f);

  void setXRotation(int angle);

  void setYRotation(int angle);

  void setXYTranslate(int dx, int dy);

  int normalizeAngle(int angle);

  void setZoom(int zoom);

signals:
  void xRotationChanged(int angle);
  void yRotationChanged(int angle);
  void zRotationChanged(int angle);

  //h7
public:
#ifndef USE_OLD_GL
  void transformPoint(GLdouble out[4], const GLdouble m[16], const GLdouble in[4]);
  GLint project(GLdouble objx, GLdouble objy, GLdouble objz,
      const GLdouble model[16], const GLdouble proj[16],
      const GLint viewport[4],
      GLdouble * winx, GLdouble * winy, GLdouble * winz);
  void renderText(double x, double y, double z, const QString &str,
                  const QFont & font = QFont());
  void qglColor(const QColor& c);
  void updateGL(){
      update();
  }
#endif
  unsigned int drawPointdata(std::vector<float> &pointVertexs);

protected:
  void initializeGL() override;
  void paintGL() override;
  void resizeGL(int w, int h) override;

  virtual void onInitGL(){};

  void mousePressEvent(QMouseEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;

public:
  GLdouble xRot = -2584; // x旋转角度
  GLdouble yRot = 1376;  // y旋转角度
  GLdouble zRot = 0.0;   // z旋转角度
  int z_zoom = -2500;    // 缩放
  int xTran = 0;         // 界面显示X位置
  int yTran = 0;         // 界面显示Y位置
  QPoint lastPos;
//h7
  unsigned int m_VBO_MeshLine;
  unsigned int m_VAO_MeshLine;

  unsigned int m_VBO_Axis;
  unsigned int m_VAO_Axis;

  unsigned int m_VBO_Point;
  unsigned int m_VAO_Point;

  std::vector<float> m_pointData;
  unsigned int m_pointCount;

  unsigned int m_vertexCount;
};
}
