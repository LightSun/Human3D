#ifndef RRGLWIDGET_H
#define RRGLWIDGET_H
 
#include <QGLWidget>
#include <QWidget>
//#include "opengllib_global.h"
 
//https://blog.csdn.net/qq_40602000/article/details/128293336?spm=1001.2014.3001.5501
class BaseGLWidget : public QGLWidget
{
    Q_OBJECT
public:
    BaseGLWidget(QWidget *parent = 0);
    ~BaseGLWidget();
 
protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;
 
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
 
    void setGLMaterialColor(int r, int g, int b);
    void setXRotation(int angle);
    void setYRotation(int angle);
    void setXYTranslate(int dx, int dy);
    void setZoom(int zoom);
 
    virtual void drawGL() = 0;
    virtual void drawMeshline(float size = 50, int count = 15);
    virtual void drawCoordAxis(float length = 900);
 
protected:
    GLdouble m_xRotate;   /**< x旋转角度 */
    GLdouble m_yRotate;   /**< y旋转角度 */
    GLdouble m_zoom;      /**< 缩放 */
    GLdouble m_xTrans;    /**< 界面显示X位置 */
    GLdouble m_yTrans;    /**< 界面显示Y位置 */
    QPoint   lastPos;
};
#endif // RRGLWIDGET_H
