#include "registration.h"
#include "ui_registration.h"

Registration::Registration(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Registration)
{
    ui->setupUi(this);
}

Registration::~Registration()
{
    delete ui;
}

void Registration::on_registerButton_clicked()
{
    if(ui->password->text() == ui->password_2->text()){
        bool unique = true;
        QString line;
        QFile file("users.txt");      //Создаём новый файл, в который перепишем обновлённый журнал
        QTextStream in(&file);             //Подключаем потоки к файлам

        file.open(QIODevice::ReadOnly | QIODevice::Text);       //Открываем файлы

        while(!in.atEnd()){
            line = in.readLine();
            if(line == ui->login->text())
                unique = false;
        }
        file.close();

        file.open(QIODevice::Append | QIODevice::Text);       //Открываем файлы

        if(unique){

            line = ui->login->text() + "\n" + sha256(ui->password->text());
//            QString salt = QString::number(qrand() % ((99999999 + 1) - 999) + 999);
//            QString hash = sha256(ui->password->text()+salt).simplified();
//            line = ui->login->text() + "\n" + encodeDecode(hash.toLatin1().data(),hash.toLatin1().length(),
//                                 ui->password->text().toLatin1().data(),ui->password->text().toLatin1().length()) + "\n";
//            line += encodeDecode(salt.toLatin1().data(),salt.toLatin1().length(),
//                                 ui->password->text().toLatin1().data(),ui->password->text().toLatin1().length());
            in << line << endl;
        }
        else{
            ui->errorlabel->setText("Логин уже используется");
        }
        file.close();
    }else{
        ui->password_2->clear();
        ui->errorlabel->setText("Введены различные пароли");
    }
}
