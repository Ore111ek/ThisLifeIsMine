#include "hobby.h"
#include "ui_hobby.h"


void THobbySettings::initFile(QString name){
    file.setFileName(name);
}



QStringList THobbySettings::habbits(QString username){
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
        QStringList habbits = params[1].split(QString("}"));
        return habbits;
    } else return {};
}

void THobbySettings::saveHabbits(QString username, QStringList habbits){
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
                in << habbits[0];
                for(int i = 1; i < habbits.length(); i++){
                    in << "}" + habbits[i];
                }
                in << "}}}" << endl;
                if(!out.atEnd())
                    line = out.readLine();  //Пропускаем старую запись
                else flag = false;

            }
            if(flag)
                in << line << endl; //Записываем в newFile очередную строку
        }
        if(flag){
            in << username + "}}}";
            in << habbits[0];
            for(int i = 1; i < habbits.length(); i++){
                in << "}" + habbits[i];
            }
            in << "}}}";
        }
    }
    else {
        in << username + "}}}";
        in << habbits[0];
        for(int i = 1; i < habbits.length(); i++){
            in << "}" + habbits[i];
        }
        in << "}}}";
    }
    file.close();                       //Закрытие файлов
    newFile.close();
    QString oldname = file.fileName();  //Запоминаем название старого файла
    file.remove();                      //Удаляем старый файл
    newFile.rename(oldname);            //Переименовываем новый файл
}

Hobby::Hobby(QWidget *parent, QString name) :
    QMainWindow(parent),
    ui(new Ui::Hobby)
{
    ui->setupUi(this);
    ui->tableWidget->horizontalHeader()->setVisible(1);
    ui->tableWidget->verticalHeader()->setVisible(1);
    username = name;                    //Инициализация логина пользователя
    settings.initFile("HabbitsSettings.txt");
    records.initFile(username + "Habbits.txt");            //Передача в журнал названий
    records.bookmarks.initFile(username + "HHBookMarks.txt");   //файлов для хранения записей и индексов
    ui->label->setText(QDate::currentDate().toString("MMMM yyyy"));  //Запись в label текущей даты в формате "9 Декабря 2020 Ср"
    ui->Date_year->setDate(QDate::currentDate());            //Установка в DateEdit текущей даты
    ui->Date_month->setDate(QDate::currentDate());
    std::unordered_set <QDateEdit *> Edits = {ui->Date_month, ui->Date_year};
    Cal = new DateWindow(this, Edits, ui->findButton);  //Выделение памяти для календаря и передача ему управления над DateEdits
    changeFlag = false;
    graph = new TGraph(ui->customPlot);
    on_findButton_clicked();
}

Hobby::~Hobby()
{
    delete Cal;
    delete ui;
}

//void Hobby::on_tableWidget_cellClicked(int row, int column)
//{
//    if(column == 0 && row == ui->tableWidget->rowCount()-2)
//        habbits.addNew(ui->tableWidget, ui->tableWidget->item(row, column));
//}

void Hobby::on_createButton_clicked()
{
    if(ui->habbitEdit->text() != ""){
        int index = ui->tableWidget->rowCount();
    //    ui->tableWidget->insertRow(index);
    //    ui->tableWidget->setVerticalHeaderItem(index-1, new QTableWidgetItem(ui->habbitEdit->text()));
    //    ui->tableWidget->setVerticalHeaderItem(index, new QTableWidgetItem(""));
        ui->tableWidget->insertRow(index-1);
        ui->tableWidget->setVerticalHeaderItem(index-1, new QTableWidgetItem(ui->habbitEdit->text()));
        //ui->tableWidget->setVerticalHeaderItem(index, new QTableWidgetItem(""));
    }
    changeFlag = true;
}

