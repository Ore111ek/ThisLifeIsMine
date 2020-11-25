#include "planner.h"
#include "ui_planner.h"
#include <QMessageBox>
#include <QComboBox>

QString TList::listText(QListWidget *list){
    QString text = "";
    int number = list->count();
    if (number != 0){
        text = list->item(0)->text() + "}" + (list->item(0)->font().strikeOut() ? "1" : "0");
        for(int i = 1; i<number; i++)
            text += "}" + list->item(i)->text() + "}" + (list->item(i)->font().strikeOut() ? "1" : "0");
    }
    return text;
}

void TList::strikeOutItem(QListWidgetItem *item){
QFont newFont = item->font();
newFont.setStrikeOut(!newFont.strikeOut());
item->setFont(newFont);
}

void TList::setEditableItem(QListWidgetItem *item){
QFont newFont = item->font();
newFont.setStrikeOut(false);
item->setFont(newFont);
item->setFlags (item->flags () | Qt::ItemIsEditable);
}

void TList::addNew(QListWidget *updlist, QListWidgetItem *item){
    item->setText("Задача");
    updlist->insertItem(updlist->count(),"Добавить задачу");
}

void TList::delEmpty(QListWidget *updlist, QListWidgetItem *item){
    if(item->text() == ""){
        for(int i = 0; i<updlist->count()-1; i++){
            if(updlist->item(i)->text()=="")
                updlist->takeItem(i);
        }
    }
}

QString TTable::tableText(QTableWidget *table){
    QString text = "";
    int number = table->rowCount();
    if (number != 0){
        text = table->item(0,0)->text() + "}" + table->item(0,1)->text() + "}" + (table->item(0,1)->font().strikeOut() ? "1" : "0");
        for(int i = 1; i<number-1; i++)
            text += "}" + table->item(i,0)->text() + "}" + table->item(i,1)->text() + "}" + (table->item(i,1)->font().strikeOut() ? "1" : "0");
    }
    return text;
}

void TTable::strikeOutItem(QTableWidgetItem *item){
    QFont newFont = item->font();
    newFont.setStrikeOut(!newFont.strikeOut());
    item->setFont(newFont);
}

void TTable::addNew(QTableWidget *updtable, QTableWidgetItem *item){
    updtable->item(item->row(),1)->setText("Задача");
    updtable->setItem(updtable->rowCount()-2,0,new QTableWidgetItem("   Время"));
    updtable->insertRow(updtable->rowCount());
    updtable->setItem(updtable->rowCount()-2,1,new QTableWidgetItem("Добавить новую строку"));
    updtable->setItem(updtable->rowCount()-2,0,new QTableWidgetItem("         +"));
}

void TTable::delEmpty(QTableWidget *updtable, QTableWidgetItem *item){
    if(item->text() == ""){
        for(int i = 0; i<updtable->rowCount()-2; i++){
            if(updtable->item(i,1)->text()=="")
                updtable->removeRow(i);
        }
    }
}

void TPlannerSettings::initFile(QString name){
    file.setFileName(name);
}

QStringList TPlannerSettings::Categories(QString username){
    QString line;
    QTextStream stream(&file);      //Подключение потока к файлу
    file.open(QIODevice::ReadOnly | QIODevice::Text);   //Открытие файла
    line = stream.readLine();
    QStringList params = line.split(QString("}}}"));
    while(params[0] != username && !stream.atEnd()){
        line = stream.readLine();
        params = line.split(QString("}}}"));
    }
    file.close();
    if(params[0] == username){
        QStringList categories = params[1].split(QString("}"));
        return categories;
    } else return {""};
}

