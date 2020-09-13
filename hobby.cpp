#include "hobby.h"
#include "ui_hobby.h"

Hobby::Hobby(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Hobby)
{
    ui->setupUi(this);
}

Hobby::~Hobby()
{
    delete ui;
}
