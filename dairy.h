#ifndef DAIRY_H
#define DAIRY_H

#include <QMainWindow>
#include <QFile>
#include <QTextStream>
#include "datewindow.h"
#include "journal.h"

namespace Ui {
class Dairy;
}

class Dairy : public QMainWindow
{
    Q_OBJECT

public:
    explicit Dairy(QWidget *parent = nullptr, QString name = "Corporate");
    QString username;           //Логин пользователя
    TDate date;                 //Дата
    TJournal records;           //Записи
    bool rewritingFlag = false; //Флаг режима перезаписи
    DateWindow *Cal;            //Календарь для выбора даты
    ~Dairy();

private slots:
    void on_saveButton_clicked(); //Сохранить запись

    void on_findButton_clicked(); //Открыть в дневнике выбранную дату

    void on_pushButton_clicked(); //Открыть календарь для выбора даты

    void on_fontSelect_currentTextChanged(const QString &arg1); //Изменение шрифта

    void on_fontSize_valueChanged(int arg1);  //Изменение размера шрифта

private:
    Ui::Dairy *ui;
};

#endif // DAIRY_H
