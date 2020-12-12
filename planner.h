#ifndef PLANNER_H
#define PLANNER_H

#include <QMainWindow>
#include <QtWidgets>
#include "journal.h"
#include "datewindow.h"
#include <map>
/*!
    \file
    \brief Заголовочный файл с описанием классов для интерфейсной части ежедневника

    Файл содержит описание классов TList, TTable, TPlannerSettings и Planner
*/
/*!
    \brief Класс Список

    Объект класса позволяет реализовать зачеркивание, редактирование и удаление элементов списка
*/
class TList{
public:
    QString listText(QListWidget *list);
    void strikeOutItem(QListWidgetItem *item);
    void setEditableItem(QListWidgetItem *item);
    void addNew(QListWidget *updlist, QListWidgetItem *item);
    void delEmpty(QListWidget *updlist, QListWidgetItem *item);
};
/*!
    \brief Класс Таблица

    Объект класса позволяет реализовать зачеркивание, редактирование и удаление элементов таблицы
*/
class TTable{
public:
    QString tableText(QTableWidget *table);
    void strikeOutItem(QTableWidgetItem *item);
    void addNew(QTableWidget *updtable, QTableWidgetItem *item);
    void delEmpty(QTableWidget *updtable, QTableWidgetItem *item);
};
/*!
    \brief Класс настроек для ежедневника

    Объект класса представляет собой контейнер для хранения текущих интерфейсных настроек ежедневника, а именно работает с категориями
*/
class TPlannerSettings{
public:
    void initFile(QString name);
    QStringList Categories(QString username);
    void saveCategories(QString username, QStringList categories);
    QFile file;
};

namespace Ui {
class Planner;
}
/*!
    \brief Ежедневник

    Объект класса представляет собой ежедневник, включающий в себя обработчики событий,
    журнал для записей, файл с индексами и контейнер с текущими настройками интерфейса
*/
class Planner : public QMainWindow
{
    Q_OBJECT

public:
    explicit Planner(QWidget *parent = nullptr, QString name = "Corporate");
    ~Planner();
private:
    QString username;           ///< Логин пользователя
    TDate date[2];              ///< Дата
    TList list;                 ///< Список дел на день
    TTable table;               ///< Таблица для расписания на день
    TJournal records[3];        ///< Журналы с записями на год, неделю, день
    TPlannerSettings settings;  ///< Настройки интерфейса дневника
    int categoryIndex;          ///< Индекс текущей категории
    bool rewritingFlagW = false;///< Флаг режима перезаписи
    bool rewritingFlagD = false;///< Флаг режима перезаписи
    bool changeFlagW = false;   ///< Флаг режима изменений интерфейса для недели
    bool changeFlagD = false;   ///< Флаг режима изменений интерфейса для дня
    bool searchFlag = false;    ///< Флаг режима поиска
    DateWindow *Cal[2];         ///< Окно календаря для выбора даты
    QListWidget *week[8] = {};  ///< Списки дел на каждый день недели и лист заметок
    QLabel *labels[7] = {};     ///< Заголовки дней недели
    Ui::Planner *ui;

private slots:

    void on_weekSaveButton_clicked();

    void on_weekList_itemDoubleClicked(QListWidgetItem *item);

    void on_weekList_itemClicked(QListWidgetItem *item);

    void on_weekList_itemChanged(QListWidgetItem *item);

    void on_weekFindButton_clicked();

    void on_prevWeekButton_clicked();

    void on_nextWeekButton_clicked();

    void on_calendarButton_clicked();

    void on_delCategoryTab_triggered();

    void on_newCategoryTab_triggered();

    void on_week_Categories_tabBarDoubleClicked();

    void on_acceptButton_clicked();

    void on_renameCategory_triggered();

    //void on_week_Categories_currentChanged(int index);

    void on_week_Categories_tabBarClicked(int index);

    void on_daySaveButton_clicked();

    void on_todolist_itemClicked(QListWidgetItem *item);

    void on_todolist_itemDoubleClicked(QListWidgetItem *item);

    void on_todolist_itemChanged(QListWidgetItem *item);

    void on_tableWidget_itemClicked(QTableWidgetItem *item);

    void on_tableWidget_itemChanged(QTableWidgetItem *item);

    void on_tableWidget_itemDoubleClicked(QTableWidgetItem *item);

    void on_dayFindButton_clicked();

    void on_calendButton_clicked();

    void on_prevDayButton_clicked();

    void on_nextDayButton_clicked();

    void on_notes_textChanged();

};

#endif // PLANNER_H
