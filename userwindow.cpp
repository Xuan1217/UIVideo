#include "userwindow.h"
#include "ui_userwindow.h"

userwindow::userwindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::userwindow)
{
    ui->setupUi(this);
}

userwindow::~userwindow()
{
    delete ui;
}
