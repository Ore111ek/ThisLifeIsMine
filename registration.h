#ifndef REGISTRATION_H
#define REGISTRATION_H

#include <QMainWindow>
#include <QFile>
#include <QTextStream>
#include "sha256.h"

namespace Ui {
class Registration;
}

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
