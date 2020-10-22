#include "dairy.h"
#include "ui_dairy.h"
#include "QDate"
#include <QMessageBox>
#include <QFont>
#include <unordered_set>

Dairy::Dairy(QWidget *parent, QString name) :
    QMainWindow(parent),
    ui(new Ui::Dairy)
{
    username = name;
    ui->setupUi(this);                  //Инициализация графического интерфейса
    QStringList fonts = {"MS Shell Dlg2", "Arial Narrow", "Arimo", "Century Gothic","Constantia","Gabriola",
                         "Garamond","Georgia","Monotype Corsiva","Palatino Linotype","ScriptS","Seqoe Print"};
    ui->fontSelect->addItems(fonts);    //Добавление в fontSelect названий шрифтов для выбора пользователем
    records.initFile(username + "DairyRecords.txt");        //Передача в журнал названий
    records.bookmarks.initFile(username + "BookMarks.txt"); //файлов для хранения записей
    ui->label->setText(date.dateString(QDate::currentDate()));   //Запись в label текущей даты в формате "9 Декабря 2020 Ср"
    ui->dateEdit_day->setDate(QDate::currentDate());    //Установка в DateEdit текущей даты
    ui->dateEdit_month->setDate(QDate::currentDate());
    ui->dateEdit_year->setDate(QDate::currentDate());
    qint64 pos = records.findRecord(date.selectedDate); //Поиск позиции записи на текущую дату в журнале
    if (pos!=-1){               //Запись найдена
        rewritingFlag = true;   //Следовательно кнопка "Сохранить" ведёт к перезаписи
        QStringList list = records.readText(pos).split(QLatin1Char('}')); //Разделить считанный текст на части
        ui->textEdit->setText(list[1]); //В textEdit вывести первую часть текста
        if (list.size()>2)              //Если текст не закончен
            ui->textEdit_2->setText(list[2]);   //В textEdit_2 вывести вторую часть текста
    }
    std::unordered_set <QDateEdit *> Edits = {ui->dateEdit_day, ui->dateEdit_month, ui->dateEdit_year};
    Cal = new DateWindow(this,Edits);   //Выделение памяти для календаря и передача ему управления над DateEdits
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
