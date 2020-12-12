#include "settings.h"
#include "ui_settings.h"
#include "sha256.h"

Settings::Settings(QWidget *parent, QString name) :
    QMainWindow(parent),
    ui(new Ui::Settings)
{
    username = name;
    ui->setupUi(this);
}

Settings::~Settings()
{
    delete ui;
}

void Settings::on_changeButton_clicked()
{
    if(username != "Corporate" && ui->password->text() == ui->password_2->text()){
        bool correctPass = false;
        QString hash, salt;
        QString pass = ui->oldpassword->text();

        QFile file("users.txt");      //Создаём новый файл, в который перепишем обновлённый журнал
        QTextStream out(&file);             //Подключаем потоки к файлам

        file.open(QIODevice::ReadOnly | QIODevice::Text);       //Открываем файлы
        QString line;
        bool newUser = true;
        while(!out.atEnd()){
            line = out.readLine();
            if(line == "}}}}" + username){
                line = out.readLine();
                QString text = line;
                if(!out.atEnd())
                    line = out.readLine();
                while(!line.startsWith("}}}}") && !out.atEnd()){
                    text += "\n" + line;
                    line = out.readLine();
                }

                QStringList list = text.split("}}}");
                hash = list[0];
                salt = list[1];
                newUser = false;
    //            hash = encodeDecode(hash.toLatin1().data(),hash.toLatin1().length(),
    //                                             pass.toLatin1().data(),pass.toLatin1().length());
    //            salt = encodeDecode(salt.toLatin1().data(),salt.toLatin1().length(),
    //                                             pass.toLatin1().data(),pass.toLatin1().length());
    //            if(hash == sha256(pass + salt).simplified()){
                if(hash == sha256(ui->oldpassword->text()+salt).simplified()){
                    correctPass = true;
                }
                else{
                    ui->errorlabel->setText("Неправильный пароль");
                }

            }
        }
        file.close();
        if(correctPass){
            QFile newFile("newUsers.txt");      //Создаём новый файл, в который перепишем обновлённый журнал
            QTextStream in(&newFile);
            file.open(QIODevice::ReadOnly | QIODevice::Text);       //Открываем файлы
            newFile.open(QIODevice::WriteOnly | QIODevice::Text);
            while(!out.atEnd()){    //Пока исходный файл ещё не закончен
                line = out.readLine();
                    if(line == "}}}}" + username){
                        salt = QString::number(qrand() % ((99999999 + 1) - 999) + 999);
                        hash = sha256(ui->password->text()+salt).simplified();
                        line = "}}}}" + username + "\n" + hash + "}}}";
                        line += salt;
                        out.readLine();
                    }
                in << line << endl;
            }
            file.close();                       //Закрытие файлов
            newFile.close();
            QString oldname = file.fileName();  //Запоминаем название старого файла
            file.remove();                      //Удаляем старый файл
            newFile.rename(oldname);            //Переименовываем новый файл

            this->close();
        }
        ui->errorlabel->setText("Введён неправильный текущий пароль");
    }
    else if(username == "Corporate"){
        ui->errorlabel->setText("Нельзя изменить пароль общей записи");
    }
    else if(ui->password->text() != ui->password_2->text()){
        ui->errorlabel->setText("Пароли не совпадают");
    }
}
