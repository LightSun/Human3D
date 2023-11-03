#pragma once

#include <vector>

namespace h7_3d {

class GLPointManager
{
public:
    GLPointManager();


private:
    unsigned int m_VBO_MeshLine;
    unsigned int m_VAO_MeshLine;

    unsigned int m_VBO_Axis;
    unsigned int m_VAO_Axis;

    unsigned int m_VBO_Point;
    unsigned int m_VAO_Point;

    std::vector<float> m_pointData;
};

}
