#ifndef JOURNAL_H
#define JOURNAL_H

#include <QFile>
#include <QTextStream>

/*!
    \file
    \brief Заголовочный файл с описанием классов для работы с файлами

    Файл содержит описание классов TJournal, TRecord, TIndexes и TDate
*/
/*!
    \brief Класс менеджера для работы с файлами индексов

    Объект класса представляет собой посредника между журналом и записями в нём.
    Индексы необходимы для более структурированного хранения записей и сокращения времени на их поиск.
    Файл с индексами имеет следующий обобщённый вид:
    "месяц 1" "страница в журнале"
    ...
    "месяц n" "страница в журнале"
*/
class TIndexes{
public:
    void saveNew(QString date, qint64 pos);   ///< Создание нового индекса, если такого нет в текущем файле индексов
    void updateIndexes(QString date, qint64 position, qint64 length, bool flag); ///<Обновление индексов после добавления новой записи в журнал.
    qint64 checkIndex(QString date);            ///< Проверка существования индекса на определённый месяц и возврат его позиции
    void initFile(QString filename);        ///< Установка связи с файлом для хранения в нём индексов
    QFile file;                             ///< Файл с индексами
};
/*!
    \brief Класс даты

    Объект класса хранит текущую дату currentDate и выбранную пользователем selectedDate.
 При выводе сообщений может использоваться преобразование dateString для вывода даты в формате "9 Декабря 2020 Ср"
*/
class TDate{
public:
    TDate();
    QString currentDate;    ///< Дата запуска приложения
    QString selectedDate;   ///< Выбранная пользователем дата
    QString dateString(QDate selDate);   ///< Преобразование из QDate в строковый формат "9 Декабря 2020 Ср"
    QDate nearMonday(QDate selDate);
    QString weekString(QDate Monday);
};
/*!
    \brief Класс записи в журнале

    Объект класса представляет собой запись, формат которой используется при записи в журнал
*/
class TRecord{
public:
    TRecord(){
        date = "";
        text = date = changeDate;
    }                               ///< Конструктор по умолчанию для создания пустой записи
    TRecord(QString ntext, QString ndate, QString nchangeDate);///< Конструктор для создания записи
    QString text;           ///< Текст записи
    QString date;           ///< Дата события, описываемого в записи
    QString changeDate;     ///< Дата последнего изменения текста записи
};
/*!
    \brief Класс менеджера для работы с записями в файле

    Объект класса представляет собой менеджер для добавления, изменения и поиска записей в журнале.
    Файл с записями имеет следующий обобщённый вид:
    &"Дата 1" "Текст записи" "Дата изменения"
    ...
    &"Дата n" "Текст записи" "Дата изменения"
*/
class TJournal{
public:
    void saveNew(TRecord rec);      ///< Сохраняет в журнал новую запись указанной даты, соблюдая сортировку по дате события
    void rewrite(TRecord rec);      ///< Сохраняет изменения в записи указанной даты
    qint64 findRecord(QString date);///< Находит позицию записи в журнале по переданной дате события
    void initFile(QString filename);///< Установка связи с файлом для хранения в нём записей
    QString readText(qint64 pos);   ///< Чтение текста в позиции pos
    TIndexes bookmarks;             ///< Закладки для быстрого поиска месяца в журнале
private:
    QFile file;                     ///< Файл с записями из дневника
    TRecord oldRecord;              ///< Созданная ранее запись
};

#endif // JOURNAL_H
