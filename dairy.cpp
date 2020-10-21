#include "dairy.h"
#include "ui_dairy.h"
#include "QDate"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QFont>
#include <unordered_set>

//Установка связи с определённым файлом, принадлежащем пользователю
void TIndexes::initFile(QString filename){
    file.setFileName(filename);
};

void TIndexes::saveNew(QString date, qint64 pos){
    if (checkIndex(date)==-1){
        date.truncate(date.length()-3);
        QTextStream in(&file);          //Подключаем к файлу поток ввода in
        file.open(QIODevice::Append | QIODevice::Text); //Открытие файла для добавления
        in << date + "&" + QString::number(pos) << endl;   //Запись в файл
        file.close();                   //Закрытие файла
    }
};

void TIndexes::updateIndexes(QString date, qint64 position, qint64 length, bool flag){
    date.truncate(date.length()-3);
    QString line;
    QFile newFile("newBook.txt");
    QTextStream out(&file);
    QTextStream in(&newFile);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    newFile.open(QIODevice::WriteOnly | QIODevice::Text);
    while(!out.atEnd()){
        line = out.readLine();
        if(line[0]!='&' && line.split(QLatin1Char('&'))[0]<=date)
            in << line << endl;
        else if(line[0]!='&' && line.split(QLatin1Char('&'))[0]>date){
            QStringList list = line.split(QLatin1Char('&'));
            in << list[0] + '&' + QString::number(list[1].toULongLong()+length+1) << endl;
        }else if(line[0]=='&' && flag){
            in << '&' + QString::number(line.replace(0,1,' ').trimmed().toULongLong()+length+1) << endl;
        }else if(line[0]=='&' && !flag){
            in << '&' + QString::number(position) << endl;
        }
    }
    file.close();
    newFile.close();
    file.remove();
    newFile.rename("BookMarks.txt");
};

qint64 TIndexes::checkIndex(QString targetDate){
    QString str; qint64 pos;
    targetDate.truncate(targetDate.length()-3);
    QTextStream stream(&file);      //Подключение потока к файлу
    file.open(QIODevice::ReadOnly | QIODevice::Text);   //Открытие файла
    str = stream.readLine();        //Считывание первой строки
    QStringList list = str.split(QLatin1Char('&'));
    while(list[0]!=targetDate && !stream.atEnd()){  //Если строка не является искомой датой и файл не закончился
        str = stream.readLine();    //то считываем следующую строку и проверяем её
        list = str.split(QLatin1Char('&'));
    }
    if(stream.atEnd() && list[0]!=targetDate) pos = -1;    //Если файл закончился и искомая дата не встретилась, то вернём -1
    else pos = list[1].toULongLong();        //Иначе считываем позицию в файле, когда начинается искомая запись
    file.close();                   //Закрытие файла
    return pos;                     //Возвращаем позицию записи искомой даты или -1
};