void Hobby::on_findButton_clicked()
{
    save_habbits();
    QDate sel;      //Формирование даты из полей ввода
    sel.setDate(ui->Date_year->date().year(),ui->Date_month->date().month(),1);
    ui->label->setText(sel.toString("MMMM yyyy"));   //Вывод выбранной даты в заголовке

    date.selectedDate = ui->Date_year->date().toString("yyyy.")+
       ui->Date_month->date().toString("MM.") + "01";
    ui->tableWidget->clearContents();
    while(ui->tableWidget->rowCount() != 1){
        ui->tableWidget->removeRow(0);
    }
    while(ui->tableWidget->columnCount() > sel.daysInMonth()){
        ui->tableWidget->removeColumn(ui->tableWidget->columnCount()-1);
    }
    while(ui->tableWidget->columnCount() < sel.daysInMonth()){
        ui->tableWidget->insertColumn(ui->tableWidget->columnCount());
    }
    qint64 pos = records.findRecord(date.selectedDate); //Поиск записи по дате из полей ввода
    if (pos!=-1){                   //Если запись найдена
        rewritingFlag = true;       //То страницы заполнены и возможна перезапись
        //int index;
        QStringList data;
        QStringList list = records.readText(pos).split(QString("\n"));   //Разделение текста на две страницы
        for(int i = 0; i < list.length() - 2; i++){
            data = list[i].split(" }");
            //index = ui->tableWidget->rowCount()-1;
            ui->tableWidget->insertRow(i);
            ui->tableWidget->setVerticalHeaderItem(i, new QTableWidgetItem(data[0]));
            for(int j = 1; j < data.length()-1; j++){
                if(j > ui->tableWidget->columnCount()){
                    ui->tableWidget->insertColumn(ui->tableWidget->columnCount());
                }
                ui->tableWidget->setItem(i,j-1, new QTableWidgetItem(data[j]));
            }
        }
    } else {                        //Если запись не найдена
        rewritingFlag = false;      //То страница пустая и перезапись не потребуется
        QStringList habbits = settings.habbits(username);
        for(int i = 0; i < habbits.length(); i++){
            //index = ui->tableWidget->rowCount()-1;
            ui->tableWidget->insertRow(i);
            ui->tableWidget->setVerticalHeaderItem(i, new QTableWidgetItem(habbits[i]));
        }
     //   ui->textEdit_2->clear();
    }
    changeFlag = false;

    updateComboBox(sel);


    drawGraph();
}

void Hobby::save_habbits()
{
    if(changeFlag){
        TRecord newRecord("", date.selectedDate, date.currentDate);    //Формирование записи для сохранения в файле
        // Привычка }5 }3 }6 }2 }6 }7 }8 }4 }9 }4 }5 }7 }2 }3 }6 }7 }8 }9 }2 }4 }6 }7 }8 }6 }4 }
        for(int i = 0; i < ui->tableWidget->rowCount()-1; i++){
            newRecord.text += ui->tableWidget->verticalHeaderItem(i)->text() + " }";
            for(int j = 0; j < ui->tableWidget->columnCount(); j++){
                if(ui->tableWidget->item(i,j)!=nullptr)
                    newRecord.text += ui->tableWidget->item(i,j)->text() + " }";
                else newRecord.text += " }";
            }
            newRecord.text += "\n";
        }
        if (rewritingFlag){         //Если производится перезапись(изменение) прошлой записи
            records.rewrite(newRecord);             //Изменить запись NewRecord в журнале records
        } else {                            //Иначе, если запись новая
            records.saveNew(newRecord);     //То сохранить её в конце файла
            rewritingFlag = true;           //Поставить флаг перезаписи на случай изменения этой же записи
        }
        QStringList habbits;
        for(int i = 0; i < ui->tableWidget->rowCount()-1; i++){
            habbits.push_back(ui->tableWidget->verticalHeaderItem(i)->text());
        }
        settings.saveHabbits(username, habbits);
        changeFlag = false;
    }
}

void Hobby::on_tableWidget_cellChanged(int row, int column)
{
    changeFlag = true;
}

void Hobby::on_deleteButton_clicked()
{
    QString headName = ui->habbitEdit->text();
    for(int i = ui->tableWidget->rowCount() - 1; i > -1; i--){
        if(ui->tableWidget->verticalHeaderItem(i)->text() == headName && headName != "")
            ui->tableWidget->removeRow(i);
    }
    changeFlag = true;
}

void Hobby::on_calendButton_clicked()
{
    Cal->show();    //Показать окно с календарём
}

void Hobby::on_graphStyleCombo_currentIndexChanged(int index)
{
    drawGraph();
}

void Hobby::on_radioSumMonth_clicked()
{
    updateComboBox(QDate(ui->Date_year->date().year(),ui->Date_month->date().month(),1));
    if(ui->habbitComboBox->currentIndex() != 0)
        ui->habbitComboBox->setCurrentIndex(0);
    else drawGraph();
    ui->habbitComboBox->setDisabled(1);
}

void Hobby::on_radioProgressMonth_clicked()
{
    updateComboBox(QDate(ui->Date_year->date().year(),ui->Date_month->date().month(),1));
    ui->habbitComboBox->setEnabled(1);
    drawGraph();
}