void TPlannerSettings::saveCategories(QString username, QStringList categories){
    QStringList params;
    QString line;
    QFile newFile("newSettings.txt");      //Создаём новый файл, в который перепишем обновлённый журнал
    QTextStream out(&file);             //Подключаем потоки к файлам
    QTextStream in(&newFile);
    file.open(QIODevice::ReadOnly | QIODevice::Text);       //Открываем файлы
    newFile.open(QIODevice::WriteOnly | QIODevice::Text);
    bool flag = true;       //flag = false - остановить запись в новый файл
    if(!out.atEnd()){
        while(!out.atEnd()){    //Пока исходный файл ещё не закончен
            line = out.readLine();      //Считываем очередную строку из файла
            params = line.split(QString("}}}"));
            if (params[0] == username){    //Если строка - дата искомой записи
                in << username + "}}}";     //Переписываем такую же дату записи
                in << categories[0];
                for(int i = 1; i < categories.length(); i++){
                    in << "}" + categories[i];
                }
                in << "}}}" << endl;
                if(!out.atEnd())
                    line = out.readLine();  //Пропускаем старую запись
                else flag = false;

            }
            if(flag)
                in << line << endl; //Записываем в newFile очередную строку
        }
    }
    else {
        in << username + "}}}";     //Переписываем такую же дату записи
        in << categories[0];
        for(int i = 1; i < categories.length(); i++){
            in << "}" + categories[i];
        }
        in << "}}}";
    }
    file.close();                       //Закрытие файлов
    newFile.close();
    QString oldname = file.fileName();  //Запоминаем название старого файла
    file.remove();                      //Удаляем старый файл
    newFile.rename(oldname);            //Переименовываем новый файл
}

Planner::Planner(QWidget *parent, QString name) :
    QMainWindow(parent),
    ui(new Ui::Planner)
{
    username = name;                    //Инициализация логина пользователя
    ui->setupUi(this);
    ui->changeLabel->hide();
    ui->acceptButton->hide();
    ui->changeEdit->hide();
    ui->weekDate_day->setDate(QDate::currentDate());            //Установка в DateEdit текущей даты
    ui->weekDate_month->setDate(QDate::currentDate());
    ui->weekDate_year->setDate(QDate::currentDate());

    ui->dayDate_day->setDate(QDate::currentDate());            //Установка в DateEdit текущей даты
    ui->dayDate_month->setDate(QDate::currentDate());
    ui->dayDate_year->setDate(QDate::currentDate());
    QLabel *labelsInit[8] = {ui->label_Mon, ui->label_Tues, ui->label_Wed, ui->label_Thurs,
                             ui->label_Fri, ui->label_Sat, ui->label_Sun};
    QDate Monday = date[0].nearMonday(QDate::currentDate());
    ui->weekLabel_month->setText(date[0].weekString(Monday));
    date[0].selectedDate = Monday.toString("yyyy.MM.dd");
    for(int i = 0; i < 7; i++){
        labels[i] = labelsInit[i];
        labels[i]->setText(Monday.addDays(i).toString("ddd, d MMMM"));
    }
    QListWidget *daysInit[8] = {ui->weekList_Mon, ui->weekList_Tues, ui->weekList_Wed, ui->weekList_Thurs,
     ui->weekList_Fri, ui->weekList_Sat, ui->weekList_Sun, ui->weekList_Notes};
    for(int i = 0; i < 8; i++){
        week[i] = daysInit[i];
        week[i]->clear();

        connect(week[i], SIGNAL(itemDoubleClicked(QListWidgetItem *)),
                this, SLOT(on_weekList_itemDoubleClicked(QListWidgetItem *)));
        connect(week[i], SIGNAL(itemClicked(QListWidgetItem *)),
                this, SLOT(on_weekList_itemClicked(QListWidgetItem *)));
        connect(week[i], SIGNAL(itemChanged(QListWidgetItem *)),
                this, SLOT(on_weekList_itemChanged(QListWidgetItem *)));
    }
    settings.initFile("PlannerSettings.txt");
    ui->week_Categories->clear();
    QStringList categories = settings.Categories(username);
    for(int i = 0; i < categories.length(); i++){
        QWidget *newTab = new QWidget(ui->week_Categories);
        ui->week_Categories->addTab(newTab, categories[i]);
    }
    categoryIndex = ui->week_Categories->currentIndex();
    records[1].initFile(username + "PlannerWRecords.txt");            //Передача в журнал названий
    records[1].bookmarks.initFile(username + "PlWBookMarks.txt");   //файлов для хранения записей и индексов
    records[0].initFile(username + "PlannerDRecords.txt");            //Передача в журнал названий
    records[0].bookmarks.initFile(username + "PlDBookMarks.txt");   //файлов для хранения записей и индексов

    searchFlag = true;
    on_weekFindButton_clicked();
    on_dayFindButton_clicked();
    changeFlagW = false;
    changeFlagD = false;
    std::unordered_set <QDateEdit *> weekEdits = {ui->weekDate_day, ui->weekDate_month, ui->weekDate_year};
    Cal[0] = new DateWindow(this, weekEdits, ui->weekFindButton);  //Выделение памяти для календаря и передача ему управления над DateEdits
    std::unordered_set <QDateEdit *> dayEdits = {ui->dayDate_day, ui->dayDate_month, ui->dayDate_year};
    Cal[1] = new DateWindow(this, dayEdits, ui->dayFindButton);
    //settings.textColor = Qt::red;                       //Установка изначальных цветов
    //settings.backgroundColor = Qt::yellow;              //для выделения текста
}

