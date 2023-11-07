#include "widget.h"
#include "ui_widget.h"
#include <QMessageBox>
#include <QFileDialog>
#include "qobjload.h"


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    connect(ui->lineEdit_file, SIGNAL(returnPressed()), this, SLOT(slot_choose_model()));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::slot_choose_model()
{
    QString file_name = QFileDialog::getOpenFileName(nullptr, "open", ".", "*.obj");
    ui->lineEdit_file->setText(file_name);

    if (file_name.isEmpty())
    {
        ui->label_log->setText("File is not existed, use default.");
        QString dir = "/home/heaven7/heaven7/study/github/mine_clone/Human3D/bin/STLFile/human";
        file_name = dir + "/A右大腿.stl";
    }

    D3ModelLoader objLoader_;
    QVector<float> vPoints_;
    if (!objLoader_.loadStl(file_name, vPoints_))
    {
        ui->label_log->setText("Load model file failed!");
        return;
    }
    else
        ui->label_log->setText("Load model file successed!");

    if (vPoints_.size() < 3)
    {
        ui->label_log->setText("Model file is not correct!");
        return;
    }

    m_glWin = new GLW(vPoints_);
    m_glWin->resize(800, 600);
    ui->verticalLayout_model->addWidget(m_glWin);
    connect(ui->horizontalSlider_X, SIGNAL(valueChanged(int)), m_glWin, SLOT(slot_cam_X(int)));
    connect(ui->horizontalSlider_Y, SIGNAL(valueChanged(int)), m_glWin, SLOT(slot_cam_Y(int)));
    connect(ui->horizontalSlider_Z, SIGNAL(valueChanged(int)), m_glWin, SLOT(slot_cam_Z(int)));
}
