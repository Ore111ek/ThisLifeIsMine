#ifndef HOBBY_H
#define HOBBY_H

#include <QMainWindow>
#include <QTableWidget>
#include <QFile>
#include <QTextStream>
#include <vector>
#include "datewindow.h"
#include "journal.h"
#include "graph.h"


class THobbyStatistician : public TJournal{
public:
    std::vector <TStats> wholeMonthStats(QDate date);
    TStats wholeMonthStats(QDate date, QString name);
    std::vector <TStats> wholeYearStats(QDate date);
    TStats wholeYearStats(QDate date, QString name);
    std::vector <TStats> wholeYearSums(QDate date);
    std::vector <TStats> wholeMonthSums(QDate date);
    TStats wholeMonthSums(QDate date, QString name);
};

class THobbySettings{
public:
    void initFile(QString name);
    QStringList habbits(QString username);
    void saveHabbits(QString username, QStringList habbits);
    QFile file;
};

namespace Ui {
class Hobby;
}

class Hobby : public QMainWindow
{
    Q_OBJECT

public:
    explicit Hobby(QWidget *parent = nullptr, QString name = "Corporate");
    //THabbits habbits;
    QString username;
    TDate date;                 ///< Дата
    THobbyStatistician records;           ///< Журнал с записями
    THobbySettings settings;    ///< Настройки интерфейса
    TGraph *graph;
    DateWindow *Cal;            ///< Окно календаря для выбора даты
    bool rewritingFlag;         ///< Флаг режима перезаписи
    bool changeFlag = false;    ///< Флаг изменения открытой записи
    bool updateFlag = false;    ///< Флаг обновления формы без изменений графиков
    ~Hobby();

private slots:

    //void on_tableWidget_cellClicked(int row, int column);

    void on_createButton_clicked();

    void on_findButton_clicked();

    void save_habbits();

    void on_tableWidget_cellChanged(int row, int column);

    void on_deleteButton_clicked();

    void on_calendButton_clicked();

    void drawGraph();

    void clearGraph();

    void on_graphStyleCombo_currentIndexChanged(int index);

    void on_radioSumMonth_clicked();

    void on_radioProgressMonth_clicked();

    void on_radioProgressYear_clicked();

    void on_radioSumYear_clicked();

    void updateComboBox(QDate sel);

    void on_habbitComboBox_currentIndexChanged(const QString &arg1);

    void on_themeBox_stateChanged(int arg1);

    void on_tabWidget_tabBarClicked(int index);

private:
    Ui::Hobby *ui;
};

#endif // HOBBY_H
