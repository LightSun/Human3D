#ifndef TEST3DWIDGET_H
#define TEST3DWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLFunctions>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>

class Test3dWidget: public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
public:
    using CQString = const QString&;
    Test3dWidget();

    void loadStl(CQString str, float ratio = 1.f);

protected:
    void resizeGL(int w, int h) override;
    void paintGL() override;
    void initializeGL() override;

private:
    QOpenGLShader *m_vshader1;
    QOpenGLShader *m_fshader1;
    QOpenGLShaderProgram *m_program1;
    QOpenGLBuffer m_vbo1;

    QVector<QVector3D> m_vertices;
    QVector<QVector3D> m_normals;

    int m_vertexAttr1;
    int m_normalAttr1;
    int m_matrixUniform1;
};

#endif // TEST3DWIDGET_H
