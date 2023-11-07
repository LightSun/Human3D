#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "glwindow.h"
#include "glwindow2.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT
public:
    using GLW = GLWindow2;

    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void slot_choose_model();

private:
    Ui::Widget *ui;
    GLW *m_glWin = nullptr;
};
#endif // WIDGET_H
