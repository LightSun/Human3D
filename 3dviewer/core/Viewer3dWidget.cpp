#include "Viewer3dWidget.h"

Viewer3dWidget::Viewer3dWidget(QWidget *parent):Base3dWidget(parent)
{

}

void Viewer3dWidget::drawGL(){
    drawGrid();
    drawCoordinates();
    draw3dModel();
}
void Viewer3dWidget::draw3dModel(){
    foreach(CModule3DBatch m, m_batches) {
        glPushMatrix();
        setupColor(m.r, m.g, m.b, m.a);
        if(m.translate.isNotAllZero()){
            glTranslatef(m.translate.v1, m.translate.v2, m.translate.v3);
        }
        if(m.rotate.isNotAllZero()){
            glRotatef(m.rotate.v1, 1.0, 0.0, 0.0);
            glRotatef(m.rotate.v2, 0.0, 1.0, 0.0);
            glRotatef(m.rotate.v3, 0.0, 0.0, 1.0);
        }
        if(m.scale.isNotAllZero()){
            glScalef(m.scale.v1, m.scale.v2, m.scale.v3);
        }
        for(int i = 0, size = m.loaders.size() ; i < size ; ++i){
            m.loaders.at(i).draw();
        }
        glPopMatrix();
    }
    foreach(CModule3D _m, m_models) {
        auto& m = (Module3D&)_m;
        //glPushMatrix/glPopMatrix:  like save - restore
        glPushMatrix();
        setupColor(m.r, m.g, m.b, m.a);
        if(m.translate.isNotAllZero()){
            glTranslatef(m.translate.v1, m.translate.v2, m.translate.v3);
        }
        if(m.rotate.isNotAllZero()){
            glRotatef(m.rotate.v1, 1.0, 0.0, 0.0);
            glRotatef(m.rotate.v2, 0.0, 1.0, 0.0);
            glRotatef(m.rotate.v3, 0.0, 0.0, 1.0);
        }
        if(m.scale.isNotAllZero()){
            glScalef(m.scale.v1, m.scale.v2, m.scale.v3);
        }
        m.loader.draw();
        glPopMatrix();
    }
}
