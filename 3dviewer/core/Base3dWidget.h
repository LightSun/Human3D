#pragma once

#include "core/stlfileloader.h"
#include <QGLWidget>
#include <QWidget>

namespace h7_3d {

/**
 * @brief The RRGLWidget class 基类，提供了鼠标旋转、方法缩小、平移、网格、坐标系等基本功能
 */
class Base3dWidget : public QGLWidget {

  Q_OBJECT

public:
  Base3dWidget(QWidget *parent = 0);
  ~Base3dWidget();
  /**
   * @brief drawGrid 画网格
   */
  void drawGrid();
  /**
   * @brief drawCoordinates 画坐标系
   */
  void drawCoordinates();
  /**
   * @brief drawSTLCoordinates 画每个组件变换后的坐标系
   */
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

protected:
  void initializeGL() override;
  void paintGL() override;
  void resizeGL(int w, int h) override;

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
};
}
