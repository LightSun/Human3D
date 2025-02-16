#include "PointCloudOpenGLWidget.h"
#include <QDebug>
#include <QWheelEvent>

PointCloudOpenGLWidget::PointCloudOpenGLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
    m_xRotate = -30.0;
    m_zRotate = 100.0;
    m_xTrans = 0.0;
    m_yTrans = 0.0;
    m_zoom = 45.0;
}

PointCloudOpenGLWidget::~PointCloudOpenGLWidget()
{
    makeCurrent();
    glDeleteBuffers(1, &m_VBO_MeshLine);
    glDeleteVertexArrays(1, &m_VAO_MeshLine);

    glDeleteBuffers(1, &m_VBO_Axis);
    glDeleteVertexArrays(1, &m_VAO_Axis);

    glDeleteBuffers(1, &m_VBO_Point);
    glDeleteVertexArrays(1, &m_VAO_Point);

    m_shaderProgramMesh.release();
    m_shaderProgramAxis.release();
    m_shaderProgramPoint.release();

    doneCurrent();
    qDebug() << __FUNCTION__;
}

void PointCloudOpenGLWidget::updatePoints(const QVector<QVector3D> &points)
{
    m_pointData.clear();
    for(auto& vector3D : points)
    {
        m_pointData.push_back(vector3D.x());
        m_pointData.push_back(vector3D.y());
        m_pointData.push_back(vector3D.z());
        m_pointData.push_back(1);
    }
}

void PointCloudOpenGLWidget::setupColor(float r, float g, float b, float a) {
  /*
   * 参数1的取值可以是GL_FRONT、GL_BACK或GL_FRONT_AND_BACK，指出材质属性将应用于物体的哪面
   * 参数2指出要设置的哪种材质属性,GL_AMBIENT_AND_DIFFUSE材质的环境颜色和散射颜色
   */
  GLfloat color[] = {r, g, b, a};
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
}
void PointCloudOpenGLWidget::loadTestData(){
    //x, y , z. r, g b
//    const float vertices[] = {
//        -0.5f, -0.5f, 0.0f, // left
//         0.5f, -0.5f, 0.0f, // right
//         0.0f,  0.5f, 0.0f  // top
//    };
    for(int i = 0 ; i < 9 ; i ++){
        auto p = (i + 1) / 2000 + 0.5;
        m_pointData.push_back(i + 100);
        m_pointData.push_back(i + 100);
        m_pointData.push_back(i + 100);
        m_pointData.push_back(1);
       // m_pointData.push_back(0);
      //  m_pointData.push_back(0);
    }
}
void PointCloudOpenGLWidget::loadCsvFile(const QString &path)
{
    m_pointData.clear();
    QFile inFile(path);
    if (inFile.open(QIODevice::ReadOnly))
    {
        QTextStream stream_text(&inFile);
        while (!stream_text.atEnd())
        {
            QString line = stream_text.readLine();
            QStringList strSplit = line.split(",");
            int count = strSplit.size() / 3;
            for(int i = 0 ; i < count; ++i){
                double x = strSplit.value(i * 3).toDouble();
                double y = strSplit.value(i * 3 + 1).toDouble();
                double z = strSplit.value(i * 3 + 2).toDouble();
                m_pointData.push_back(x);
                m_pointData.push_back(y);
                m_pointData.push_back(z);
                m_pointData.push_back(1);
            }
        }
        inFile.close();
    }
}

void PointCloudOpenGLWidget::initializeGL()
{
    initializeOpenGLFunctions();

    // enable depth_test
    glEnable(GL_DEPTH_TEST);

    // link meshline shaders   vs文件为顶点着色器  fs为片段着色器
    QString dir = "/home/heaven7/heaven7/study/github/mine_clone/Human3D/qt_gl_test/shader";
    m_shaderProgramMesh.addShaderFromSourceFile(QOpenGLShader::Vertex,
            dir + "/shader_mesh.vs");
    m_shaderProgramMesh.addShaderFromSourceFile(QOpenGLShader::Fragment,
            dir + "/shader_mesh.fs");
    m_shaderProgramMesh.link();

    // link coordinate axis shaders
    m_shaderProgramAxis.addShaderFromSourceFile(QOpenGLShader::Vertex,
            dir + "/shader_axis.vs");
    m_shaderProgramAxis.addShaderFromSourceFile(QOpenGLShader::Fragment,
            dir + "/shader_axis.fs");
    m_shaderProgramAxis.link();

    // link pointcloud shaders
    m_shaderProgramPoint.addShaderFromSourceFile(QOpenGLShader::Vertex,
            dir + "/shader_point.vs");
    m_shaderProgramPoint.addShaderFromSourceFile(QOpenGLShader::Fragment,
            dir + "/shader_point.fs");
    m_shaderProgramPoint.link();

    m_vertexCount = drawMeshline(2.0, 16);
    m_pointCount = drawPointdata(m_pointData);
    qDebug() << "point_count" << m_pointCount;
    drawCooraxis(4.0);
}

