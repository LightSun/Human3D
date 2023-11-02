#include "PointCloudOpenGLWidget.h"
#include <QApplication>

int main(int argc, char* argv[]){

    QApplication app(argc, argv);
    QString file = "/home/heaven7/heaven7/study/github/mine_clone/"
                   "Human3D/qt_gl_test/res/plot.csv";

    PointCloudOpenGLWidget w;
    //w.loadCsvFile(file);
    w.loadTestData();
    w.resize(800, 600);
    w.show();
    return app.exec();
}
