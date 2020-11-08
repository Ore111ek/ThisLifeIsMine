#ifndef DATEWINDOW_H
#define DATEWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QDate>
#include <QDateTimeEdit>
#include <unordered_set>

/*!
    \file
    \brief Заголовочный файл с описанием класса

    Файл содержит описание класса DateWindow
*/

namespace Ui {
class DateWindow;
}
/*!
    \brief Окно с календарём

    Объект класса представляет окно календаря для выбора пользователем даты не в текстовом формате
*/
class DateWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit DateWindow(QWidget *parent = nullptr, std::unordered_set <QDateEdit *> res = {}, QPushButton *but = nullptr);  ///< Конструктор
    ~DateWindow();                              ///< Деструктор для освобождения памяти
    QDate SelectedDate;                         ///< Поле хранения выбранной даты
    std::unordered_set <QDateEdit *> result;    ///< Контейнер полей вывода даты в родительском окне
    QPushButton * confirmButton;                ///< Кнопка для обновления даты в родительском окне

private slots:
    void on_pushButton_clicked();   ///< Подтверждение выбранной даты

private:
    Ui::DateWindow *ui;             ///< Указатель на интерфейс окна календаря
};

#endif // DATEWINDOW_H
