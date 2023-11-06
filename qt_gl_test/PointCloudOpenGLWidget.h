#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QMouseEvent>

//#include "opengllib_global.h"

//OPENGLLIBSHARED_EXPORT
class PointCloudOpenGLWidget: public QOpenGLWidget, public QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
public:
    PointCloudOpenGLWidget(QWidget *parent = 0);
    ~PointCloudOpenGLWidget();
    void updatePoints(const QVector<QVector3D> &points);
    void loadCsvFile(const QString &path);
    void loadTestData();

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height) override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

    virtual unsigned int drawMeshline(float size, int count);
    virtual void drawCooraxis(float length);
    virtual unsigned int drawPointdata(std::vector<float> &pointVertexs);

    void setupColor(float r, float g, float b, float a = 1.0f);

protected:
    QOpenGLShaderProgram m_shaderProgramMesh;
    QOpenGLShaderProgram m_shaderProgramAxis;
    QOpenGLShaderProgram m_shaderProgramPoint;

    unsigned int m_VBO_MeshLine;
    unsigned int m_VAO_MeshLine;

    unsigned int m_VBO_Axis;
    unsigned int m_VAO_Axis;

    unsigned int m_VBO_Point;
    unsigned int m_VAO_Point;

    std::vector<float> m_pointData;
    unsigned int m_pointCount;

    unsigned int m_vertexCount;

    float m_xRotate;
    float m_zRotate;
    float m_xTrans;
    float m_yTrans;
    float m_zoom;

    QPoint lastPos;
};
