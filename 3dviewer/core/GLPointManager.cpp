#include "GLPointManager.h"

using namespace h7_3d;

GLPointManager::GLPointManager()
{

}

//unsigned int GLPointManager::drawPointdata(std::vector<float> &pointVertexs)
//{
//    unsigned int point_count = 0;

//    glGenVertexArrays(1, &m_VAO_Point);
//    glGenBuffers(1, &m_VBO_Point);

//    glBindVertexArray(m_VAO_Point);

//    glBindBuffer(GL_ARRAY_BUFFER, m_VBO_Point);
//    glBufferData(GL_ARRAY_BUFFER, pointVertexs.size() * sizeof(float), &pointVertexs[0], GL_STATIC_DRAW);

//    // 位置属性
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
//    glEnableVertexAttribArray(0);

//    // 颜色属性
//    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
//                          (void *)(3 * sizeof(float)));
//    glEnableVertexAttribArray(1);

//    glBindBuffer(GL_ARRAY_BUFFER, 0);

//    glBindVertexArray(0);

//    point_count = (unsigned int)pointVertexs.size() / 4;

//    return point_count;
//}
