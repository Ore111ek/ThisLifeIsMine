#include "calendar.h"
#include "ui_calendar.h"
#include <QColorDialog>

FemaleCal::FemaleCal(QTableWidget *tab, QDate month, QString name){
    table = tab;
    shownMonth = month;
    username = name;
    settingsFile.setFileName("fSettings.txt");
    readFemaleSettings();
    for(int i = 0; i < 6; i++){
            for(int j = 0; j < 7; j++){
                table->setItem(i,j,new QTableWidgetItem(""));
                table->item(i,j)->setTextAlignment(Qt::AlignTop);
                table->item(i,j)->setTextColor(QColor(20,20,20));
            }
    }
    //markDays(QDate::currentDate());
}
void FemaleCal::saveFemaleSettings(){
    QStringList params;
    QString line;
    QFile newFile("newSettings.txt");      //Создаём новый файл, в который перепишем обновлённый журнал
    QTextStream out(&settingsFile);             //Подключаем потоки к файлам
    QTextStream in(&newFile);
    settingsFile.open(QIODevice::ReadOnly | QIODevice::Text);       //Открываем файлы
    newFile.open(QIODevice::WriteOnly | QIODevice::Text);
    bool flag = true;       //flag = false - остановить запись в новый файл
    if(!out.atEnd()){
        while(!out.atEnd()){    //Пока исходный файл ещё не закончен
            line = out.readLine();      //Считываем очередную строку из файла
            params = line.split(QString("}}}"));
            if (params[0] == username){    //Если строка - дата искомой записи
                in << username + "}}}" + QString::number(cycle) + "}}}" + QString::number(duration) + "}}}"
                      + (stage?"1":"0") + "}}}" + backColor.name() + "}}}" + numColor.name() + "}}}";     //Переписываем такую же дату записи
                //in << categories[0];
                QMap<QDate, int>::const_iterator i = settings.constBegin();
                while (i != settings.constEnd()) {
                    in << i.key().toString("yyyy.MM.dd") << "}" << i.value() << "}";
                    ++i;
                }
                if(!out.atEnd())
                    line = out.readLine();  //Пропускаем старую запись
                else flag = false;

            }
            if(flag)
                in << line << endl; //Записываем в newFile очередную строку
        }
    }
    else {
        in << username + "}}}" + QString::number(cycle) + "}}}" + QString::number(duration) + "}}}" + (stage?"1":"0") + "}}}"
               + backColor.name() + "}}}" + numColor.name() + "}}}";     //Переписываем такую же дату записи
        QMap<QDate, int>::const_iterator i = settings.constBegin();
        while (i != settings.constEnd()) {
            in << i.key().toString("yyyy.MM.dd") << "}" << i.value() << "}";
            ++i;
        }
    }
    settingsFile.close();                       //Закрытие файлов
    newFile.close();
    QString oldname = settingsFile.fileName();  //Запоминаем название старого файла
    settingsFile.remove();                      //Удаляем старый файл
    newFile.rename(oldname);            //Переименовываем новый файл
}
// username}}}28}}}7}}}1}}}date1}6}date2}5}date3}7}
void FemaleCal::readFemaleSettings(){
    QString line;
    QTextStream stream(&settingsFile);      //Подключение потока к файлу
    settingsFile.open(QIODevice::ReadOnly | QIODevice::Text);   //Открытие файла
    line = stream.readLine();
    QStringList params = line.split(QString("}}}"));
    while(params[0] != username && !stream.atEnd()){
        line = stream.readLine();
        params = line.split(QString("}}}"));
    }
    settingsFile.close();
    if(params[0] == username){
        cycle = params[1].toInt();
        duration = params[2].toInt();
        stage = (params[3]=="1"?true:false);
        backColor = params[4];
        backColor.setAlpha(90);
        numColor = params[5];
        QStringList dates = params[6].split(QString("}"));
        for(int i = 0; i < dates.length()/2; i++){
            settings.insert(QDate::fromString(dates[2*i],"yyyy.MM.dd"),dates[2*i+1].toInt());
        }
    }else{
        cycle = 28;
        duration = 7;
        settings.clear();
        stage = false;
    }
}

