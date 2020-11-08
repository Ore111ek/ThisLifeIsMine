#include "datewindow.h"
#include "ui_datewindow.h"

/*!
    \file
    \brief Файл реализации класса

    Файл содержит реализацию класса DateWindow
*/
/*!
\param[in] parent - указатель на родительское окно (например, дневник)
\param[in] res - контейнер с полями вывода, в которые нужно передать дату
*/
DateWindow::DateWindow(QWidget *parent, std::unordered_set <QDateEdit *> res, QPushButton *but) :
    QMainWindow(parent),
    ui(new Ui::DateWindow)
{
    ui->setupUi(this);
    result = res;       //Инициализация полей вывода даты
    confirmButton = but;//Кнопка обновления в родительском окне
}
//Освобождение памяти интерфейса календаря
DateWindow::~DateWindow()
{
    delete ui;
}
//Подтверждение выбранной даты
void DateWindow::on_pushButton_clicked()
{
    SelectedDate = ui->calendarWidget->selectedDate();  //Запомнить выбранную дату
    for (auto edit: result){                //Для каждого из полей даты(день, месяц, год)
        edit->setDate(SelectedDate);        //Присвоить выбранную пользователем дату
    }
    confirmButton->click();                 //Обновить родительское окно
    this->close();                          //Закрыть календарь
}
