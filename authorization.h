#ifndef AUTHORIZATION_H
#define AUTHORIZATION_H

#include <QMainWindow>
#include "registration.h"
#include "sha256.h"
/*!
    \file
    \brief Заголовочный файл с описанием классов для интерфейсной части авторизации

    Файл содержит описание класса Authorization
*/
namespace Ui {
class Authorization;
}
/*!
    \brief Авторизация

    Объект класса представляет собой окно для авторизации
*/
class Authorization : public QMainWindow
{
    Q_OBJECT

public:
    explicit Authorization(QWidget *parent = nullptr, QString *username = nullptr);
    ~Authorization();

private slots:
    void on_registerButton_clicked();

    void on_loginButton_clicked();

    void on_corporateButton_clicked();

    void decodeArchive();

private:
    QString *login;
    Ui::Authorization *ui;
    Registration *win_reg = nullptr;
    QString hash = "Corporate";
    bool needDecode = false;
};

#endif // AUTHORIZATION_H
