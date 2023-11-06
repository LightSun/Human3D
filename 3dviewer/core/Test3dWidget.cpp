#include "Test3dWidget.h"
#include <QFile>

Test3dWidget::Test3dWidget()
{

}

void Test3dWidget::loadStl(CQString filename, float ratio){
    qDebug() << "load STL text file:" << filename;
    QFile file(filename);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
          while (!file.atEnd()) {
                QString line =
                    file.readLine().trimmed(); // trimmed去除了开头和结尾的空白字符串
                QStringList words = line.split(' ', QString::SkipEmptyParts);
                if (words[0] == "facet") {
                    auto v3d = QVector3D(words[2].toFloat(),
                                                words[3].toFloat(),
                                                words[4].toFloat());
                    m_normals << v3d;
                    m_normals << v3d;
                    m_normals << v3d;
                }
                if (words[0] == "vertex") {
                    m_vertices << QVector3D(ratio *words[1].toFloat(),
                            ratio *words[2].toFloat(),
                            ratio *words[3].toFloat());
                }
                if (words[0] == "endloop") {
                    //check vertex. normals.
                }
          }
          file.close();
    }else{
        Q_ASSERT_X(false, "STLFileLoader::loadTextStl", "open fale failed.");
    }
}
void Test3dWidget::resizeGL(int w, int h){
     glViewport(0, 0, w, h);
}
void Test3dWidget::paintGL(){
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glFrontFace(GL_CW);
    glCullFace(GL_FRONT);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    QMatrix4x4 modelview;
    modelview.rotate(1.f, 0.0f, 1.0f, 0.0f);
    modelview.rotate(1.f, 1.0f, 0.0f, 0.0f);
    modelview.rotate(1.f, 0.0f, 0.0f, 1.0f);
    modelview.scale(1.f);
    modelview.translate(0.0f, -0.2f, 0.0f);

    m_program1->bind();
    m_program1->setUniformValue(m_matrixUniform1, modelview);
    //
    m_program1->enableAttributeArray(m_vertexAttr1);
    m_program1->enableAttributeArray(m_normalAttr1);

    m_vbo1.bind();
    // The data in the buffer is placed like this:
    // vertex1.x, vertex1.y, vertex1.z, normal1.x, normal1.y, normal1.z, vertex2.x, ...
    m_program1->setAttributeBuffer(m_vertexAttr1, GL_FLOAT, 0,
                                   3, 6 * sizeof(GLfloat));
    m_program1->setAttributeBuffer(m_normalAttr1, GL_FLOAT,
                                   3 * sizeof(GLfloat),
                                   3, 6 * sizeof(GLfloat));
    m_vbo1.release();

    glDrawArrays(GL_TRIANGLES, 0, m_vertices.size());

    m_program1->disableAttributeArray(m_normalAttr1);
    m_program1->disableAttributeArray(m_vertexAttr1);
    //
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
}
void Test3dWidget::initializeGL(){
    initializeOpenGLFunctions();
    //compile shader
    m_vshader1 = new QOpenGLShader(QOpenGLShader::Vertex);
    const char *vsrc1 =
        "layout (location = 0) in vec3 aPos;\n"
        "attribute mediump vec3 normal;\n"
        "uniform mat4 model;\n"
        "uniform mat4 view;"
        "uniform mat4 projection;"
        "uniform vec3 in_color;"
        "varying mediump vec4 color;\n"
        "void main(void)\n"
        "{\n"
        "    color = vec4(in_color, 1.0);\n"
        "    gl_Position = model * view * projection * vec4(aPos，1.0);\n"
        "}\n";
    m_vshader1->compileSourceCode(vsrc1);

    m_fshader1 = new QOpenGLShader(QOpenGLShader::Fragment);
    const char *fsrc1 =
        "varying mediump vec4 color;\n"
        "void main(void)\n"
        "{\n"
        "    gl_FragColor = color;\n"
        "}\n";
    m_fshader1->compileSourceCode(fsrc1);

    m_program1 = new QOpenGLShaderProgram;
    m_program1->addShader(m_vshader1);
    m_program1->addShader(m_fshader1);
    m_program1->link();

    m_vertexAttr1 = m_program1->attributeLocation("vertex");
    m_normalAttr1 = m_program1->attributeLocation("normal");
    m_matrixUniform1 = m_program1->uniformLocation("matrix");

    m_vbo1.create();
    m_vbo1.bind();
    // For the cube all the data belonging to the texture coordinates and
    // normals is placed separately, after the vertices. Here, for the Qt logo,
    // let's do something different and potentially more efficient: create a
    // properly interleaved data set.
    const int vertexCount = m_vertices.count();
    QVector<GLfloat> buf;
    buf.resize(vertexCount * 3 * 2);
    GLfloat *p = buf.data();
    for (int i = 0; i < vertexCount; ++i) {
        *p++ = m_vertices[i].x();
        *p++ = m_vertices[i].y();
        *p++ = m_vertices[i].z();
        *p++ = m_normals[i].x();
        *p++ = m_normals[i].y();
        *p++ = m_normals[i].z();
    }
    m_vbo1.allocate(buf.constData(), buf.count() * sizeof(GLfloat));
    m_vbo1.release();
}
