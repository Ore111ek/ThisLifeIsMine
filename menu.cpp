#include "menu.h"
#include "ui_menu.h"
//Конструктор окна Меню
Menu::Menu(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Menu)
{
    ui->setupUi(this);              //Инициализация графического интерфейса Меню
    //delete win_a;                   //Освободить уже выделенную для win_a память
    //win_a = new Authorization(this);//Выделить память для окна авторизации
    //win_a->show();                  //Отобразить окно авторизации
    username = new QString("Corporate");
    delete win_a;                   //Удалить три строки кода
    win_a = new Authorization(this, username);
    win_a->show();
}
//Деструктор окна Меню
Menu::~Menu()
{
    delete win_a;   //Освобождение памяти,
    delete win_c;   //выделенной для окон,
    delete win_d;   //вызванных из Меню
    //delete win_t;
    delete win_n;
    delete win_h;
    //delete win_b;
    delete win_p;
    delete win_s;
    delete ui;      //Освобождение пямяти для интерфейса Меню
}
//Освобождение и выделение памяти для календаря, отображение окна созданного календаря
void Menu::on_showCalendar_triggered()
{
    delete win_c;                 //Удаление уже существующего календаря
    win_c = new Calendar(this, *username);   //Выделение памяти для нового календаря
    win_c->show();                //Отобразить окно нового календаря
}
//Освобождение и выделение памяти для дневника, отображение окна созданного дневника
void Menu::on_showDiary_triggered()
{
    delete win_d;                 //Удаление уже существующего дневника
    win_d = new Dairy(this, *username);      //Выделение памяти для нового дневника
    win_d->show();                //Отобразить окно нового дневника
}
//Освобождение и выделение памяти для расписания, отображение окна созданного расписания
//void Menu::on_showTimetable_triggered()
//{
//    delete win_t;                 //Удаление уже существующего расписания
//    win_t = new Timetable(this);  //Выделение памяти для нового расписания
//    win_t->show();                //Отобразить окно нового расписания
//}
//Освобождение и выделение памяти для заметок, отображение созданного окна заметок
void Menu::on_showNotes_triggered()
{
    delete win_n;                 //Удаление уже существующего окна заметок
    win_n = new Notes(this, *username);      //Выделение памяти для нового окна заметок
    win_n->show();                //Отобразить новое окно заметок
}
//Освобождение и выделение памяти для хобби, отображение окна созданного хобби
void Menu::on_showHobby_triggered()
{
    delete win_h;                 //Удаление уже существующего хобби
    win_h = new Hobby(this, *username);      //Выделение памяти для нового хобби
    win_h->show();                //Отобразить окно нового хобби
}
//Освобождение и выделение памяти для бюджета, отображение окна созданного бюджета
//void Menu::on_showBudget_triggered()
//{
//    delete win_b;                 //Удаление уже существующего бюджета
//    win_b = new Budget(this);     //Выделение памяти для нового бюджета
//    win_b->show();                //Отобразить окно нового бюджета
//}
//Освобождение и выделение памяти для планировщика, отображение окна созданного планировщика
void Menu::on_showPlanner_triggered()
{
    delete win_p;                 //Удаление уже существующего планировщика
    win_p = new Planner(this, *username);    //Выделение памяти для нового планировщика
    win_p->show();                //Отобразить окно нового планировщика
}

void Menu::on_calendarButton_clicked()
{
    on_showCalendar_triggered();
}

void Menu::on_diaryButton_clicked()
{
    on_showDiary_triggered();
}

void Menu::on_plannerButton_clicked()
{
    on_showPlanner_triggered();
}

void Menu::on_notesButton_clicked()
{
    on_showNotes_triggered();
}

void Menu::on_hobbyButton_clicked()
{
    on_showHobby_triggered();
}

void Menu::on_settingsButton_clicked()
{
    delete win_s;                   //Удалить три строки кода
    win_s = new Settings(this, *username);
    win_s->show();
}

void Menu::on_close_action_triggered()
{
    this->close();
}

void Menu::on_change_password_triggered()
{
    on_settingsButton_clicked();
}

void Menu::on_change_user_triggered()
{
    delete win_a;                   //Удалить три строки кода
    win_a = new Authorization(this, username);
    win_a->show();
}