void Hobby::on_radioProgressYear_clicked()
{
    updateComboBox(QDate(ui->Date_year->date().year(),ui->Date_month->date().month(),1));
    ui->habbitComboBox->setEnabled(1);
    drawGraph();
}

void Hobby::on_radioSumYear_clicked()
{
    updateComboBox(QDate(ui->Date_year->date().year(),ui->Date_month->date().month(),1));
    ui->habbitComboBox->setEnabled(1);
    ui->habbitComboBox->setCurrentIndex(1);
    drawGraph();
}

void Hobby::updateComboBox(QDate sel){
    std::vector<TStats> data;
    TStats result;
    if(ui->radioProgressMonth->isChecked() || ui->radioSumMonth->isChecked()){
        data = records.wholeMonthStats(sel);
    }
    if(ui->radioProgressYear->isChecked() || ui->radioSumYear->isChecked()){
        data = records.wholeYearStats(sel);
    }
    QString currentBox = ui->habbitComboBox->currentText();
    updateFlag = true;
    while(ui->habbitComboBox->count()>1)
        ui->habbitComboBox->removeItem(1);
    for(auto hobby: data){
        ui->habbitComboBox->insertItem(ui->habbitComboBox->count(),hobby.name);
    }
    if(currentBox!="Все привычки") {
        data.clear();
        if(ui->radioProgressMonth->isChecked() || ui->radioSumMonth->isChecked()){
            result = records.wholeMonthStats(sel, currentBox);
        }
        if(ui->radioProgressYear->isChecked() || ui->radioSumYear->isChecked()){
            result = records.wholeYearStats(sel, currentBox);
        }
        data.push_back(result);
        if(data.size() > 0){
            int index = 0;
            for(int i = 0; i < ui->habbitComboBox->count(); i++){
                if(ui->habbitComboBox->itemText(i) == data[0].name)
                    index = i;
            }
            ui->habbitComboBox->setCurrentIndex(index);
        }
    }
    updateFlag = false;
}

void Hobby::clearGraph(){
    while(ui->customPlot->graphCount() != 0)
        ui->customPlot->removeGraph(0);
    while(ui->customPlot->itemCount() != 0)
        ui->customPlot->removeItem(0);
    while(ui->customPlot->plottableCount() != 0)
        ui->customPlot->removePlottable(0);
    ui->customPlot->xAxis->setTickLabels(false);
    ui->customPlot->yAxis->setTickLabels(false);
}

void Hobby::drawGraph()
{
    if(!updateFlag){
        clearGraph();
        QDate sel = QDate(ui->Date_year->date().year(),ui->Date_month->date().month(),1);
        QDate selYear = QDate(ui->Date_year->date().year(),1,1);
        //
        std::vector<TStats> data;
        TStats result;

        if(ui->habbitComboBox->currentText()=="Все привычки"){
            if(ui->radioProgressMonth->isChecked()){
                data = records.wholeMonthStats(sel);
            }
            if(ui->radioProgressYear->isChecked()){
                sel = selYear;
                data = records.wholeYearStats(sel);
            }
            if(ui->radioSumMonth->isChecked()){
                data = records.wholeMonthSums(sel);
            }
            if(ui->radioSumYear->isChecked()){
                sel = selYear;
                data = records.wholeYearSums(sel);
            }
        }
        else {
            if(ui->radioProgressMonth->isChecked()){
                result = records.wholeMonthStats(sel, ui->habbitComboBox->currentText());
            }
            if(ui->radioProgressYear->isChecked()){
                sel = selYear;
                result = records.wholeYearStats(sel, ui->habbitComboBox->currentText());
            }
            data.push_back(result);
            if(ui->radioSumMonth->isChecked()){
                data.clear();
                data = records.wholeMonthSums(sel);
            }
            if(ui->radioSumYear->isChecked()){

                result.name = ui->habbitComboBox->currentText();
                for(int i = 1; i < 13; i++){
                    sel = QDate(sel.year(),i,1);
                    result.stats.push_back(records.wholeMonthSums(sel, ui->habbitComboBox->currentText()).stats[0]);
                }
                data.clear();
                data.push_back(result);
            }
        }
        if(data.size()>0){
            switch(ui->graphStyleCombo->currentIndex()){
                case 0: graph->drawLinearGraph1(sel, data);
                    break;
                case 1: graph->drawLinearGraph2(sel, data);
                    break;
                case 2: graph->drawLinearGraph3(sel, data);
                    break;
                case 3: graph->drawBarsGraph1(sel, data);
                    break;
                case 4: graph->drawBarsGraph2(sel, data);
                    break;
            }
        }
        ui->customPlot->replot();
    }
}