void PointCloudOpenGLWidget::paintGL()
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /*
       为了将坐标从一个坐标系转换到另一个坐标系，需要用到几个转换矩阵，
       分别是模型(Model)、视图(View)、投影(Projection)三个矩阵。
    */
    QMatrix4x4 projection, view, model;
    //透视矩阵变换
    projection.perspective(m_zoom, (float)width() / (float)height(), 1.0f, 100.0f);

    // eye：摄像机位置  center：摄像机看的点位 up：摄像机上方的朝向
    view.lookAt(QVector3D(0.0, 0.0, 50.0),
                QVector3D(0.0, 0.0, 1.0),
                QVector3D(0.0, 1.0, 0.0));

    model.translate(m_xTrans, m_yTrans, 0.0);
    model.rotate(m_xRotate, 1.0, 0.0, 0.0);
    model.rotate(m_zRotate, 0.0, 0.0, 1.0);

    m_shaderProgramMesh.bind();
    m_shaderProgramMesh.setUniformValue("projection", projection);
    m_shaderProgramMesh.setUniformValue("view", view);
    m_shaderProgramMesh.setUniformValue("model", model);

    m_shaderProgramAxis.bind();
    m_shaderProgramAxis.setUniformValue("projection", projection);
    m_shaderProgramAxis.setUniformValue("view", view);
    m_shaderProgramAxis.setUniformValue("model", model);

    m_shaderProgramPoint.bind();
    m_shaderProgramPoint.setUniformValue("projection", projection);
    m_shaderProgramPoint.setUniformValue("view", view);
    m_shaderProgramPoint.setUniformValue("model", model);

    //画网格
    m_shaderProgramMesh.bind();
    glBindVertexArray(m_VAO_MeshLine);
    glLineWidth(1.0f);
    glDrawArrays(GL_LINES, 0, m_vertexCount);

    //画坐标轴
    m_shaderProgramAxis.bind();
    glBindVertexArray(m_VAO_Axis);
    glLineWidth(5.0f);
    glDrawArrays(GL_LINES, 0, 6);

    //setupColor(1, 0, 0, 1);
    //画点云
    m_shaderProgramPoint.bind();
    glBindVertexArray(m_VAO_Point);
    //glPointSize(1.0f);
    glDrawArrays(GL_TRIANGLES, 0, m_pointCount);
}

void PointCloudOpenGLWidget::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
}

void PointCloudOpenGLWidget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
}

void PointCloudOpenGLWidget::mouseMoveEvent(QMouseEvent *event)
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

void PointCloudOpenGLWidget::wheelEvent(QWheelEvent *event)
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

unsigned int PointCloudOpenGLWidget::drawMeshline(float size, int count)
{
    std::vector<float> mesh_vertexs;
    unsigned int vertex_count = 0;

    float start = count * (size / 2);
    float posX = start, posZ = start;

    for (int i = 0; i <= count; ++i)
    {
        mesh_vertexs.push_back(posX);
        mesh_vertexs.push_back(start);
        mesh_vertexs.push_back(0);

        mesh_vertexs.push_back(posX);
        mesh_vertexs.push_back(-start);
        mesh_vertexs.push_back(0);

        mesh_vertexs.push_back(start);
        mesh_vertexs.push_back(posZ);
        mesh_vertexs.push_back(0);

        mesh_vertexs.push_back(-start);
        mesh_vertexs.push_back(posZ);
        mesh_vertexs.push_back(0);

        posX = posX - size;
        posZ = posZ - size;
    }

    glGenVertexArrays(1, &m_VAO_MeshLine);
    glGenBuffers(1, &m_VBO_MeshLine);

    glBindVertexArray(m_VAO_MeshLine);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO_MeshLine);

    glBufferData(GL_ARRAY_BUFFER, mesh_vertexs.size() * sizeof(float), &mesh_vertexs[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    vertex_count = (int)mesh_vertexs.size() / 3;

    return vertex_count;
}

void PointCloudOpenGLWidget::drawCooraxis(float length)
{
    std::vector<float> axis_vertexs =
    {
        //x,y ,z ,r, g, b
        0.0, 0.0, 0.0, 1.0, 0.0, 0.0,
        length, 0.0, 0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 0.0, 0.0, 1.0, 0.0,
        0.0, length, 0.0, 0.0, 1.0, 0.0,
        0.0, 0.0, 0.0, 0.0, 0.0, 1.0,
        0.0, 0.0, length, 0.0, 0.0, 1.0,
    };

    glGenVertexArrays(1, &m_VAO_Axis);
    glGenBuffers(1, &m_VBO_Axis);

    glBindVertexArray(m_VAO_Axis);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO_Axis);
    glBufferData(GL_ARRAY_BUFFER, axis_vertexs.size() * sizeof(float), &axis_vertexs[0], GL_STATIC_DRAW);

    // 位置属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // 颜色属性
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                          (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}

unsigned int PointCloudOpenGLWidget::drawPointdata(std::vector<float> &pointVertexs)
{
    unsigned int point_count = 0;

    glGenVertexArrays(1, &m_VAO_Point);
    glGenBuffers(1, &m_VBO_Point);

    glBindVertexArray(m_VAO_Point);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO_Point);
    glBufferData(GL_ARRAY_BUFFER, pointVertexs.size() * sizeof(float),
                 &pointVertexs[0], GL_STATIC_DRAW);
//GL_TEXTURE_COORD_ARRAY
    // 位置属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // 颜色属性
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                          (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    point_count = (unsigned int)pointVertexs.size() / 2;

    return point_count;
}
