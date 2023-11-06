#pragma once

#include <QOpenGLFunctions_3_3_Core>
#include "stlfileloader.h"

namespace h7_3d {
class Module3DManager
{
public:
    using Module3D = h7_3d::Module3D;
    using Module3DBatch = h7_3d::Module3DBatch;
    using Vec3F = h7_3d::Vec3F;
    using CModule3D = const Module3D&;
    using CModule3DBatch = const Module3DBatch&;
    using CVec3F = const Vec3F&;

    Module3DManager();

    void onInitGL(QOpenGLFunctions_3_3_Core*);
    void onDestroy();

    void draw3dModel();

private:
    QOpenGLFunctions_3_3_Core* m_gl;
    QList<h7_3d::Module3D> m_models;
    QList<h7_3d::Module3DBatch> m_batches;
};

}