Planner::~Planner()
{
    QStringList categories;
    for(int i = 0; i < ui->week_Categories->tabBar()->count(); i++)
        categories.append(ui->week_Categories->tabBar()->tabText(i));
    settings.saveCategories(username, categories);
    delete Cal[0];
    delete Cal[1];
    delete ui;
}


void Planner::on_weekSaveButton_clicked()
{
    if(changeFlagW){
        TRecord newRecord("}", date[0].selectedDate, date[0].currentDate);    //Формирование записи для сохранения в файле
        //if (ui->textEdit_2->toPlainText()=="") {ui->textEdit_2->insertPlainText(" ");}
        //if (ui->textEdit->toPlainText()=="") {ui->textEdit->insertPlainText(" ");}

        newRecord.text = list.listText(week[0]);
        for(int i = 1; i < 8; i++)
            newRecord.text += "}}" + list.listText(week[i]);
        newRecord.text = ui->week_Categories->tabText(ui->week_Categories->currentIndex()) + "}}" + newRecord.text + "}}}";
        if (rewritingFlagW){         //Если производится перезапись(изменение) прошлой записи
            QMessageBox msgBox;     //Создание окна для вывода сообщения
            msgBox.setWindowTitle("Подтверждение перезаписи в Еженедельник");
            msgBox.setText("Вы уверены, что хотите удалить старую запись и сохранить вместо неё новую?");
            QAbstractButton *YesButton = msgBox.addButton("Да", QMessageBox::YesRole);  //Создание кнопок ответа
            QAbstractButton *NoButton = msgBox.addButton("Нет", QMessageBox::NoRole);
            msgBox.setIcon(QMessageBox::Question);      //Установка иконки для вопросительного сообщения
            msgBox.exec();                              //Вывод сообщения
            if(msgBox.clickedButton() == YesButton){     //В случае подтверждения происходит перезапись

                QString newtext = "";
                qint64 pos = records[1].findRecord(date[0].selectedDate);
                QStringList Categories = records[1].readText(pos).split(QString("}}}"));
                bool flag = true;
                for(int i = 0; i<Categories.length()-1; i++){
                    if(Categories[i].startsWith(ui->week_Categories->tabText(ui->week_Categories->currentIndex()))){
                        newtext += newRecord.text;
                        flag = false;
                    }
                    else{
                        newtext += Categories[i] + "}}}";
                    }
                }
                if(flag)
                    newtext += newRecord.text;
                newRecord.text = newtext;
                records[1].rewrite(newRecord);             //Изменить запись NewRecord в журнале records
            }
        } else {                            //Иначе, если запись новая
            records[1].saveNew(newRecord);     //То сохранить её в конце файла
            rewritingFlagW = true;           //Поставить флаг перезаписи на случай изменения этой же записи
        }
    }
    changeFlagW = false;
}