//Возвращает переданную дату в формате "9 Декабря 2020 Ср"
QString TDate::dateString(QDate selDate){
    QString month = selDate.toString("MMMM");  //month содержит текущий месяц
    QString s = selDate.toString("d MMMM");    //s содержит число и месяц
    s.truncate(s.length()-1);                               //Удаляем из s последний символ месяца
    if (month == "Март" || month == "Август") s = s + "та";  //В зависимости от текущего месяца добавляем к s
    else s = s + "я";                                       //символ 'а' или 'я' для родительного падежа
    return s + selDate.toString(" yyyy ddd");  //Прибавляем к s год и день недели
}
//Конструктор TDate для инициализации поля текущей даты
TDate::TDate(){
    currentDate = QDate::currentDate().toString("yyyy.MM.dd"); //Формирование строки из текущей даты
    selectedDate = currentDate;                     //Дневник по умолчанию открывается на текущей дате
};
//Конструктор TRecord для инициализации полей одной записи в дневнике
TRecord::TRecord(QString ntext, QString ndate, QString nchangeDate)
{
    text = ntext;               //Инициализация текста записи
    date = ndate;               //Инициализация даты события
    changeDate = nchangeDate;   //Инициализация даты последнего изменения текста
}
//Установка связи журнала с определённым файлом, принадлежащем пользователю
void TJournal::initFile(QString filename){
    file.setFileName(filename);
};
//Добавить в конец журнала запись rec
void TJournal::saveNew(TRecord rec){
    QTextStream inf(&file);          //Подключаем к файлу поток ввода in
    file.open(QIODevice::Append | QIODevice::Text); //Открытие файла для добавления
    QTextStream book(&bookmarks.file);          //Подключаем к файлу поток ввода/вывода
    bookmarks.file.open(QIODevice::ReadWrite | QIODevice::Text); //Открытие файла
    qint64 position = inf.pos();
    QString line;
    if(position != 0){
        file.close();
        file.open(QIODevice::ReadWrite | QIODevice::Text); //Открытие файла для добавления
        line = book.readLine();
        while(line[0]!='&'){
            line = book.readLine();
        }
        bookmarks.file.close();
        line.replace(0,1,' ');
        inf.seek(line.trimmed().toULongLong());
        line = inf.readLine();
        if('&' + rec.date < line){
            QFile newFile("newDairy.txt");
            QString targetDate = '&' + rec.date;
            QTextStream outf(&newFile);
            newFile.open(QIODevice::WriteOnly | QIODevice::Text);
            bool flag = true;
            inf.seek(0);
            while(!inf.atEnd()){
                line = inf.readLine();
                if (flag && line[0] == '&' && line > targetDate){
                    bookmarks.updateIndexes(rec.date, position, (rec.date+rec.text+rec.changeDate).size() + 7, true);
                    position = outf.pos();
                    bookmarks.saveNew(rec.date, position);
                    outf << '&' + rec.date << endl;
                    outf << rec.text << endl;
                    outf << rec.changeDate << endl;
                    flag = false;
                }
                outf << line << endl;
            }
            file.close();
            newFile.close();
            file.remove();
            newFile.rename("DairyRecords.txt");
        }else{
            bookmarks.saveNew(rec.date, position);
            bookmarks.updateIndexes(rec.date, position, (rec.date+rec.text+rec.changeDate).size() + 7, false);
            inf.seek(position);
            inf << '&' + rec.date << endl;   //Запись в файл даты события,
            inf << rec.text << endl;         //текста записи
            inf << rec.changeDate << endl;   //и даты последнего его изменения
            file.close();                   //Закрытие файла
        }
    } else {
        book << "&0" << endl;
        bookmarks.file.close();
        bookmarks.saveNew(rec.date, 0);
        inf << '&' + rec.date << endl;   //Запись в файл даты события,
        inf << rec.text << endl;         //текста записи
        inf << rec.changeDate << endl;   //и даты последнего его изменения
        file.close();                   //Закрытие файла
    }
/*
    QTextStream in(&file);          //Подключаем к файлу поток ввода in
    file.open(QIODevice::Append | QIODevice::Text); //Открытие файла для добавления
    qint64 position = file.pos();
    in << '&' + rec.date << endl;   //Запись в файл даты события,
    in << rec.text << endl;         //текста записи
    in << rec.changeDate << endl;   //и даты последнего его изменения
    file.close();                   //Закрытие файла
*/
};
//Найти в журнале позицию текста, дата которого targetDate
qint64 TJournal::findRecord(QString targetDate){
    QString str; qint64 pos;
    targetDate = '&' + targetDate;
    QTextStream stream(&file);      //Подключение потока к файлу
    file.open(QIODevice::ReadOnly | QIODevice::Text);   //Открытие файла
    stream.seek(bookmarks.checkIndex(targetDate));
    str = stream.readLine();        //Считывание первой строки
    while(str!=targetDate && !stream.atEnd()){  //Если строка не является искомой датой и файл не закончился
        str = stream.readLine();    //то считываем следующую строку и проверяем её
    }
    if(stream.atEnd()) pos = -1;    //Если файл закончился и искомая дата не встретилась, то вернём -1
    else pos = stream.pos();        //Иначе считываем позицию в файле, когда начинается искомая запись
    file.close();                   //Закрытие файла
    return pos;                     //Возвращаем позицию записи искомой даты или -1
};
//Перезаписать запись rec
void TJournal::rewrite(TRecord rec){
    QString line;
    QFile newFile("newDairy.txt");
    QString targetDate = '&' + rec.date;
    QTextStream out(&file);
    QTextStream in(&newFile);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    newFile.open(QIODevice::WriteOnly | QIODevice::Text);
    bool flag = true;
    while(!out.atEnd()){
        line = out.readLine();
        if (line == targetDate){
            in << line << endl;
            in << rec.text << endl;
            in << rec.changeDate << endl;
            line[0]='a';
            while(line[0]!='&' && !out.atEnd())
                line = out.readLine();
            if (out.atEnd())
                flag = false;
        }
        if (flag)
            in << line << endl;
    }
    file.close();
    newFile.close();
    file.remove();
    newFile.rename("DairyRecords.txt");
};

QString TJournal::readText(qint64 pos){
    QString text = "";
    QTextStream stream(&file);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    stream.seek(pos);
    QString line = stream.readLine();
    int count = 0;
    while(line[0]!='&' && !stream.atEnd()){
        count++;
        line = stream.readLine();
    }
    if (stream.atEnd())
        count++;
    stream.seek(pos);
    for(int i=1;i<count;i++){
        line = stream.readLine();
        text = text + line + '\n';
    }
    file.close();
    return text;
};