void FemaleCal::markDays(QDate firstDayInTable){

    for(int i = 0; i < 6; i++){
            for(int j = 0; j < 7; j++){

                table->item(i,j)->setTextColor(QColor(20,20,20));
                table->item(i,j)->setText("");
                table->item(i,j)->setBackground(QBrush(QColor(255,255,255,0)));
            }
    }

    if(settings.count()>0){
        int row = 0, col = 0;
        QDate selDate = firstDayInTable;
        QMap<QDate, int>::const_iterator i = settings.constBegin();
        QDate prevDate = i.key();
        int prevDur = i.value();
        ++i;
        while(row < 6){
            while(i != settings.constEnd() && i.key() <= selDate){
                prevDate = i.key();
                prevDur = i.value();
                ++i;
            }
            while(i == settings.constEnd() && selDate>=prevDate.addDays(cycle)){
                prevDate = prevDate.addDays(cycle);
                prevDur = duration;
                backColor.setAlpha(30);
            }
            if(prevDate.daysTo(selDate) >= 0){
                if(prevDate.daysTo(selDate) < prevDur){
                    table->item(row,col)->setBackground(QBrush(backColor));
                    table->item(row,col)->setTextColor(numColor);
                }

                table->item(row,col)->setText(QString::number(prevDate.daysTo(selDate)+1));
            }
            selDate = selDate.addDays(1);
            col++;
            if(col>6){
                row++;
                col = 0;
            }
        }
    }
    backColor.setAlpha(90);
}

FemaleCal::~FemaleCal(){}

Calendar::Calendar(QWidget *parent, QString name) :
    QMainWindow(parent),
    ui(new Ui::Calendar)
{
    username = name;
    records.initFile(username + "CalendarRecords.txt");            //Передача в журнал названий
    records.bookmarks.initFile(username + "CalBookMarks.txt");   //файлов для хранения записей и индексов

    ui->setupUi(this);
    ui->femaleSettingsWidget->hide();
    fwidget = new FemaleCal(ui->femaleWidget,QDate::currentDate(),username);
    QString style = "QPushButton {background-color:" + fwidget->numColor.name() + ";border:1px solid #555;width:25px;height:25px;}";
    ui->numColorBtn->setStyleSheet(style);
    style = "QPushButton {background-color:" + fwidget->backColor.name() + ";border:1px solid #555;width:25px;height:25px;}";
    ui->backColorBtn->setStyleSheet(style);

    ui->backColorBtn->hide();
    ui->backColorLabel->hide();
    ui->numColorBtn->hide();
    ui->numColorLabel->hide();

    int index = tableFromDate(ui->calendarWidget->selectedDate());
    curRow = index/7;
    curCol = index%7;
    findEvent();
}

Calendar::~Calendar()
{
    saveEvent();
    delete fwidget;
    delete ui;
}

void Calendar::saveEvent()
{
    if(!updateFlag){
        TRecord newRecord("", date.selectedDate, date.currentDate);    //Формирование записи для сохранения в файле
        int row = 0, col = 0;
        while(dateFromTable(row,col).day()!=1){
            col++;
            if(col>6){
                row++;
                col = 0;
            }
        }
        for(int i = 0; i < dateFromTable(row,col).daysInMonth(); i++){
            newRecord.text += ui->tableWidget->item(row, col)->text() + " }" + iconsTable[row][col] + " }";
            col++;
            if(col>6){
                row++;
                col = 0;
            }
        }
        if (rewritingFlag){         //Если производится перезапись(изменение) прошлой записи
            records.rewrite(newRecord);             //Изменить запись NewRecord в журнале records
        } else {                            //Иначе, если запись новая
            records.saveNew(newRecord);     //То сохранить её в конце файла
            rewritingFlag = true;           //Поставить флаг перезаписи на случай изменения этой же записи
        }
    }
}

