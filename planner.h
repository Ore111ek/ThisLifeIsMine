#ifndef PLANNER_H
#define PLANNER_H

#include <QMainWindow>
#include <QtWidgets>
#include "journal.h"
#include "datewindow.h"
#include <map>

class TList{
public:
    QString listText(QListWidget *list);
    void strikeOutItem(QListWidgetItem *item);
    void setEditableItem(QListWidgetItem *item);
    void addNew(QListWidget *updlist, QListWidgetItem *item);
    void delEmpty(QListWidget *updlist, QListWidgetItem *item);
};

class TTable{
public:
    QString tableText(QTableWidget *list);
    void strikeOutItem(QTableWidgetItem *item);
    void addNew(QTableWidget *updlist, QTableWidgetItem *item);
    void delEmpty(QTableWidget *updlist, QTableWidgetItem *item);
};
/*!
    \brief Класс настроек для ежедневника

    Объект класса представляет собой контейнер для хранения текущих интерфейсных настроек ежедневника
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

class Planner : public QMainWindow
{
    Q_OBJECT

public:
    explicit Planner(QWidget *parent = nullptr, QString name = "Corporate");
    QString username;           ///< Логин пользователя
    TDate date[2];                 ///< Дата
    TList list;
    TTable table;
    TJournal records[3];           ///< Журнал с записями
    TPlannerSettings settings;    ///< Настройки интерфейса дневника
    int categoryIndex;
    bool rewritingFlagW = false; ///< Флаг режима перезаписи
    bool rewritingFlagD = false; ///< Флаг режима перезаписи
    bool changeFlagW = false;
    bool changeFlagD = false;
    bool searchFlag = false;
    DateWindow *Cal[2];            ///< Окно календаря для выбора даты
    QListWidget *week[8] = {};
    QLabel *labels[7] = {};
    ~Planner();

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

private:
    Ui::Planner *ui;
};

#endif // PLANNER_H