void Planner::on_weekFindButton_clicked()
{
    if(!searchFlag)
        on_weekSaveButton_clicked();
    searchFlag = false;
    QDate sel;      //Формирование даты из полей ввода
    sel.setDate(ui->weekDate_year->date().year(),ui->weekDate_month->date().month(),ui->weekDate_day->date().day());
    QDate Monday = date[0].nearMonday(sel);
    for(int i = 0; i < 7; i++){
        labels[i]->setText(Monday.addDays(i).toString("ddd, d MMMM"));
    }
    ui->weekLabel_month->setText(date[0].weekString(Monday));
    date[0].selectedDate = Monday.toString("yyyy.MM.dd");
    qint64 pos = records[1].findRecord(date[0].selectedDate); //Поиск записи по дате из полей ввода
    if (pos!=-1){                   //Если запись найдена
        int index = 0;
        rewritingFlagW = true;       //То страницы заполнены и возможна перезапись
        for(int i = 0; i < 8; i++)
            week[i]->clear();
        QStringList Categories = records[1].readText(pos).split(QString("}}}"));
        while(index<Categories.length() && !Categories[index].startsWith(ui->week_Categories->tabText(ui->week_Categories->currentIndex())))
            index++;
        if(index < Categories.length()){
            QStringList days = Categories[index].split(QString("}}"));
            for(int j = 0; j<days.length()-1; j++){
                QStringList tasks = days[j+1].split(QString("}"));
                for (int i = 0; i<tasks.length(); i+=2){
                    if(tasks[i]!=""){
                        week[j]->insertItem(i/2,tasks[i]);
                        if(tasks[i+1][0] == '1'){
                            list.strikeOutItem(week[j]->item(i/2));
//                            QFont strikeFont = week[j]->item(i/2)->font();
//                            strikeFont.setStrikeOut(true);
//                            week[j]->item(i/2)->setFont(strikeFont);
                        }
                    }
                }
            }
        } else {
            for(int i = 0; i < 8; i++){
                week[i]->insertItem(0,"Добавить задачу");
            }
        }
    } else {                        //Если запись не найдена
        rewritingFlagW = false;      //То страница пустая и перезапись не потребуется
        for(int i = 0; i < 8; i++){
            week[i]->clear();
            week[i]->insertItem(0,"Добавить задачу");
        }
    }
    changeFlagW = false;
}

void Planner::on_weekList_itemDoubleClicked(QListWidgetItem *item)
{
    QListWidget *day = item->listWidget();
    if(item != day->item(day->count()-1)){
        list.setEditableItem(item);
    }
    changeFlagW = true;
}

void Planner::on_weekList_itemClicked(QListWidgetItem *item)
{
    QListWidget *day = item->listWidget();
    if(item != day->item(day->count()-1)){
        list.strikeOutItem(item);
    }else{
        list.addNew(day, item);
    }
    changeFlagW = true;
}

void Planner::on_weekList_itemChanged(QListWidgetItem *item)
{
    list.delEmpty(item->listWidget(), item);
    changeFlagW = true;
}

void Planner::on_prevWeekButton_clicked()
{
    QDate sel;      //Формирование даты из полей ввода
    sel.setDate(ui->weekDate_year->date().year(),ui->weekDate_month->date().month(),ui->weekDate_day->date().day());
    sel = date[0].nearMonday(sel).addDays(-7);
    ui->weekDate_year->setDate(sel);
    ui->weekDate_month->setDate(sel);
    ui->weekDate_day->setDate(sel);
    on_weekFindButton_clicked();
}

void Planner::on_nextWeekButton_clicked()
{
    QDate sel;      //Формирование даты из полей ввода
    sel.setDate(ui->weekDate_year->date().year(),ui->weekDate_month->date().month(),ui->weekDate_day->date().day());
    sel = date[0].nearMonday(sel).addDays(7);
    ui->weekDate_year->setDate(sel);
    ui->weekDate_month->setDate(sel);
    ui->weekDate_day->setDate(sel);
    on_weekFindButton_clicked();
}