void Calendar::findEvent()
{
    updateFlag = true;
    for(int i = 0; i < 6; i++){
        for(int j = 0; j < 7; j++){
            //ui->tableWidget->item(j, i)->setIcon(QIcon(""));
            //ui->tableWidget->item(j, i)->icon().setThemeName("");
            delete ui->tableWidget->itemAt(i,j);
            iconsTable[i][j] = "";
        }
    }
    date.selectedDate = ui->calendarWidget->selectedDate().toString("yyyy.")+
       ui->calendarWidget->selectedDate().toString("MM.") + "01";
    qint64 pos = records.findRecord(date.selectedDate); //Поиск записи по дате из полей ввода
    if (pos!=-1){                   //Если запись найдена
        rewritingFlag = true;       //То страницы заполнены и возможна перезапись
        QStringList list = records.readText(pos).split(QString(" }"));   //Разделение текста на две страницы
        int row = 0, col = 0;
        for(int i = 0; i < 6; i++){
            for(int j = 0; j < 7; j++){
                if(dateFromTable(i,j).month() == ui->calendarWidget->selectedDate().month()){
                    ui->tableWidget->setItem(i,j,new QTableWidgetItem(list[2*(row*7+col)]));
                    if(list[2*(row*7+col)+1]!=""){
                        //setIcon(i, j, "");
                        //iconsTable[i][j] = "";
                        setIcon(i, j, list[2*(row*7+col)+1]);
                    }
                    col++;
                    if(col>6){
                        row++;
                        col = 0;
                    }
                }else{
                    ui->tableWidget->setItem(i,j,new QTableWidgetItem(""));
                }
                ui->tableWidget->item(i,j)->setTextAlignment(Qt::AlignBottom);
            }
        }

    } else {                        //Если запись не найдена
        rewritingFlag = false;      //То страница пустая и перезапись не потребуется
        //ui->textEdit->clear();      //Очистка страниц от текста предыдущей даты
        for(int i = 0; i < 6; i++){
            for(int j = 0; j < 7; j++){
                ui->tableWidget->setItem(i,j,new QTableWidgetItem(""));
                ui->tableWidget->item(i,j)->setTextAlignment(Qt::AlignBottom);
                //iconsTable[i][j] = "";
            }
        }
    }
    updateFlag = false;

    fwidget->markDays(dateFromTable(0,0));
}

int Calendar::tableFromDate(QDate sel){
    int col = 0, row = 0;
    while(dateFromTable(row, col)!=sel){
        col++;
        if(col>6){
            row++;
            col = 0;
        }
    }
    return (row*7 + col);
}

QDate Calendar::dateFromTable(int row, int column){
    //QDate sel = ui->calendarWidget->selectedDate();
    QDate sel = QDate(ui->calendarWidget->yearShown(),ui->calendarWidget->monthShown(),1);
    //sel.setDate(sel.year(),sel.month(),1);
    if(sel.dayOfWeek()==1)
        row--;
    while(sel.dayOfWeek()!=1){
//        pos++;
        sel = sel.addDays(-1);
    }
    sel = sel.addDays(row*7 + column);
    return sel;
}

void Calendar::on_tableWidget_cellClicked(int row, int column)
{
    ui->calendarWidget->setSelectedDate(dateFromTable(row, column));
    int index = tableFromDate(ui->calendarWidget->selectedDate());
    //if(!newPageFlag){
        curRow = index/7;
        curCol = index%7;
    //}
    //newPageFlag = false;
}

void Calendar::setIcon(int row, int col, QString path){
    if(iconsTable[row][col] == path){
        QString text =  ui->tableWidget->item(row, col)->text();
        delete ui->tableWidget->itemAt(row, col);
        ui->tableWidget->item(row, col)->icon().setThemeName("");
        ui->tableWidget->setItem(row,col,new QTableWidgetItem(text));
        ui->tableWidget->item(row,col)->setTextAlignment(Qt::AlignBottom);
        iconsTable[row][col] = "";
    }else {
        ui->tableWidget->item(row, col)->setIcon(QIcon(path));
        ui->tableWidget->item(row, col)->icon().setThemeName(path);
        iconsTable[row][col] = path;
    }
    saveEvent();
}

void Calendar::on_icon_gift_triggered()
{
    setIcon(curRow, curCol, ":/icons/img/gift.png");
}

void Calendar::on_icon_party_triggered()
{
    setIcon(curRow, curCol, ":/icons/img/party.png");
}

void Calendar::on_icon_credit_triggered()
{
    setIcon(curRow, curCol, ":/icons/img/credit.png");
}

void Calendar::on_icon_trip_triggered()
{
    setIcon(curRow, curCol, ":/icons/img/trip.png");
}

void Calendar::on_icon_sport_triggered()
{
    setIcon(curRow, curCol, ":/icons/img/sport.png");
}

void Calendar::on_icon_heart_triggered()
{
    setIcon(curRow, curCol, ":/icons/img/heart.png");
}

void Calendar::on_icon_medicine_triggered()
{
    setIcon(curRow, curCol, ":/icons/img/medicine.png");
}

void Calendar::on_icon_important_triggered()
{
    setIcon(curRow, curCol, ":/icons/img/important.png");
}

