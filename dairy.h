#ifndef DAIRY_H
#define DAIRY_H

#include <QMainWindow>
#include <QFile>
#include <QTextStream>
#include "datewindow.h"

namespace Ui {
class Dairy;
}

class TIndexes{
public:
    void saveNew(QString date, qint64 pos);   //Создание индекса
    void updateIndexes(QString date, qint64 position, qint64 length, bool flag); //Обновление индексов
    qint64 checkIndex(QString date);            //Проверка существования индекса
    void initFile(QString filename);        //Связь с файлом индексов
    QFile file;
};

class TDate{
public:
    TDate();
    QString currentDate;    //Дата запуска приложения
    QString selectedDate;   //Выбранная дата
    QString dateString(QDate selDate);   //Формирует строку из даты
};

class TRecord{
public:
    TRecord(){
        date = "";
        text = date = changeDate;
    }
    TRecord(QString ntext, QString ndate, QString nchangeDate);
    QString text;           //Текст записи
    QString date;           //Дата записи
    QString changeDate;     //Дата последнего изменения
};

class TJournal{
public:
    void saveNew(TRecord rec);      //Сохраняет в конце журнала новую запись указанной даты
    void rewrite(TRecord rec);      //Сохраняет изменения в записи указанной даты
    qint64 findRecord(QString date);//Находит позицию записи искомой даты в журнале
    void initFile(QString filename);//Связь журнала с файлом записей
    QString readText(qint64 pos);   //Чтение текста в позиции pos
    TIndexes bookmarks;              //Закладки для быстрого поиска месяца
private:
    QFile file;                     //Файл с записями из дневника
    TRecord oldRecord;              //Созданная ранее запись
    qint64 posOfOld;                //Позиция в файле записи для изменения
};

class Dairy : public QMainWindow
{
    Q_OBJECT

public:
    explicit Dairy(QWidget *parent = nullptr);
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
