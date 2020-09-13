#include "planner.h"
#include "ui_planner.h"

Planner::Planner(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Planner)
{
    ui->setupUi(this);
}

Planner::~Planner()
{
    delete ui;
}
