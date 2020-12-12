#ifndef CALENDAR_H
#define CALENDAR_H

#include <QMainWindow>
#include "QtWidgets"
#include "journal.h"
/*!
    \file
    \brief Заголовочный файл с описанием классов для интерфейсной части ежедневника

    Файл содержит описание классов TList, TTable, TPlannerSettings и Planner
*/
/*!
    \brief Женский календарь

    Объект класса представляет собой женский календарь, представленный таблицей для отслеживания циклов
*/
class FemaleCal : public QMainWindow
{

public:
    explicit FemaleCal(QTableWidget *tab = nullptr, QDate month = QDate::currentDate(), QString name = "Corporate");
    void saveFemaleSettings();
    void readFemaleSettings();
    void markDays(QDate curDate);
    ~FemaleCal();
    QMap <QDate, int> settings; ///< Контейнер, который хранит даты начала и длительность в днях
    QTableWidget *table;        ///< Таблица для отображения циклов
    int duration;               ///< Средняя длительность
    int cycle;                  ///< Средний цикл
    bool stage;                 ///< Текущее состояние
    QColor backColor = QColor(240,13,14,50);  ///< Цвет фона выделяемых дней
    QColor numColor = QColor(255,255,255,255);///< Цвет чисел женского календаря
private:
    QFile settingsFile;         ///< Файл для хранения данных и настроек
    QString username;           ///< Логин пользователя
    QDate shownMonth;           ///< Показываемый месяц
    QDate lastDate;             ///< Последняя дата начала

};


namespace Ui {
class Calendar;
}
/*!
    \brief Календарь

    Объект класса представляет собой календарь, включающий в себя обработчики событий,
    журнал для записей, файл с индексами и виджет женского календаря
*/
class Calendar : public QMainWindow
{
    Q_OBJECT

public:
    explicit Calendar(QWidget *parent = nullptr, QString name = "Corporate");
    ~Calendar();
private:
    QString username;           ///< Логин пользователя
    TDate date;                 ///< Дата
    TJournal records;           ///< Журнал с записями
    bool rewritingFlag = false; ///< Флаг перезаписи
    bool updateFlag = false;    ///< Флаг обновления интерфейса календаря
    bool updateTFlag = false;   ///< Флаг обновления интерфейса женского календаря
    //bool newPageFlag = false;
    QString iconsTable[6][7];   ///< Иконки, отображаемые на днях
    int curRow;                 ///< Строка выбранной ячейки
    int curCol;                 ///< Столбец выбранной ячейки
    FemaleCal *fwidget = nullptr;///< Женский календарь
    Ui::Calendar *ui;           ///< Указатель на графический интерфейс
    //QWidget *colorSettings;
private slots:

    void saveEvent();               ///< Сохранить запись в журнал

    void findEvent();               ///< Открыть в дневнике выбранную дату

    void setIcon(int row, int col, QString path);   ///< Установить иконку с путём path для ячейки в строке row и столбце col

    int tableFromDate(QDate sel);                   ///< Получить номер ячейки из выбранной даты

    QDate dateFromTable(int row, int column);       ///< Получить дату из выбранных столбца и строчки

    void on_tableWidget_cellClicked(int row, int column);   ///< Выделить ячейку при нажатии

    void on_icon_gift_triggered();      ///< Установить иконку подарка

    void on_icon_party_triggered();

    void on_icon_credit_triggered();

    void on_icon_trip_triggered();

    void on_icon_sport_triggered();

    void on_icon_heart_triggered();

    void on_icon_medicine_triggered();

    void on_icon_important_triggered();

    void on_calendarWidget_currentPageChanged(int year, int month); ///< Обновить виджеты при переключении месяца

    void on_tableWidget_itemChanged(QTableWidgetItem *item);    ///< Сохранить изменения при изменении ячейки

    void on_saveFemaleSettings_clicked();       ///< Сохранить данные и настройки женского календаря

    void on_showFemaleSettings_triggered();     ///< Показать данные и настройки женского календаря

    void on_showColorSettings_clicked();        ///< Показать/скрыть цветовые настройки

    void on_periodSpin_valueChanged(int arg1);  ///< Изменить среднюю длительность периода

    void on_cycleSpin_valueChanged(int arg1);   ///< Изменить среднюю длительность цикла

    void on_addCycleButton_clicked();           ///< Добавить новый цикл

    void on_backColorBtn_clicked();             ///< Выбрать цвет выделяемых дней

    void on_numColorBtn_clicked();              ///< Выбрать цвет чисел женского календаря

    void on_stageButton_clicked();              ///< Изменить текущее состояние (Начались/закончились)

    void on_datesTable_itemChanged(QTableWidgetItem *item); ///< Сохранить данные при изменении таблицы с циклами

    void on_switchFemaleWidget_triggered();     ///< Показывать/скрыть женский календарь

};

#endif // CALENDAR_H