void Calendar::on_calendarWidget_currentPageChanged(int year, int month)
{
 /*
    if(ui->calendarWidget->selectedDate().isValid()){
        if(ui->calendarWidget->selectedDate().month() != month){
            //ui->calendarWidget->setSelectedDate(dateFromTable(curRow, curCol));
            ui->calendarWidget->setSelectedDate(QDate(year,month,1));
            int index = tableFromDate(ui->calendarWidget->selectedDate());
            curRow = index/7;
            curCol = index%7;
        }else{
            int index = tableFromDate(ui->calendarWidget->selectedDate());
            curRow = index/7;
            curCol = index%7;
        }
    }else if(!ui->calendarWidget->selectedDate().isValid()){
        int index = tableFromDate(QDate(year,month,1));
        curRow = index/7;
        curCol = index%7;
        ui->calendarWidget->setSelectedDate(dateFromTable(curRow, curCol));
    }
    newPageFlag = true;
    findEvent();
    */


    if(ui->calendarWidget->selectedDate().isValid()){
        if(ui->calendarWidget->selectedDate().month() != month)
            ui->calendarWidget->setSelectedDate(QDate(year,month,1));
        int index = tableFromDate(ui->calendarWidget->selectedDate());
        curRow = index/7;
        curCol = index%7;
        }
     else if(!ui->calendarWidget->selectedDate().isValid()){
        int index = tableFromDate(QDate(year,month,1));
        curRow = index/7;
        curCol = index%7;
        ui->calendarWidget->setSelectedDate(dateFromTable(curRow, curCol));
    }
//    newPageFlag = true;
    findEvent();
}

void Calendar::on_tableWidget_itemChanged(QTableWidgetItem *item)
{
    saveEvent();
}

void Calendar::on_saveFemaleSettings_clicked()
{
    if(ui->stageButton->text()=="Начались сегодня")
        fwidget->stage = false;
    else fwidget->stage = true;
    fwidget->saveFemaleSettings();
    ui->femaleSettingsWidget->hide();
    fwidget->markDays(dateFromTable(0,0));
}

void Calendar::on_showFemaleSettings_triggered()
{
    ui->femaleSettingsWidget->show();


    updateTFlag = true;
    while(ui->datesTable->rowCount() != 0){
        ui->datesTable->removeRow(0);
    }

    fwidget->readFemaleSettings();
    ui->periodSpin->setValue(fwidget->duration);
    ui->cycleSpin->setValue(fwidget->cycle);
    if(fwidget->stage){
        ui->stageButton->setText("Закончились сегодня");
    }
    else {
        ui->stageButton->setText("Начались сегодня");
    }
    /*
    QMap<QDate, int>::const_iterator i = fwidget->settings.constBegin();
    int j = 0;
    while (i != fwidget->settings.constEnd()) {
        ui->datesTable->insertRow(ui->datesTable->rowCount());
        ui->datesTable->setItem(j,0, new QTableWidgetItem(i.key().toString("yyyy.MM.dd")));
        ui->datesTable->setItem(j,1, new QTableWidgetItem(QString::number(i.value())));
        ui->datesTable->setItem(j,2, new QTableWidgetItem("28")); //!!!!!!!! Исправить строку на вычисление !!!!!!!!!!!!
        ++i;
        j++;
    }*/
    if(fwidget->settings.count()>0){
        QMap<QDate, int>::const_iterator i = fwidget->settings.constBegin();
        int j = 0;
        QDate prev_date = i.key();
        int prev_value = i.value();
        ++i;
        while (i != fwidget->settings.constEnd()) {
            ui->datesTable->insertRow(ui->datesTable->rowCount());
            ui->datesTable->setItem(j,0, new QTableWidgetItem(prev_date.toString("dd.MM.yyyy")));
            ui->datesTable->setItem(j,1, new QTableWidgetItem(QString::number(prev_value)));
            ui->datesTable->setItem(j,2, new QTableWidgetItem(QString::number(prev_date.daysTo(i.key()))));
            prev_date = i.key();
            prev_value = i.value();
            ++i;
            j++;
        }
        j = ui->datesTable->rowCount();
        ui->datesTable->insertRow(ui->datesTable->rowCount());
        ui->datesTable->setItem(j,0, new QTableWidgetItem(prev_date.toString("dd.MM.yyyy")));
        ui->datesTable->setItem(j,1, new QTableWidgetItem(QString::number(prev_value)));
        ui->datesTable->setItem(j,2, new QTableWidgetItem(QString::number(fwidget->cycle)));
    }
    updateTFlag = false;
}

