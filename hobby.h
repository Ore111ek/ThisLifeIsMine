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

/*!
    \file
    \brief Заголовочный файл с описанием классов для интерфейсной части трекера привычек

    Файл содержит описание классов THobbyStatistician, THobbySettings и Hobby
*/
/*!
    \brief Класс для сбора данных из журнала

    Объект класса представляет собой Журнал, в котором хранятся все данные о привычках и
    есть возможность запроса данных о них
*/
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
/*!
    \brief Класс настроек для трекера привычек

    Объект класса представляет собой контейнер для хранения текущих привычек трекера
*/
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
/*!
    \brief Трекер привычек

    Объект класса представляет собой трекер привычек, включающий в себя обработчики событий,
    журнал для записей и сбора данных, файл с индексами и контейнер с текущими настройками
*/
class Hobby : public QMainWindow
{
    Q_OBJECT

public:
    explicit Hobby(QWidget *parent = nullptr, QString name = "Corporate");
    ~Hobby();
private:
    QString username;           ///< Имя текущего пользователя
    TDate date;                 ///< Дата
    THobbyStatistician records; ///< Журнал с записями
    THobbySettings settings;    ///< Настройки
    TGraph *graph;              ///< График для вывода данных из журнала
    DateWindow *Cal;            ///< Окно календаря для выбора даты
    bool rewritingFlag;         ///< Флаг режима перезаписи
    bool changeFlag = false;    ///< Флаг изменения открытой записи
    bool updateFlag = false;    ///< Флаг обновления формы без изменений графиков
    Ui::Hobby *ui;              ///< Указатель на графический интерфейс

private slots:

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

    void on_prevMonthButton_clicked();
    void on_nextMonthButton_clicked();
};

#endif // HOBBY_H
