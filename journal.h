#ifndef JOURNAL_H
#define JOURNAL_H

#include <QFile>
#include <QTextStream>

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


#endif // JOURNAL_H
