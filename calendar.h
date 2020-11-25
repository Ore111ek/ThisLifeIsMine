#ifndef CALENDAR_H
#define CALENDAR_H

#include <QMainWindow>
#include "QtWidgets"
#include "journal.h"

class FemaleCal : public QMainWindow
{

public:
    explicit FemaleCal(QTableWidget *tab = nullptr, QDate month = QDate::currentDate(), QString name = "Corporate");
    void saveFemaleSettings();
    void readFemaleSettings();
    void markDays(QDate curDate);
    ~FemaleCal();
    QFile settingsFile;
    QString username;
    QMap <QDate, int> settings;
    QTableWidget *table;
    QDate shownMonth;
    QDate lastDate;
    int duration;
    int cycle;
    bool stage;
    QColor backColor = QColor(240,13,14,50);
    QColor numColor = QColor(255,255,255,255);
};


namespace Ui {
class Calendar;
}

class Calendar : public QMainWindow
{
    Q_OBJECT

public:
    explicit Calendar(QWidget *parent = nullptr, QString name = "Corporate");
    ~Calendar();
    QString username;
    TDate date;                 ///< Дата
    TJournal records;           ///< Журнал с записями
    bool rewritingFlag = false;
    bool updateFlag = false;
    bool updateTFlag = false;
    //bool newPageFlag = false;
    QString iconsTable[6][7];
    int curRow;
    int curCol;
    FemaleCal *fwidget = nullptr;
    //QWidget *colorSettings;
private slots:

    void saveEvent();               ///< Сохранить запись в журнал

    void findEvent();               ///< Открыть в дневнике выбранную дату

    void setIcon(int row, int col, QString path);

    int tableFromDate(QDate sel);

    QDate dateFromTable(int row, int column);

    void on_tableWidget_cellClicked(int row, int column);

    void on_icon_gift_triggered();

    void on_icon_party_triggered();

    void on_icon_credit_triggered();

    void on_icon_trip_triggered();

    void on_icon_sport_triggered();

    void on_icon_heart_triggered();

    void on_icon_medicine_triggered();

    void on_icon_important_triggered();

    void on_calendarWidget_currentPageChanged(int year, int month);

    void on_tableWidget_itemChanged(QTableWidgetItem *item);

    void on_saveFemaleSettings_clicked();

    void on_showFemaleSettings_triggered();

    void on_showColorSettings_clicked();

    void on_periodSpin_valueChanged(int arg1);

    void on_cycleSpin_valueChanged(int arg1);

    void on_addCycleButton_clicked();

    void on_backColorBtn_clicked();

    void on_numColorBtn_clicked();

    void on_stageButton_clicked();

    void on_datesTable_itemChanged(QTableWidgetItem *item);

private:
    Ui::Calendar *ui;
};

#endif // CALENDAR_H
