#include "dairy.h"
#include "ui_dairy.h"
#include "QDate"

//Возвращает текущую дату в формате "9 Декабря 2020 Ср"
QString DateString(){
    QString month = QDate::currentDate().toString("MMMM");  //month содержит текущий месяц
    QString s = QDate::currentDate().toString("d MMMM");    //s содержит число и месяц
    s.truncate(s.length()-1);                               //Удаляем из s последний символ месяца
    if (month == "Март" || month == "Август") s = s + "а";  //В зависимости от текущего месяца добавляем к s
    else s = s + "я";                                       //символ 'а' или 'я' для родительного падежа
    return s + QDate::currentDate().toString(" yyyy ddd");  //Прибавляем к s год и день недели
}

Dairy::Dairy(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Dairy)
{
    ui->setupUi(this);                  //Инициализация графического интерфейса
    ui->label->setText(DateString());   //Запись в label текущей даты в формате "9 Декабря 2020 Ср"
}

Dairy::~Dairy()
{
    delete ui;
}
