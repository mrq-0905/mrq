#include "widget.h"
#include "ui_widget.h"
#include "qdebug.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_pushButton_clicked()
{
    if(ui ->pushButton->text() == "开灯"){
        myled.on(fsmpLeds::LED1);
        ui->pushButton->setText(("关灯"));
    }else{
        myled.off(fsmpLeds::LED1);
        ui->pushButton->setText("开灯");
    }
}


void Widget::on_pushButton_2_clicked()
{
    qDebug() << "PG1" << endl;
}


void Widget::on_pushButton_3_clicked()
{
    qDebug() << "庆祝的酒为你开好，千万不要膨胀的太早" <<endl;
}


void Widget::on_pushButton_4_clicked()
{
    qDebug() << "把每首歌都唱好" <<endl;
}


void Widget::on_pushButton_5_clicked()
{
    qDebug() <<"回到哈尔滨去见家乡父老" <<endl;
}


void Widget::on_pushButton_6_clicked()
{
    qDebug() <<"加油，你是最棒的" << endl;
}