void Calendar::on_showColorSettings_clicked()
{
    if(ui->backColorBtn->isHidden()){
        ui->backColorBtn->show();
        ui->backColorLabel->show();
        ui->numColorBtn->show();
        ui->numColorLabel->show();
    }else{
        ui->backColorBtn->hide();
        ui->backColorLabel->hide();
        ui->numColorBtn->hide();
        ui->numColorLabel->hide();
    }
}

void Calendar::on_periodSpin_valueChanged(int arg1)
{
    fwidget->duration = arg1;
}

void Calendar::on_cycleSpin_valueChanged(int arg1)
{
    fwidget->cycle = arg1;
}

void Calendar::on_addCycleButton_clicked()
{
    updateTFlag = true;
    fwidget->settings.insert(ui->dateNewCycle->date(),ui->spinNewPeriod->value());
    on_showFemaleSettings_triggered();
    updateTFlag = false;
}

void Calendar::on_backColorBtn_clicked()
{
    QColor color = QColorDialog::getColor(Qt::white,this,"Выберите цвет фона");//Вывод окна для выбора цвета
    if (color.isValid()){           //Выбранный цвет корректен
        color.setAlpha(90);
        fwidget->backColor = color;
        QString style = "QPushButton {background-color:" + color.name() + ";border:1px solid #555;width:25px;height:25px;}";
        ui->backColorBtn->setStyleSheet(style);
    }
}

void Calendar::on_numColorBtn_clicked()
{
    QColor color = QColorDialog::getColor(Qt::white,this,"Выберите цвет чисел на фоне");//Вывод окна для выбора цвета
    if (color.isValid()){           //Выбранный цвет корректен
        fwidget->numColor = color;
        QString style = "QPushButton {background-color:" + color.name() + ";border:1px solid #555;width:25px;height:25px;}";
        ui->numColorBtn->setStyleSheet(style);
    }
}

void Calendar::on_stageButton_clicked()
{
    updateTFlag = true;
    if(fwidget->stage){
        ui->stageButton->setText("Начались сегодня");
        fwidget->stage = false;

        //        int j = ui->datesTable->rowCount();
        //        QDate curDate = QDate::currentDate();
        //        ui->datesTable->insertRow(ui->datesTable->rowCount());
        //        ui->datesTable->setItem(j,0, new QTableWidgetItem(curDate.toString("dd.MM.yyyy")));
        //        ui->datesTable->setItem(j,1, new QTableWidgetItem(QString::number(fwidget->duration)));
        //        ui->datesTable->setItem(j,2, new QTableWidgetItem(QString::number(fwidget->cycle)));

        auto i = fwidget->settings.end();
        --i;
        i.value() = (int)i.key().daysTo(QDate::currentDate()) + 1;
    }
    else {
        ui->stageButton->setText("Закончились сегодня");
        fwidget->stage = true;

        //int j = ui->datesTable->rowCount() - 1;
        //int dur = QDate::fromString(ui->datesTable->item(j, 0)->text(),"dd.MM.yyyy").daysTo(QDate::currentDate())+1;
        //ui->datesTable->setItem(j,1, new QTableWidgetItem(QString::number(dur)));

        fwidget->settings.insert(QDate::currentDate(),fwidget->duration);

    }
    on_saveFemaleSettings_clicked();
    on_showFemaleSettings_triggered();
    updateTFlag = false;
}

void Calendar::on_datesTable_itemChanged(QTableWidgetItem *item)
{
    if(!updateTFlag){
        fwidget->settings.clear();
        for(int i = 0; i < ui->datesTable->rowCount(); i++){
            if(ui->datesTable->item(i, 0)->text() != "" && ui->datesTable->item(i, 1)->text() != "")
            fwidget->settings.insert(QDate::fromString(ui->datesTable->item(i, 0)->text(),"dd.MM.yyyy"),ui->datesTable->item(i, 1)->text().toInt());
        }
    }
    /*
    QMap<QDate, int>::const_iterator i = settings.constBegin();
    while (i != settings.constEnd()) {
        in << i.key().toString("yyyy.MM.dd") << "}" << i.value() << "}";
        ++i;
    }*/
}

void Calendar::on_switchFemaleWidget_triggered()
{
    if(fwidget->table->isHidden()){
        fwidget->table->show();
    }
    else{
        fwidget->table->hide();
    }
}