void Planner::on_calendarButton_clicked()
{
    Cal[0]->show();
}

void Planner::on_delCategoryTab_triggered()
{
    ui->week_Categories->removeTab(ui->week_Categories->currentIndex());
}

void Planner::on_newCategoryTab_triggered()
{
    QWidget *newTab = new QWidget(ui->week_Categories);
    ui->week_Categories->addTab(newTab, tr("Категория"));
}

void Planner::on_week_Categories_tabBarDoubleClicked()
{
    ui->changeLabel->show();
    ui->acceptButton->show();
    ui->changeEdit->show();

}

void Planner::on_acceptButton_clicked()
{
    ui->week_Categories->setTabText(ui->week_Categories->currentIndex(), ui->changeEdit->text());
    ui->changeLabel->hide();
    ui->acceptButton->hide();
    ui->changeEdit->hide();
}

void Planner::on_renameCategory_triggered()
{
    on_week_Categories_tabBarDoubleClicked();
}

void Planner::on_week_Categories_tabBarClicked(int index)
{
    ui->week_Categories->setCurrentIndex(categoryIndex);
    on_weekSaveButton_clicked();
    ui->week_Categories->setCurrentIndex(index);
    categoryIndex = index;
    searchFlag = true;
    on_weekFindButton_clicked();
}


void Planner::on_todolist_itemClicked(QListWidgetItem *item)
{
    on_weekList_itemClicked(item);
    changeFlagD = true;
}

void Planner::on_todolist_itemDoubleClicked(QListWidgetItem *item)
{
    on_weekList_itemDoubleClicked(item);
    changeFlagD = true;
}

void Planner::on_todolist_itemChanged(QListWidgetItem *item)
{
    list.delEmpty(item->listWidget(), item);
    changeFlagD = true;
}

void Planner::on_tableWidget_itemClicked(QTableWidgetItem *item)
{
    QTableWidget *day = item->tableWidget();
    if(item->column()==1 && item->row()<day->rowCount()-2){
        table.strikeOutItem(item);
    }else if(item->row() == day->rowCount()-2){
        table.addNew(day, item);
    }
    changeFlagD = true;
}

void Planner::on_tableWidget_itemChanged(QTableWidgetItem *item)
{
    table.delEmpty(item->tableWidget(), item);
    changeFlagD = true;
}

void Planner::on_tableWidget_itemDoubleClicked(QTableWidgetItem *item)
{
    QFont newFont = item->font();
    newFont.setStrikeOut(false);
    item->setFont(newFont);
    changeFlagD = true;
}

void Planner::on_daySaveButton_clicked()
{
    if(changeFlagD){
        TRecord newRecord("}", date[1].selectedDate, date[1].currentDate);    //Формирование записи для сохранения в файле
        newRecord.text = table.tableText(ui->tableWidget) + "}}}" + list.listText(ui->todolist) + "}}}" + ui->notes->toPlainText();
        if (rewritingFlagD){         //Если производится перезапись(изменение) прошлой записи
            QMessageBox msgBox;     //Создание окна для вывода сообщения
            msgBox.setWindowTitle("Подтверждение перезаписи в Еженедельник");
            msgBox.setText("Вы уверены, что хотите удалить старую запись и сохранить вместо неё новую?");
            QAbstractButton *YesButton = msgBox.addButton("Да", QMessageBox::YesRole);  //Создание кнопок ответа
            QAbstractButton *NoButton = msgBox.addButton("Нет", QMessageBox::NoRole);
            msgBox.setIcon(QMessageBox::Question);      //Установка иконки для вопросительного сообщения
            msgBox.exec();                              //Вывод сообщения
            if(msgBox.clickedButton() == YesButton){     //В случае подтверждения происходит перезапись
                records[0].rewrite(newRecord);             //Изменить запись NewRecord в журнале records
            }
        } else {                            //Иначе, если запись новая
            records[0].saveNew(newRecord);     //То сохранить её в конце файла
            rewritingFlagD = true;           //Поставить флаг перезаписи на случай изменения этой же записи
        }
    }
    changeFlagD = false;
}



