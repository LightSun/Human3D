
#include <QApplication>
#include "Viewer3dWidget.h"

using namespace h7_3d;

int main(int argc, char* argv[]){
    QApplication app(argc, argv);

    QString dir = "/home/heaven7/heaven7/study/github/mine_clone/Human3D/bin/STLFile/human";
    QString stl = dir + "/A右大腿.stl";

    QString stl2 = "/home/heaven7/heaven7/work/IAS/debug_out/mvh_thy_nodule.stl";
    Module3D m;
    m.loader.setRatio(10);
    m.loader.loadStl(stl);
    m.setRgbColor(1, 0, 0);

    Viewer3dWidget v3w;
    v3w.resize(800, 600);
   // v3w.setWholeRotateAngle()
    v3w.addModule3D(m);
    v3w.show();

    return app.exec();
}
