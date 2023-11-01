#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "stlfileloader.h"
#include "sensorloader.h"
#include <QDebug>
#include <QDesktopWidget>
#include <QVBoxLayout>
#include <QStyleFactory>
#include <QFileDialog>
#include <QStyle>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  setWindowTitle("人体3D显示");
  moveWindowToCenter();
  connect(ui->rotateParaLeftArm1, &RotateParameterForm::valueChanged, this, &MainWindow::slot_OnValueChanged);
  connect(ui->rotateParaLeftArm2, &RotateParameterForm::valueChanged, this, &MainWindow::slot_OnValueChanged);

  connect(ui->rotateParaRightArm1, &RotateParameterForm::valueChanged,this, &MainWindow::slot_OnValueChanged);
  connect(ui->rotateParaRightArm2, &RotateParameterForm::valueChanged,this, &MainWindow::slot_OnValueChanged);

  connect(ui->rotateParaLeftThigh, &RotateParameterForm::valueChanged, this, &MainWindow::slot_OnValueChanged);
  connect(ui->rotateParaLeftLeg, &RotateParameterForm::valueChanged, this, &MainWindow::slot_OnValueChanged);

  connect(ui->rotateParaRightThigh, &RotateParameterForm::valueChanged,this, &MainWindow::slot_OnValueChanged);
  connect(ui->rotateParaRightLeg, &RotateParameterForm::valueChanged,this, &MainWindow::slot_OnValueChanged);

  connect(ui->rotateParaWhole, &RotateParameterForm::valueChanged,this, &MainWindow::slot_OnValueChanged);

  SensorLoader *worker = new SensorLoader;
  worker->moveToThread(&workerThread);
  connect(this, SIGNAL(open(const QString)), worker, SLOT(parser(const QString)));
  connect(this, SIGNAL(stopped()), worker, SLOT(stopWork()));
  connect(&workerThread, &QThread::finished, worker, &QObject::deleteLater);
  connect(worker, SIGNAL(resultReady(QVariant)), this, SLOT(slot_handleResult(QVariant)));
  on_btnStop_clicked();
}

MainWindow::~MainWindow() {
    workerThread.wait();
    workerThread.quit();
    delete ui;
}

void MainWindow::moveWindowToCenter() {
  //屏幕居中
  this->setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter,
                                        this->size(),
                                        qApp->desktop()->availableGeometry()));
}

void MainWindow::slot_OnValueChanged(double xA, double yA, double zA)
{
    qDebug()<< xA << yA<< zA;
    RotateParameterForm *obj = (RotateParameterForm*) sender();
    //left arm
    if(obj == ui->rotateParaLeftArm1)
    {
        ui->humanGLWidget->setLeftArm1Angle(xA,yA,zA);
    }
    else if(obj == ui->rotateParaLeftArm2)
    {
        ui->humanGLWidget->setLeftArm2Angle(xA,yA,zA);
    }
    //right arm
    else if(obj == ui->rotateParaRightArm1)
    {
        ui->humanGLWidget->setRightArm1Angle(xA,yA,zA);
    }
    else if(obj == ui->rotateParaRightArm2)
    {
        ui->humanGLWidget->setRightArm2Angle(xA,yA,zA);
    }
    //left leg
    else if(obj == ui->rotateParaLeftThigh)
    {
        ui->humanGLWidget->setLeftThighAngle(xA,yA,zA);
    }
    else if(obj == ui->rotateParaLeftLeg)
    {
        ui->humanGLWidget->setLeftLegAngle(xA,yA,zA);
    }
    //right leg
    else if(obj == ui->rotateParaRightThigh){
        ui->humanGLWidget->setRightThighAngle(xA,yA,zA);
    }
    else if(obj == ui->rotateParaRightLeg){
        ui->humanGLWidget->setRightLegAngle(xA,yA,zA);
    }
    //whole
    else if(obj == ui->rotateParaWhole){
        ui->humanGLWidget->setWholeAngle(xA,yA,zA);
    }
}

void MainWindow::slot_handleResult(QVariant data)
{
    //TODO: 建议修改为 计算前后差值，再用差值相对旋转

    T_SensorData sensorData = data.value<T_SensorData>();
    qDebug()<<sensorData.partName << sensorData.xAngle << sensorData.yAngle  << sensorData.zAngle;
    /* WTarm1up是左上臂, WTarm2up是右上臂， WTleg1是左大腿, WTleg2是右大腿, WTarm2是腰 */
    if(sensorData.partName.contains("WTarm1up"))
    {
//        ui->humanGLWidget->setLeftArmAngle(sensorData.xAngle,sensorData.yAngle,sensorData.zAngle);
        ui->humanGLWidget->setLeftArm1Angle(0, -sensorData.yAngle + 75,0);
    }
    else if(sensorData.partName.contains("WTarm2up"))
    {
//        ui->humanGLWidget->setRightArmAngle(sensorData.xAngle,sensorData.yAngle,sensorData.zAngle);
        ui->humanGLWidget->setRightArm1Angle(0, sensorData.yAngle - 75,0);
    }
    else if(sensorData.partName.contains("WTleg1"))
    {
//        ui->humanGLWidget->setLeftLegAngle(sensorData.xAngle,sensorData.yAngle,sensorData.zAngle);
        ui->humanGLWidget->setLeftThighAngle(-sensorData.yAngle, 0 ,0);
    }
    else if(sensorData.partName.contains("WTleg2"))
    {
//        ui->humanGLWidget->setRightLegAngle(sensorData.xAngle,sensorData.yAngle,sensorData.zAngle);
        ui->humanGLWidget->setRightThighAngle(sensorData.yAngle, 0 ,0);
    }
}


void MainWindow::on_tbnSelectFile_clicked()
{
    QString filepath = QFileDialog::getOpenFileName(this, QString("选择文件"), QString("./"),QString("TEXT(*.txt)"));
    ui->lineEditPath->setText(filepath);
}

void MainWindow::on_btnLoadRun_clicked()
{
    QString filepath = ui->lineEditPath->text();
    if(!filepath.isEmpty()){
        workerThread.start();
        emit open(filepath);
    }
}

void MainWindow::on_btnStop_clicked()
{
   ui->humanGLWidget->setLeftArm1Angle(0,75,0);
   ui->humanGLWidget->setRightArm1Angle(0,-75,0);
   ui->humanGLWidget->setLeftLegAngle(0,0,0);
   ui->humanGLWidget->setRightLegAngle(0,0,0);
   emit stopped();
}