Dairy::Dairy(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Dairy)
{
    ui->setupUi(this);                  //Инициализация графического интерфейса
    QStringList fonts = {"MS Shell Dlg2", "Arial Narrow", "Arimo", "Century Gothic","Constantia","Gabriola",
                         "Garamond","Georgia","Monotype Corsiva","Palatino Linotype","ScriptS","Seqoe Print"};
    ui->fontSelect->addItems(fonts);
    records.initFile("DairyRecords.txt");
    records.bookmarks.initFile("BookMarks.txt");
    ui->label->setText(date.dateString(QDate::currentDate()));   //Запись в label текущей даты в формате "9 Декабря 2020 Ср"
    ui->dateEdit_day->setDate(QDate::currentDate());
    ui->dateEdit_month->setDate(QDate::currentDate());
    ui->dateEdit_year->setDate(QDate::currentDate());



    qint64 pos=-1;
    pos = records.findRecord(date.selectedDate);
    if (pos!=-1){
        rewritingFlag = true;
        QStringList list = records.readText(pos).split(QLatin1Char('}'));
        ui->textEdit->setText(list[1]);
        if (list.size()>2)
            ui->textEdit_2->setText(list[2]);
    }
    std::unordered_set <QDateEdit *> Edits = {ui->dateEdit_day, ui->dateEdit_month, ui->dateEdit_year};
    Cal = new DateWindow(this,Edits);
}
//Освобождение памяти для интерфейса дневника и календаря
Dairy::~Dairy()
{
    delete ui;
    delete Cal;
}

void Dairy::on_saveButton_clicked()
{
    TRecord newRecord("}", date.selectedDate, date.currentDate);    //Формирование записи для сохранения в файле
    newRecord.text = "}" + ui->textEdit->toPlainText() + "\n}" + ui->textEdit_2->toPlainText();
    if (rewritingFlag){         //Если производится перезапись(изменение) прошлой записи
        QMessageBox msgBox;     //Создание окна для вывода сообщения
        msgBox.setWindowTitle("Подтверждение перезаписи");
        msgBox.setText("Вы уверены, что хотите удалить старую запись и сохранить вместо неё новую?");
        QAbstractButton *YesButton = msgBox.addButton("Да", QMessageBox::YesRole);  //Создание кнопок ответа
        QAbstractButton *NoButton = msgBox.addButton("Нет", QMessageBox::NoRole);
        msgBox.setIcon(QMessageBox::Question);      //Установка иконки для вопросительного сообщения
        msgBox.exec();                              //Вывод сообщения
        if(msgBox.clickedButton() == YesButton)     //В случае подтверждения происходит перезапись
            records.rewrite(newRecord);             //Изменить запись NewRecord в журнале records
    } else {                            //Иначе, если запись новая
        records.saveNew(newRecord);     //То сохранить её в конце файла
        rewritingFlag = true;           //Поставить флаг перезаписи на случай изменения этой же записи
    }
}
//Открытие дневника на выбранной дате
void Dairy::on_findButton_clicked()
{
    QDate sel;      //Формирование даты из полей ввода
    sel.setDate(ui->dateEdit_year->date().year(),ui->dateEdit_month->date().month(),ui->dateEdit_day->date().day());
    ui->label->setText(date.dateString(sel));   //Вывод выбранной даты в заголовке
    date.selectedDate = ui->dateEdit_year->date().toString("yyyy.")+
       ui->dateEdit_month->date().toString("MM.") + ui->dateEdit_day->date().toString("dd");
    qint64 pos = records.findRecord(date.selectedDate); //Поиск записи по датей из полей ввода
    if (pos!=-1){                   //Если запись найдена
        rewritingFlag = true;       //То страницы заполнены и возможна перезапись
        QStringList list = records.readText(pos).split(QLatin1Char('}'));   //Разделение текста на две страницы
        ui->textEdit->setText(list[1]);         //Вывод первой части текста
        if (list.size()>2)                      //Если имеется вторая часть текста
            ui->textEdit_2->setText(list[2]);   //То вывести её на второй странице
    } else {                        //Если запись не найдена
        rewritingFlag = false;      //То страница пустая и перезапись не потребуется
        ui->textEdit->clear();      //Очистка страниц от текста предыдущей даты
        ui->textEdit_2->clear();
    }
}
//Открыть календарь по нажатию кнопки
void Dairy::on_pushButton_clicked()
{
    Cal->show();    //Показать окно с календарём
}
//Изменение шрифта в comboBox
void Dairy::on_fontSelect_currentTextChanged(const QString &arg1)
{
    QFont font(arg1,ui->fontSize->value()); //Создание выбранного шрифта
    ui->textEdit->setFont(font);            //Применение шрифта к страницам дневника
    ui->textEdit_2->setFont(font);
    QFont title(arg1, 30);                  //Создание выбранного шрифта для заголовка
    ui->label->setFont(title);              //Применение шрифта к заголовку
}
//Изменение значения в поле размера шрифта
void Dairy::on_fontSize_valueChanged(int arg1)
{
    QFont font(ui->fontSelect->currentText(),arg1); //Создание шрифта размером arg1 из spinBox
    ui->textEdit->setFont(font);                    //Применение шрифта к страницам дневника
    ui->textEdit_2->setFont(font);
}
