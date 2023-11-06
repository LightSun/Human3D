#pragma once

#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
//#include <QOpenGLFunctions_3_3_Compatibility>

namespace h7_3d {

#define VERTEX_POS_SIZE         3 // x, y and z
#define VERTEX_NORMAL_SIZE      3 // x, y and z
#define VERTEX_COLOR_SIZE       3 // r, g, b

class GLBatch
{
public:
    using QVAO = QOpenGLVertexArrayObject;
    using QVBO = QOpenGLBuffer;
    enum GLT_SHADER_ATTRIBUTE {
        GLT_ATTRIBUTE_VERTEX = 0,
        GLT_ATTRIBUTE_COLOR,
        GLT_ATTRIBUTE_NORMAL,
        GLT_ATTRIBUTE_TEXTURE0,
        GLT_ATTRIBUTE_TEXTURE1,
        GLT_ATTRIBUTE_TEXTURE2,
        GLT_ATTRIBUTE_TEXTURE3,
        GLT_ATTRIBUTE_LAST
    };

    void setQTOpenGL(QOpenGLFunctions_3_3_Core* gl){
        m_gl = gl;
    }

    void begin(int nVertex){
        m_numVerts = nVertex;
    }
//    void bindVertexNormal2(float* array){
//        m_vao.create();
//        m_vbo.create();
//        m_vao.bind();
//        m_vbo.bind();
//        m_vbo.setUsagePattern(QOpenGLBuffer::DynamicDraw);
//        m_vbo.allocate(array, m_numVerts * 6);
//        //
//        m_vbo.release();
//    }
    void bindVertexNormal(float* array){
        //glVertexAttribPointer:: p1: 要配置的定点属性，p2: 定点属性的大小，顶点由x,y,z , so = 3.
        //  p3: GL_FLOAT or etc.  p4:if normallized. if true. will map to -1~1. 一般false.
        //  p5: 步长(Stride), 连续的顶点属性组之间的间隔
        //  p6: 起始偏移量
        m_gl->glGenVertexArrays(1, &m_VAO);
        m_gl->glBindVertexArray(m_VAO);
        m_gl->glGenBuffers(1, &m_VBO);//TODO crash why?
        //buffer: vertex and normal.
        m_gl->glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        m_gl->glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * m_numVerts,
                             array, GL_DYNAMIC_DRAW);
        //vertex
        m_gl->glEnableVertexAttribArray(GLT_ATTRIBUTE_VERTEX);
        m_gl->glVertexAttribPointer(GLT_ATTRIBUTE_VERTEX, 3,
                                    GL_FLOAT, GL_FALSE,
                                    6 * sizeof(float),
                                    (void *)0);
        //normal
        m_gl->glEnableVertexAttribArray(GLT_ATTRIBUTE_NORMAL);
        m_gl->glVertexAttribPointer(GLT_ATTRIBUTE_NORMAL, 3,
                                    GL_FLOAT, GL_FALSE,
                                    6 * sizeof(float),
                                    (void *)(3 * sizeof(float)));
        //
        m_gl->glBindBuffer(GL_ARRAY_BUFFER, 0);
        m_gl->glBindVertexArray(0);
    }

    void drawTriangles(){
        m_gl->glBindVertexArray(m_VAO);
        glDrawArrays(GL_TRIANGLES, 0, m_numVerts/3);
        //release VAO
        m_gl->glBindVertexArray(0);
    }

    void drawPoints(){
        m_gl->glBindVertexArray(m_VAO);
        glDrawArrays(GL_POINTS, 0, m_numVerts);
        //release VAO
        m_gl->glBindVertexArray(0);
    }

    void destroy(){
        m_gl->glDeleteBuffers(1, &m_VBO);
        m_gl->glDeleteVertexArrays(1, &m_VAO);
        m_VAO = 0;
        m_VBO = 0;
    }

private:
    QOpenGLFunctions_3_3_Core* m_gl;
    QVAO m_vao;
    QVBO m_vbo;
    unsigned int m_VBO {0};
    unsigned int m_VAO {0};
    unsigned int m_numVerts {0};
};

}
