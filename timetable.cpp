#include "timetable.h"
#include "ui_timetable.h"

Timetable::Timetable(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Timetable)
{
    ui->setupUi(this);
}

Timetable::~Timetable()
{
    delete ui;
}
