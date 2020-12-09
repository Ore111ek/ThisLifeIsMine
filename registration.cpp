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
            if(line == "}}}}" + ui->login->text())
                unique = false;
        }
        file.close();

        file.open(QIODevice::Append | QIODevice::Text);       //Открываем файлы

        if(unique){

            //line = "}}}}" + ui->login->text() + "\n" + sha256(ui->password->text());
          //Этого не было
            QString pass = ui->password->text();
            QString salt = QString::number(qrand() % ((99999999 + 1) - 999) + 999);
            QString hash = sha256(ui->password->text()+salt).simplified();
            /*
            line = "}}}}" + ui->login->text() + "\n" + encodeDecode(hash.toUtf8().data(),hash.toUtf8().length(),
                                 ui->password->text().toUtf8().data(),ui->password->text().toUtf8().length()) + "}}}";
            line += encodeDecode(salt.toUtf8().data(),salt.toUtf8().length(),
                                 ui->password->text().toUtf8().data(),ui->password->text().toUtf8().length());
                                 */
            //line = "}}}}" + ui->login->text() + "\n" + encode(hash, ui->password->text()) + "}}}";
            //line += encode(salt, ui->password->text());
            line = "}}}}" + ui->login->text() + "\n" + hash + "}}}";
            line += salt;
          //Вот до сюда
            in << line << endl;
            ui->errorlabel->setText("Пользователь зарегистрирован");
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
