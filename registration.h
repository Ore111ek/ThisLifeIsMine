#ifndef REGISTRATION_H
#define REGISTRATION_H

#include <QMainWindow>
#include <QFile>
#include <QTextStream>
#include "sha256.h"
/*!
    \file
    \brief Заголовочный файл с описанием классов для интерфейсной части регистрации

    Файл содержит описание класса Registration
*/
namespace Ui {
class Registration;
}
/*!
    \brief Регистрация

    Объект класса представляет собой окно для регистрации новых пользователей
*/
class Registration : public QMainWindow
{
    Q_OBJECT

public:
    explicit Registration(QWidget *parent = nullptr);
    ~Registration();

private slots:
    void on_registerButton_clicked();

private:
    Ui::Registration *ui;
};

#endif // REGISTRATION_H
