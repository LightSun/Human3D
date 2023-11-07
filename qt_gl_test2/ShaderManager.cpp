#include <QOpenGLShaderProgram>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QMatrix4x4>
#include <QVector2D>
#include <QVector3D>
#include "ShaderManager.h"

using namespace h7_3d;

static const char * _vertex_shader_src =
        "#version 330 core\n"
        "in vec3 vertex;\n"
        "in vec3 normal;\n"
        "attribute mediump vec4 color;\n"
        "varying mediump vec4 ourColor;\n"
        "varying mediump vec3 vertNormal;\n"
        "uniform mat4 projMatrix;\n"
        "uniform mat4 mvMatrix;\n"
        "uniform mat3 normalMatrix;\n"
        "void main() {\n"
        "   vertNormal = normalMatrix * normal;\n"
        "   ourColor = color;\n"
        "   gl_Position = projMatrix * mvMatrix * vec4(vertex,1.0);\n"
        "}\n";

static const char* _frag_shader_src =
        "#version 330 core\n"
        "varying mediump vec4 ourColor;\n"
        "void main()\n"
        "{\n"
        "   gl_FragColor = ourColor;\n"
        "}\n";

namespace h7_3d {

using CMat4 = const QMatrix4x4&;

struct _ShaderManager_ctx{
    QOpenGLShaderProgram* m_program;
    int m_loc_proj;
    int m_loc_mv; //model, view
    int m_loc_normalMat;
    int m_loc_color;
    QOpenGLBuffer m_Vbo;
    //x,y,z, n1,n2,n3, x,y,z, n1,n2,n3, x,y,z, n1,n2,n3
    QVector<float> m_datas;
    int m_shader_type;
    QColor m_color;

    void setData(int type, const QVector<float>& data){
        m_shader_type = type;
        m_datas = data;
    }

    void _check_link(){
        if (m_program->link())
        {
            qDebug() << "link success!";
        }
        else
        {
            qDebug() << "link failed";
        }
    }

    void createShader(){
        m_program = new QOpenGLShaderProgram;
        m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, _vertex_shader_src);
        m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, _frag_shader_src);
        _check_link();
        m_program->bindAttributeLocation("vertex", 0);
        m_program->bindAttributeLocation("normal", 1);
        _check_link();
        m_program->bind();

        m_loc_proj = m_program->uniformLocation("projMatrix");
        m_loc_mv = m_program->uniformLocation("mvMatrix");
        m_loc_normalMat = m_program->uniformLocation("normalMatrix");
        m_loc_color = m_program->attributeLocation("color");
        //
        m_Vbo.create();
        m_Vbo.bind();
        m_Vbo.allocate(m_datas.data(), m_datas.size() * sizeof(float));

        setVertexAttrs();
        m_program->setAttributeValue(m_loc_color, m_color);
        m_program->release();
    }

    void setVertexAttrs(){
        m_Vbo.bind();
        QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
        switch (m_shader_type) {
        case kShaderT_VERTEX_NORMAL:{
            f->glEnableVertexAttribArray(0);
            f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                                     6 * sizeof(float), (void *)0);

            f->glEnableVertexAttribArray(1);
            f->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
                                     6 * sizeof(float), (void *)(3 * sizeof(float)));
        }break;
        case kShaderT_VERTEX:{
            f->glEnableVertexAttribArray(0);
            f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                                     3 * sizeof(float), (void *)0);
            f->glEnableVertexAttribArray(1);
            f->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
                                         3 * sizeof(GLfloat),
                                         reinterpret_cast<void *>(3 * sizeof(GLfloat)));
        }break;
        case kShaderT_VERTEX_COLOR:{
            //TODO
        }break;

        default:{
            std::string msg = " wrong shader type = " + std::to_string(m_shader_type);
            Q_ASSERT_X(false, "setVertexAttrs", msg.data());
        }
        }
        m_Vbo.release();
    }

    void draw(CMat4 model, CMat4 view, CMat4 proj){
        m_program->bind();
        m_program->setUniformValue(m_loc_proj, proj);
        m_program->setUniformValue(m_loc_mv, model * view);
        m_program->setUniformValue(m_loc_normalMat, model.normalMatrix());

        int vertex_size = 0;
        switch (m_shader_type) {
        case kShaderT_VERTEX_NORMAL:{
            vertex_size = m_datas.size() / 2;
        }break;
        case kShaderT_VERTEX:{
            vertex_size = m_datas.size();
        }break;
        case kShaderT_VERTEX_COLOR:{
            vertex_size = m_datas.size() / 2;
        }break;
        default:{
            std::string msg = " wrong shader type = " + std::to_string(m_shader_type);
            Q_ASSERT_X(false, "draw", msg.data());
        }
        }
        glDrawArrays(GL_TRIANGLES, 0, vertex_size);
        m_program->release();
    }
    void destroy(){
        if(m_program){
            m_Vbo.destroy();
            delete m_program;
            m_program = nullptr;
        }
    }
};
}

ShaderManager::ShaderManager()
{
    m_ptr = new _ShaderManager_ctx();
}
ShaderManager::~ShaderManager()
{
    if(m_ptr){
        delete m_ptr;
        m_ptr = nullptr;
    }
}
void ShaderManager::setVertexData(const QVector<float>& data){
    m_ptr->setData(kShaderT_VERTEX, data);
}
void ShaderManager::setVertexNormalData(const QVector<float>& data){
    m_ptr->setData(kShaderT_VERTEX_NORMAL, data);
}
void ShaderManager::setColor(const QColor& c){
    m_ptr->m_color = c;
}
void ShaderManager::onInitGl(){
    m_ptr->createShader();
}
void ShaderManager::onDraw(CMat4 model, CMat4 view, CMat4 proj){
    m_ptr->draw(model, view, proj);
}
void ShaderManager::onDestroy(){
    m_ptr->destroy();
}
