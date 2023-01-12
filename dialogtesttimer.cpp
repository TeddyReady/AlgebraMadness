#include "dialogtesttimer.h"
#include "ui_dialogtesttimer.h"

DialogTestTimer::DialogTestTimer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogTestTimer)
{
    ui->setupUi(this);
    ui->timeEdit->setTime(QTime(0, 0, 0, 0));
}

DialogTestTimer::~DialogTestTimer()
{
    delete ui;
}

void DialogTestTimer::on_buttonBox_accepted()
{
    if (!ui->checkBox->isChecked())
        ui->timeEdit->setTime(QTime(0, 0, 0, 0));
    emit timeRemaining(ui->timeEdit->time());
    close();
}


void DialogTestTimer::on_buttonBox_rejected()
{
    close();
}
