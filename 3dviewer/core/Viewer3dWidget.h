#pragma once

#include "core/Base3dWidget.h"
#include <QList>


class Viewer3dWidget: public h7_3d::Base3dWidget
{
public:
    using Module3D = h7_3d::Module3D;
    using Module3DBatch = h7_3d::Module3DBatch;
    using Vec3F = h7_3d::Vec3F;
    using CModule3D = const Module3D&;
    using CModule3DBatch = const Module3DBatch&;
    using CVec3F = const Vec3F&;

    Viewer3dWidget(QWidget *parent = 0);

    void addModule3D(CModule3D m){
        m_models.push_back(m);
    }
    void addModule3DBatch(CModule3DBatch m){
        m_batches.push_back(m);
    }
    void setWholeRotateAngle(CVec3F vec){
        m_wholeAngle = vec;
    }

    void drawGL() override;

protected:
   //void paintGL() override;
    void onInitGL() override;

private:
    void draw3dModel();
    void drawTest();

private:
    Vec3F m_wholeAngle;
    QList<h7_3d::Module3D> m_models;
    QList<h7_3d::Module3DBatch> m_batches;
};

