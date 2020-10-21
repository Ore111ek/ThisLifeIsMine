#include "datewindow.h"
#include "ui_datewindow.h"

DateWindow::DateWindow(QWidget *parent, std::unordered_set <QDateEdit *> res) :
    QMainWindow(parent),
    ui(new Ui::DateWindow)
{
    ui->setupUi(this);
    result = res;       //Инициализация полей вывода даты
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
    this->close();                          //Закрыть календарь
}
