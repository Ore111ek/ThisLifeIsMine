#ifndef MENU_H
#define MENU_H

#include "authorization.h"
#include "calendar.h"
#include "notes.h"
#include "dairy.h"
#include "planner.h"
#include "hobby.h"
#include "settings.h"
#include <QMainWindow>
/*!
    \file
    \brief Заголовочный файл с описанием классов для интерфейсной части ежедневника

    Файл содержит описание классов TList, TTable, TPlannerSettings и Planner
*/

namespace Ui {
class Menu;
}
/*!
    \brief Меню

    Объект класса представляет собой диспетчер для вызова приложений и их настройки
*/
class Menu : public QMainWindow
{
    Q_OBJECT

public:
    explicit Menu(QWidget *parent = nullptr);
    ~Menu();



private slots:

    void on_showCalendar_triggered();

    void on_showDiary_triggered();

    //void on_showTimetable_triggered();

    void on_showNotes_triggered();

    void on_showHobby_triggered();

    //void on_showBudget_triggered();

    void on_showPlanner_triggered();

    void on_calendarButton_clicked();

    void on_diaryButton_clicked();

    void on_plannerButton_clicked();

    void on_notesButton_clicked();

    void on_hobbyButton_clicked();

    void on_settingsButton_clicked();

    void on_close_action_triggered();

    void on_change_password_triggered();

    void on_change_user_triggered();

private:
    Ui::Menu *ui;                   ///< Указатель на графический интерфейс
    QString *username;              ///< Логин пользователя
    Authorization *win_a = nullptr; ///< Указатели виджетов приложений
    Calendar *win_c = nullptr;
    Notes *win_n = nullptr;
    Dairy *win_d = nullptr;
    Planner *win_p = nullptr;
    //Timetable *win_t = nullptr;
    Hobby *win_h = nullptr;
    //Budget *win_b = nullptr;
    Settings *win_s = nullptr;

};

#endif // MENU_H
