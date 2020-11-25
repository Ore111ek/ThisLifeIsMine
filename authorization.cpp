#include "authorization.h"
#include "ui_authorization.h"
#include <QTextCodec>
Authorization::Authorization(QWidget *parent, QString *username) :
    QMainWindow(parent),
    ui(new Ui::Authorization)
{
    login = username;
    ui->setupUi(this);





  //  decodeFile("users.txt","jgrko346ojelnbjthldofi56");
}

Authorization::~Authorization()
{
   // decodeFile("users.txt","jgrko346ojelnbjthldofi56");
   // if(needDecode)
    //    decodeArchive();
    delete win_reg;
    delete ui;
}

void Authorization::on_registerButton_clicked()
{
    delete win_reg;
    win_reg = new Registration(this);
    win_reg->show();
}

void Authorization::on_loginButton_clicked()
{
    *login = ui->login->text();
    QString hash, salt;
    QString pass = ui->password->text();

    QFile file("users.txt");      //Создаём новый файл, в который перепишем обновлённый журнал
    QTextStream in(&file);             //Подключаем потоки к файлам

    file.open(QIODevice::ReadOnly | QIODevice::Text);       //Открываем файлы
    QString line;
    bool newUser = true;
    while(!in.atEnd()){
        line = in.readLine();
        if(line == *login){
            hash = in.readLine();
//            salt = in.readLine();
            newUser = false;
//            hash = encodeDecode(hash.toLatin1().data(),hash.toLatin1().length(),
//                                             pass.toLatin1().data(),pass.toLatin1().length());
//            salt = encodeDecode(salt.toLatin1().data(),salt.toLatin1().length(),
//                                             pass.toLatin1().data(),pass.toLatin1().length());
//            if(hash == sha256(pass + salt).simplified()){
            if(hash == sha256(ui->password->text())){
                //decodeArchive();
                needDecode = true;
                this->close();
            }
            else{
                ui->errorlabel->setText("Неправильный пароль");
                *login = "Corporate";
            }

        }
    }
    if(in.atEnd() && newUser){
        ui->errorlabel->setText("Логин не найден");
    }
    file.close();


}

void Authorization::on_corporateButton_clicked()
{
    this->close();
}

void Authorization::decodeArchive()
{
    decodeFile(*login + "PlannerWRecords.txt",hash);
    decodeFile(*login + "PlWBookMarks.txt",hash);
    decodeFile(*login + "PlannerDRecords.txt",hash);
    decodeFile(*login + "PlDBookMarks.txt",hash);
    decodeFile(*login + "PlannerSettings.txt",hash);
    decodeFile(*login + "Notes.txt",hash);
    decodeFile(*login + "DairyRecords.txt",hash);
    decodeFile(*login + "DRBookMarks.txt",hash);
    decodeFile(*login + "CalendarRecords.txt",hash);
    decodeFile(*login + "CalBookMarks.txt",hash);
    decodeFile(*login + "Habbits.txt",hash);
    decodeFile(*login + "HHBookMarks.txt",hash);
}


