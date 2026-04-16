#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <fsmpLed.h>
#include<fsmpBeeper.h>
#include<fsmpFan.h>
#include<fsmpEvents.h>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_checkBox_stateChanged(int arg1);

    void on_checkBox_2_stateChanged(int arg1);

    void on_horizontalSlider_valueChanged(int value);

    void on_checkBox_3_stateChanged(int arg1);

    void pushbutton(int num);//自创槽函数

private:
    Ui::Widget *ui;

    fsmpLeds myled;
    fsmpBeeper mybeeper;
    fsmpFan myfan;
    fsmpEvents myevent;

};
#endif // WIDGET_H
