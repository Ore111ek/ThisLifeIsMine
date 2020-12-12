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
    //if(needDecode)
       // decodeArchive();
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
    QString loginstr = ui->login->text();
    QString checkhash, salt;
    QString pass = ui->password->text();

    QFile file("users.txt");      //Создаём новый файл, в который перепишем обновлённый журнал
    QTextStream in(&file);             //Подключаем потоки к файлам

    file.open(QIODevice::ReadOnly | QIODevice::Text);       //Открываем файлы
    QString line;
    bool newUser = true;
    while(!in.atEnd()){
        line = in.readLine();
        if(line == "}}}}" + loginstr){
            line = in.readLine();
            QString text = line;
            if(!in.atEnd())
                line = in.readLine();
            while(!line.startsWith("}}}}") && !in.atEnd()){
                text += "\n" + line;
                line = in.readLine();
            }
            //if(in.atEnd())
            //    text += "\n" + line;
            QStringList list = text.split("}}}");
            checkhash = list[0];
            salt = list[1];
            newUser = false;
            /*
            hash = encodeDecode(hash.toUtf8().data(),hash.toUtf8().length(),
                                             pass.toUtf8().data(),pass.toUtf8().length());
            salt = encodeDecode(salt.toUtf8().data(),salt.toUtf8().length(),
                                             pass.toUtf8().data(),pass.toUtf8().length());
                                             */
            //checkhash = encode(checkhash, pass);
            //salt = encode(salt, pass);
            if(checkhash == sha256(pass + salt).simplified()){
            //if(hash == sha256(ui->password->text())){
                hash = checkhash;
                //decodeArchive();
               // needDecode = true;
                *login = ui->login->text();
                this->close();
            }
            else{
                ui->errorlabel->setText("Неправильный пароль");
                //*login = "Corporate";
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
    *login = "Corporate";
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


