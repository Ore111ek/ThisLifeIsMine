#ifndef MENU_H
#define MENU_H

#include "authorization.h"
#include "calendar.h"
#include "notes.h"
#include "dairy.h"
#include "planner.h"
#include "timetable.h"
#include "hobby.h"
#include "budget.h"
#include <QMainWindow>

namespace Ui {
class Menu;
}

class Menu : public QMainWindow
{
    Q_OBJECT

public:
    explicit Menu(QWidget *parent = nullptr);
    ~Menu();

private slots:

    void on_action_triggered();

    void on_action_2_triggered();

    void on_action_3_triggered();

    void on_action_4_triggered();

    void on_action_5_triggered();

    void on_action_6_triggered();

    void on_action_7_triggered();

private:
    Ui::Menu *ui;                   //Указатель на графический интерфейс

    Authorization *win_a = nullptr; //Указатели виджетов приложений
    Calendar *win_c = nullptr;
    Notes *win_n = nullptr;
    Dairy *win_d = nullptr;
    Planner *win_p = nullptr;
    Timetable *win_t = nullptr;
    Hobby *win_h = nullptr;
    Budget *win_b = nullptr;
};

#endif // MENU_H
