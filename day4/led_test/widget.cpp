#include "widget.h"
#include "ui_widget.h"

#include <qdebug.h>
#include<fsmpBeeper.h>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    ui->horizontalSlider->setRange(0,100);

    connect(&myevent,&fsmpEvents::keyPressed,this,&Widget:: pushbutton);
}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_pushButton_clicked()
{
    if(ui->pushButton->text()=="开灯"){

        myled.on(fsmpLeds::LED1);
        ui->pushButton->setText("关灯");
    }
    else{
        myled.off(fsmpLeds::LED1);
        ui->pushButton->setText("开灯");
    }
}


void Widget::on_pushButton_2_clicked()
{

}


void Widget::on_checkBox_stateChanged(int arg1)
{
    if(arg1){

        myled.on(fsmpLeds::LED2);    }
    else
    {

        myled.off(fsmpLeds::LED2);
    }
}



void Widget::on_checkBox_2_stateChanged(int arg1)
{
    if(arg1){

        mybeeper.setRate(5000);
        mybeeper.start();
    }
    else
    {
        mybeeper.stop();
    }
}


void Widget::on_horizontalSlider_valueChanged(int value)
{
    qDebug() << ui->horizontalSlider->value();

    mybeeper.setRate(value);
    mybeeper.start();

    ui->progressBar->setValue(ui->horizontalSlider->value());
}


void Widget::on_checkBox_3_stateChanged(int arg1)
{
    if(arg1){

            myfan.setSpeed(100);
            myfan.start();
        }
        else
        {
            myfan.stop();
        }
}
void Widget:: pushbutton(int num){

  switch(num){

  case 1:
      myled.on(fsmpLeds::LED1);
      break;
  case 2:
      myled.on(fsmpLeds::LED2);
      break;
  case 3:
      myled.on(fsmpLeds::LED3);
      break;
  default:
      break;
  }

};