void Planner::on_dayFindButton_clicked()
{
    on_daySaveButton_clicked();
    QDate sel;      //Формирование даты из полей ввода
    sel.setDate(ui->dayDate_year->date().year(),ui->dayDate_month->date().month(),ui->dayDate_day->date().day());
    ui->dayLabel_date->setText(date[1].dateString(sel));   //Вывод выбранной даты в заголовке

    date[1].selectedDate = ui->dayDate_year->date().toString("yyyy.")+
       ui->dayDate_month->date().toString("MM.") + ui->dayDate_day->date().toString("dd");
    qint64 pos = records[0].findRecord(date[1].selectedDate); //Поиск записи по дате из полей ввода
    if (pos!=-1){                   //Если запись найдена
        rewritingFlagD = true;       //То страницы заполнены и возможна перезапись
        QStringList widgets = records[0].readText(pos).split(QString("}}}"));   //Разделение текста на две страницы

        QStringList tasks = widgets[0].split(QString("}"));
        ui->tableWidget->setRowCount(1);
        ui->tableWidget->clear();
        for (int i = 0; i<tasks.length(); i+=3){
            ui->tableWidget->insertRow(i/3);
            ui->tableWidget->setItem(i/3,0,new QTableWidgetItem(tasks[i]));
            ui->tableWidget->setItem(i/3,1,new QTableWidgetItem(tasks[i+1]));
            if(tasks[i+2][0] == '1'){
                table.strikeOutItem(ui->tableWidget->item(i/3,1));
            }
        }
        ui->todolist->clear();
        tasks = widgets[1].split(QString("}"));
        for (int i = 0; i<tasks.length(); i+=2){
            if(tasks[i]!=""){
                ui->todolist->insertItem(i/2,tasks[i]);
                if(tasks[i+1][0] == '1'){
                    QFont strikeFont = ui->todolist->item(i/2)->font();
                    strikeFont.setStrikeOut(true);
                    ui->todolist->item(i/2)->setFont(strikeFont);
                }
            }
        }
        ui->notes->setText(widgets[2]);
    } else {                        //Если запись не найдена
        rewritingFlagD = false;      //То страница пустая и перезапись не потребуется
        ui->tableWidget->setRowCount(2);
        ui->tableWidget->clear();
        ui->tableWidget->setItem(0,1,new QTableWidgetItem("Добавить новую строку"));
        ui->tableWidget->setItem(0,0,new QTableWidgetItem("         +"));
        ui->todolist->clear();      //Очистка страниц от текста предыдущей даты
        ui->todolist->insertItem(0,"Добавить задачу");
        ui->notes->clear();
    }
    changeFlagD = false;
}

void Planner::on_calendButton_clicked()
{
    Cal[1]->show();
}

void Planner::on_prevDayButton_clicked()
{
    QDate sel;      //Формирование даты из полей ввода
    sel.setDate(ui->dayDate_year->date().year(),ui->dayDate_month->date().month(),ui->dayDate_day->date().day());
    sel = sel.addDays(-1);
    ui->dayDate_year->setDate(sel);
    ui->dayDate_month->setDate(sel);
    ui->dayDate_day->setDate(sel);
    on_dayFindButton_clicked();
}

void Planner::on_nextDayButton_clicked()
{
    QDate sel;      //Формирование даты из полей ввода
    sel.setDate(ui->dayDate_year->date().year(),ui->dayDate_month->date().month(),ui->dayDate_day->date().day());
    sel = sel.addDays(1);
    ui->dayDate_year->setDate(sel);
    ui->dayDate_month->setDate(sel);
    ui->dayDate_day->setDate(sel);
    on_dayFindButton_clicked();
}

void Planner::on_notes_textChanged()
{
    changeFlagD = true;
}