std::vector <TStats> THobbyStatistician::wholeMonthStats(QDate date){

    std::vector <TStats> hobbies;
    TStats hobby;
    QStringList results;
    qint64 pos = findRecord(date.toString("yyyy.MM.01")); //Поиск записи по дате из полей ввода
    if (pos!=-1){                   //Если запись найдена
        //int index;
        QStringList data;
        QStringList list = readText(pos).split(QString("\n"));   //Разделение текста на две страницы
        for(int i = 0; i < list.length() - 2; i++){
            data = list[i].split(" }");
            results.clear();
            for(int j = 1; j < data.length()-1; j++){
                results.push_back(data[j]);
            }
            hobby.name = data[0];
            hobby.stats = results;
            hobbies.push_back(hobby);
        }
    }
    return hobbies;
}

TStats THobbyStatistician::wholeMonthStats(QDate date, QString name){

    std::vector <TStats> hobbies;
    TStats hobby;
    hobbies = wholeMonthStats(date);
        for(auto x : hobbies){
            if(x.name == name)
                hobby = x;
        }
    return hobby;
}

std::vector <TStats> THobbyStatistician::wholeYearStats(QDate date){

    std::vector <TStats> hobbies[12];
    TStats hobby;
    QStringList results;
    std::vector <TStats> year;
    bool newFlag = true;

    int space = 0;
    int Longspace = 0;
    for(int month = 0; month < 12; month++){
        date.setDate(date.year(),month+1,1);
        space = date.daysInMonth();

        hobbies[month] = wholeMonthStats(date);
        for(auto newHobby: hobbies[month]){
            newFlag = true;
            for(auto &oldHobby: year){
                if(newHobby.name == oldHobby.name){
                    for(auto day: newHobby.stats)
                        oldHobby.stats.push_back(day);
                    newFlag = false;
                }
            }
            if(newFlag){
                for(int i = 0; i < Longspace; i++)
                    newHobby.stats.push_front("");
                year.push_back(newHobby);
            }

        }
        Longspace += space;
        for(auto &oldHobby: year){
            if(oldHobby.stats.length() != Longspace){
                for(int i = 0; i < space; i++)
                    oldHobby.stats.push_back("");
            }
        }
    }
    return year;
}

TStats THobbyStatistician::wholeYearStats(QDate date, QString name){
    std::vector <TStats> hobbies;
    TStats hobby;
    hobbies = wholeYearStats(date);
        for(auto x : hobbies){
            if(x.name == name)
                hobby = x;
        }
    return hobby;
}

std::vector <TStats> THobbyStatistician::wholeMonthSums(QDate date){
    std::vector <TStats> hobbies = wholeMonthStats(date);
    int sum;
    for(auto &x: hobbies){
        sum = 0;
        for(int i = 0; i < x.stats.length(); i++){
            sum += x.stats[i].toInt();
        }
        x.stats.clear();
        x.stats.push_back(QString::number(sum));
    }
    return hobbies;
}

TStats THobbyStatistician::wholeMonthSums(QDate date, QString name){
    std::vector <TStats> hobbies = wholeMonthSums(date);
    TStats result;
    result.name = "name";
    result.stats.push_back("0");
    for(auto x: hobbies){
        if(x.name == name)
            result = x;
    }
    return result;
}

std::vector <TStats> THobbyStatistician::wholeYearSums(QDate date){
    std::vector <TStats> hobbies = wholeYearStats(date);
    int sum;
    for(auto &x: hobbies){
        sum = 0;
        for(int i = 0; i < x.stats.length(); i++){
            sum += x.stats[i].toInt();
        }
        x.stats.clear();
        x.stats.push_back(QString::number(sum));
    }
    return hobbies;
}

void Hobby::on_habbitComboBox_currentIndexChanged(const QString &arg1)
{
    if(arg1 == "Все привычки" && ui->radioSumYear->isChecked() && ui->habbitComboBox->count()>1)
        ui->habbitComboBox->setCurrentIndex(1);
    drawGraph();
}


void Hobby::on_themeBox_stateChanged(int arg1)
{
    if(ui->themeBox->isChecked())
        graph->blackTheme();
    else graph->whiteTheme();
    drawGraph();
}

void Hobby::on_tabWidget_tabBarClicked(int index)
{
    if(index == 1){
        save_habbits();
        drawGraph();
    }
}
