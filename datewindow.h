#ifndef DATEWINDOW_H
#define DATEWINDOW_H

#include <QMainWindow>
#include <QDate>
#include <QDateTimeEdit>
#include <unordered_set>

namespace Ui {
class DateWindow;
}

class DateWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit DateWindow(QWidget *parent = nullptr, std::unordered_set <QDateEdit *> res = {});
    ~DateWindow();
    QDate SelectedDate;         //Поле хранения выбранной даты
    std::unordered_set <QDateEdit *> result;    //Контейнер полей вывода даты

private slots:
    void on_pushButton_clicked();

private:
    Ui::DateWindow *ui;
};

#endif // DATEWINDOW_H
