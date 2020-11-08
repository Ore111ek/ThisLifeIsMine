#include "dairy.h"
#include "ui_dairy.h"
#include "QDate"
#include <QMessageBox>
#include <QColorDialog>
#include <QFontDialog>
#include <QFont>
#include <unordered_set>
#include <QCloseEvent>

/*!
    \file
    \brief Файл реализации классов для интерфейсной части дневника

    Файл содержит реализацию классов TDiarySettings и Diary
*/
/*!
\param[in] Html - строка HTML для изменения значения параметра
\param[in] param - название параметра, например "font-family"
\param[in] newValue - какое значение параметра нужно установить для параметра
\return HTML строку с изменённым значением параметра param
*/
QString TDiarySettings::correctHtml(QString Html, QString param, QString newValue){
    QString textNew = "";                       //В textNew собираем строку HTML с изменённым параметром
    QStringList list = Html.split(param);       //Исходную строку разделяем на части так, что каждая часть начинается с нашего параметра
    textNew = list[0];                          //list[0] - начало строки до появления первого param
    for(int i = 1; i<list.length(); i++){       //Для каждого list[i] - части начинающейся с param
        textNew += param + ":'" + newValue + "'";//Записываем в textNew название параметра и новое значение
        int j = 0;                              //
        while(list[i][j]!=';')                  //Пропускаем в нашей части строки list[i]
            j++;                                //старое значение параметра до точки с запятой
        while(j<list[i].length()){              //Оставшиеся данные в list[i] не содержат param
            textNew += list[i][j];              //Просто переписываем все символы в textNew
            j++;
        }                                       //Переходим к list[i+1] - следующей части исходной строки Html
    }
    return textNew;                             //Возвращаем собранную строку с изменённым значением параметра param
}
/*!
\param[in] Html - строка HTML для поиска значения параметра
\param[in] param - название параметра, например "font-family"
\return Значение параметра в переданной HTML строке
*/
QString TDiarySettings::findParamHtml(QString Html, QString param){
    QString Value = "";                         //Строка для хранения значения параметра
    QStringList list = Html.split(param);       //Исходную строку разделяем на части так, что каждая часть начинается с нашего параметра
    int i = 1;                                  //первый символ это : - пропускаем его
    while(list[1][i]!=";"){                     //Пока символ не ;
        Value += list[1][i];                    //Добавляем его в значение
        i++;                                    //Переходим к следующему символу
}
    return Value;                               //Возвращаем найденное значение параметра param
}
/*!
\param[in] parent - указатель на родительское окно (меню)
\param[in] name - логин пользователя (по умолчанию "Corporate")

Определяет текущего пользователя, создаёт окно его дневника и открывает в нём сегодняшнюю дату
*/
Dairy::Dairy(QWidget *parent, QString name) :
    QMainWindow(parent),
    ui(new Ui::Dairy)
{
    username = name;                    //Инициализация логина пользователя
    ui->setupUi(this);                  //Инициализация графического интерфейса
    QStringList fonts = {"MS Shell Dlg2", "Arial Narrow", "Arial Unicode MS",
                         "Arimo", "Candara", "Century Gothic", "Comic Sans MS","Constantia","Gabriola",
                         "Garamond","Georgia","Monotype Corsiva","Palatino Linotype","ScriptS","Seqoe Print", "Times New Roman"};
    ui->fontSelect->addItems(fonts);    //Добавление в fontSelect названий шрифтов для выбора пользователем
    records.initFile(username + "DairyRecords.txt");            //Передача в журнал названий
    records.bookmarks.initFile(username + "DRBookMarks.txt");   //файлов для хранения записей и индексов
    ui->label->setText(date.dateString(QDate::currentDate()));  //Запись в label текущей даты в формате "9 Декабря 2020 Ср"
    ui->dateEdit_day->setDate(QDate::currentDate());            //Установка в DateEdit текущей даты
    ui->dateEdit_month->setDate(QDate::currentDate());
    ui->dateEdit_year->setDate(QDate::currentDate());
    qint64 pos = records.findRecord(date.selectedDate);         //Поиск позиции записи на текущую дату в журнале
    if (pos!=-1){               //Запись найдена
        rewritingFlag = true;   //Следовательно кнопка "Сохранить" ведёт к перезаписи
        QStringList list = records.readText(pos).split(QString("}}"));   //Разделение текста на две страницы
        ui->textEdit->setStyleSheet(list[0]);       //Применение стиля CSS к textEdit
        ui->textEdit->setHtml(list[1]);             //Вывод HTML строки на первой странице
        if (list.size()>3){                      //Если имеется вторая часть текста
            ui->textEdit_2->setStyleSheet(list[2]); //Применение стиля CSS к textEdit_2
            ui->textEdit_2->setHtml(list[3]);       //Вывод HTML строки на второй странице
        }
        QString fontFamily = settings.findParamHtml(list[1],"font-family"); //Узнаём имя шрифта
        QString sizeStr = settings.findParamHtml(list[1],"font-size");      //и размер
        int i = 0;                      //размер получен в формате "20pt"
        while(sizeStr[i]!='p')i++;      //Находим индекс символа p
        sizeStr.truncate(i);            //Удаляем pt из строки размера
        ui->textEdit->setFont(QFont (fontFamily, sizeStr.toInt()));         //Применение шрифта к страницам дневника
        ui->textEdit_2->setFont(QFont (fontFamily, sizeStr.toInt()));
        ui->label->setFont(QFont (fontFamily, 30));                         //Применение к Заголовку с датой
        ui->fontSize->setValue(sizeStr.toInt());                            //Синхронизация со SpinBox
    } else {
        on_fontSelect_activated("MS Shell Dlg2");                           //Утановить шрифт по умолчанию
    }
    std::unordered_set <QDateEdit *> Edits = {ui->dateEdit_day, ui->dateEdit_month, ui->dateEdit_year};
    Cal = new DateWindow(this, Edits, ui->findButton);  //Выделение памяти для календаря и передача ему управления над DateEdits
    settings.textColor = Qt::red;                       //Установка изначальных цветов
    settings.backgroundColor = Qt::yellow;              //для выделения текста
    changeFlag = false;
}
//Освобождение памяти для интерфейса дневника и календаря
Dairy::~Dairy()
{
    delete Cal;
    delete ui;
}

void Dairy::closeEvent(QCloseEvent *event)
{
    on_saveButton_clicked();
    event->accept();
}

void Dairy::on_saveButton_clicked()
{
    if (changeFlag){
        TRecord newRecord("}", date.selectedDate, date.currentDate);    //Формирование записи для сохранения в файле
        //if (ui->textEdit_2->toPlainText()=="") {ui->textEdit_2->insertPlainText(" ");}
        //if (ui->textEdit->toPlainText()=="") {ui->textEdit->insertPlainText(" ");}
        newRecord.text = ui->textEdit->styleSheet() + "\n}}" + ui->textEdit->toHtml() + "\n}}" + ui->textEdit_2->styleSheet() + "\n}}" + ui->textEdit_2->toHtml();
        if (rewritingFlag){         //Если производится перезапись(изменение) прошлой записи
            QMessageBox msgBox;     //Создание окна для вывода сообщения
            msgBox.setWindowTitle("Подтверждение перезаписи в Дневник");
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
        changeFlag = false;
    }
}
//Открытие дневника на выбранной дате
void Dairy::on_findButton_clicked()
{
    on_saveButton_clicked();
    QDate sel;      //Формирование даты из полей ввода
    sel.setDate(ui->dateEdit_year->date().year(),ui->dateEdit_month->date().month(),ui->dateEdit_day->date().day());
    ui->label->setText(date.dateString(sel));   //Вывод выбранной даты в заголовке
    ui->textEdit->setStyleSheet("QTextEdit{background: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 0, stop: 0 #FAFBFE, stop: 1 #DCDEF1);border: 1px solid gray;border-radius: 3px;}");
    ui->textEdit_2->setStyleSheet("QTextEdit{background: qlineargradient(x1: 1, y1: 0, x2: 0, y2: 0, stop: 0 #FAFBFE, stop: 1 #DCDEF1);border: 1px solid gray;border-radius: 3px;}");
    ui->textEdit->setTextColor(Qt::black);                          //Установка цвета страниц, текста и его выделения по умолчанию
    ui->textEdit->setTextBackgroundColor(QColor(255,255,255,0));
    ui->textEdit_2->setTextColor(Qt::black);
    ui->textEdit_2->setTextBackgroundColor(QColor(255,255,255,0));
    date.selectedDate = ui->dateEdit_year->date().toString("yyyy.")+
       ui->dateEdit_month->date().toString("MM.") + ui->dateEdit_day->date().toString("dd");
    qint64 pos = records.findRecord(date.selectedDate); //Поиск записи по дате из полей ввода
    if (pos!=-1){                   //Если запись найдена
        rewritingFlag = true;       //То страницы заполнены и возможна перезапись
        QStringList list = records.readText(pos).split(QString("}}"));   //Разделение текста на две страницы
        ui->textEdit->setStyleSheet(list[0]);       //Применение стиля CSS к textEdit
        ui->textEdit->setHtml(list[1]);             //Вывод HTML строки на первой странице
        if (list.length()>3){                      //Если имеется вторая часть текста
            ui->textEdit_2->setStyleSheet(list[2]); //Применение стиля CSS к textEdit_2
            ui->textEdit_2->setHtml(list[3]);       //Вывод HTML строки на второй странице
        }
        QString fontFamily = settings.findParamHtml(list[1],"font-family"); //Узнаём имя шрифта
        QString sizeStr = settings.findParamHtml(list[1],"font-size");      //и размер
        int i = 0;                      //размер получен в формате "20pt"
        while(sizeStr[i]!='p')i++;      //Находим индекс символа p
        sizeStr.truncate(i);            //Удаляем pt из строки размера
        ui->textEdit->setFont(QFont (fontFamily, sizeStr.toInt()));         //Применение шрифта к страницам дневника
        ui->textEdit_2->setFont(QFont (fontFamily, sizeStr.toInt()));
        ui->label->setFont(QFont (fontFamily, 30));                         //Применение к Заголовку с датой
        ui->fontSize->setValue(sizeStr.toInt());                            //Синхронизация со SpinBox
    } else {                        //Если запись не найдена
        rewritingFlag = false;      //То страница пустая и перезапись не потребуется
        ui->textEdit->clear();      //Очистка страниц от текста предыдущей даты
        ui->textEdit_2->clear();
        ui->textEdit->setFontFamily("MS Shell Dlg2");   //Шрифты по умолчанию
        ui->textEdit_2->setFontFamily("MS Shell Dlg2");
        ui->label->setFont(QFont ("MS Shell Dlg2", 30));
    }
    changeFlag = false;
}
//Открыть календарь по нажатию кнопки
void Dairy::on_pushButton_clicked()
{
    Cal->show();    //Показать окно с календарём
}
//Изменение значения в поле размера шрифта
void Dairy::on_fontSize_valueChanged(int arg1)
{
    QFont font(ui->textEdit->font());       //Узнаём текущий шрифт
    font.setPointSize(arg1);                //Изменение размера шрифта на arg1 из spinBox
    ui->textEdit->setFont(font);            //Применение шрифта к самим страницам дневника
    ui->textEdit_2->setFont(font);
    QString textHtml = ui->textEdit->toHtml();  //Читаем текущий текст первой страницы
    QString textNew = settings.correctHtml(textHtml,"font-size",QString::number(arg1)+"pt");//Изменяем font-size текста
    ui->textEdit->setHtml(textNew);             //Записываем текст обратно на первую страницу
    textHtml = ui->textEdit_2->toHtml();        //Читаем текущий текст второй страницы
    textNew = settings.correctHtml(textHtml,"font-size",QString::number(settings.mainFont.pointSize())+"pt");//Изменяем font-size текста
    ui->textEdit_2->setHtml(textNew);           //Записываем текст обратно на вторую страницу
}

void Dairy::on_actionFont_triggered()
{
    bool flag;                      //Флаг корректности выбранного шрифта
    QFont font = QFontDialog::getFont(&flag,this);  //Вывод окна для выбора параметров шрифта
    if (flag)                       //Если шрифт корректен
        settings.mainFont = font;   //То запомнить его как основной шрифт в настройках
    on_actionSetFont_triggered();   //Вызов обработчика для применения шрифта к тексту
}

void Dairy::on_actionColor_triggered()
{
    if (settings.textColor.isValid()){                      //Если текущий цвет текста в настройках корректен
        settings.activeEdit->setTextColor(settings.textColor);//То применить этот цвет к тексту выделенному на последней активной странице
    }
}

void Dairy::on_actionBackground_triggered()
{
    if (settings.backgroundColor.isValid()){                //Если текущий цвет выделения в настройках корректен
        settings.activeEdit->setTextBackgroundColor(settings.backgroundColor);//То применить этот цвет
    }                                                                         //к тексту, выделенному на последней активной странице
}

void Dairy::on_actionPageGradient_triggered()
{
    QColor color = QColorDialog::getColor(Qt::white,this,"Выберите первый цвет для градиента"); //Вывод двух окон для выбора цветов
    QColor color2 = QColorDialog::getColor(Qt::white,this,"Выберите второй цвет для градиента");//начала и конца градиента
    if (color.isValid() && color2.isValid()){       //Если выбранные цвета корректны
        QString gradientStyle = "QTextEdit{background: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 0, stop: 0 ";
        QString gradientReverse = gradientStyle;    //Составление css градиентов для первой и второй страницы
        gradientStyle = gradientStyle + color.name() + ", stop: 1 " + color2.name() +  ");border: 1px solid gray;border-radius: 3px;}";
        gradientReverse = gradientReverse + color2.name() + ", stop: 1 " + color.name() +  ");border: 1px solid gray;border-radius: 3px;}";
        settings.gradientStyleDirect = gradientStyle;   //Добавление в настройки двух градиентов
        settings.gradientStyleReverse = gradientReverse;//с противоположными направлениями цветов
        settings.pageColor = nullptr;                   //Сплошной цвет обнуляется
    }
    on_actionSetPageColor_triggered();                  //Применение градиента к страницам
}

void Dairy::on_actionColorPage_triggered()
{
    QColor color = QColorDialog::getColor(Qt::white,this,"Выберите цвет страниц");//Вывод окна для выбора цвета
    if (color.isValid()){           //Выбранный цвет корректен
        settings.pageColor = color; //то сохранить сплошной цвет страниц в настройках
        settings.gradientStyleDirect = settings.gradientStyleReverse = nullptr;//Обнуляем градиенты
    }
    on_actionSetPageColor_triggered();  //Применение сплошного цвета к страницам
}

void Dairy::on_actionChoiceTextColor_triggered()
{
    settings.textColor = QColorDialog::getColor(Qt::white,this,"Выберите цвет текста");//Вывод окна для выбора цвета
    on_actionColor_triggered(); //Применение цвета к выделенному тексту
    ui->TextColor->setStyleSheet("QPushButton {background-color: " + settings.textColor.name() + "; border: 1px solid #555; width:25px; height:25px;}");
}


void Dairy::on_actionChoiceBackColor_triggered()
{
    settings.backgroundColor = QColorDialog::getColor(Qt::white,this,"Выберите цвет выделения текста");//Вывод окна для выбора цвета
    on_actionBackground_triggered(); //Применение цвета к выделенному тексту
    ui->BackColor->setStyleSheet("QPushButton {background-color: " + settings.backgroundColor.name() + "; border: 1px solid #555; width:25px; height:25px;}");
}

void Dairy::on_actionSetFont_triggered()
{
    ui->textEdit->setFont(settings.mainFont);                   //Применение шрифта к страницам дневника
    ui->textEdit_2->setFont(settings.mainFont);
    ui->fontSize->setValue(settings.mainFont.pointSize());      //Синхронизация со SpinBox
    QString fontStyle = "normal";
    switch (settings.mainFont.style()){                         //Перевод в строковый формат значения style
        case QFont::StyleNormal:fontStyle = "normal";
            break;
        case QFont::StyleItalic:fontStyle = "italic";
            break;
        case QFont::StyleOblique:fontStyle = "oblique";
    }
    QString textHtml = ui->textEdit->toHtml();                  //Читаем исходную строку HTML и изменяем в ней параметры шрифта
    QString textNew = settings.correctHtml(textHtml,"font-family",settings.mainFont.family());
    textNew = settings.correctHtml(textNew,"font-weight",QString::number(settings.mainFont.weight()));
    textNew = settings.correctHtml(textNew,"font-style",fontStyle);
    textNew = settings.correctHtml(textNew,"font-size",QString::number(settings.mainFont.pointSize())+"pt");
    ui->textEdit->setHtml(textNew);                             //Вставляем изменённую строку обратно
    textHtml = ui->textEdit_2->toHtml();                        //Также со второй страницей
    textNew = settings.correctHtml(textHtml,"font-family",settings.mainFont.family());
    textNew = settings.correctHtml(textNew,"font-weight",QString::number(settings.mainFont.weight()));
    textNew = settings.correctHtml(textNew,"font-style",fontStyle);
    textNew = settings.correctHtml(textNew,"font-size",QString::number(settings.mainFont.pointSize())+"pt");
    ui->textEdit_2->setHtml(textNew);
    settings.mainFont.setPointSize(30);
    ui->label->setFont(settings.mainFont);                      //Шрифт для заголовка с датой
}

void Dairy::on_actionSetPageColor_triggered()
{
    if (settings.pageColor != nullptr){     //Определение сплошной цвет или градиент и применение к страницам
        ui->textEdit->setStyleSheet("QTextEdit{background: "+settings.pageColor.name()+";border: 1px solid gray;border-radius: 3px;}");
        ui->textEdit_2->setStyleSheet("QTextEdit{background: "+settings.pageColor.name()+";border: 1px solid gray;border-radius: 3px;}");
    } else if (settings.gradientStyleDirect != nullptr && settings.gradientStyleReverse != nullptr){
        ui->textEdit->setStyleSheet(settings.gradientStyleDirect);
        ui->textEdit_2->setStyleSheet(settings.gradientStyleReverse);
    }
}

void Dairy::on_fontSelect_activated(const QString &arg1)
{
    QFont font(arg1,ui->fontSize->value()); //Создание выбранного шрифта
    ui->textEdit->setFont(font);            //Применение шрифта к страницам дневника
    ui->textEdit_2->setFont(font);
    if(ui->textEdit->toPlainText()==""){ui->textEdit->insertPlainText(" ");}    //Для применения щрифта необходим хотя бы 1 символ
    if(ui->textEdit_2->toPlainText()==""){ui->textEdit_2->insertPlainText(" ");}
    QString textHtml = ui->textEdit->toHtml();                              //Читаем HTML из первой страницы
    QString textNew = settings.correctHtml(textHtml,"font-family",arg1);    //Изменяем название шрифта
    ui->textEdit->setHtml(textNew);                                         //Вставляем обратно
    textHtml = ui->textEdit_2->toHtml();                                    //Также для второй страницы
    textNew = settings.correctHtml(textHtml,"font-family",arg1);
    ui->textEdit_2->setHtml(textNew);
    QFont title(arg1, 30);                  //Создание выбранного шрифта для заголовка
    ui->label->setFont(title);              //Применение шрифта к заголовку
}

void Dairy::on_textColorButton_clicked()
{
    //if(settings.textColor == nullptr)           //Если цвет текста ещё не выбран
    //    on_actionChoiceTextColor_triggered();   //То вывести окно с выбором и применить к выделенному
    //else                                        //Иначе
        on_actionColor_triggered();             //Применить к выделенному тексту
}

void Dairy::on_BackColorButton_clicked()
{
    //if(settings.backgroundColor == nullptr)     //
    //    on_actionChoiceBackColor_triggered();   //
    //else                                        //
        on_actionBackground_triggered();        //
}

void Dairy::on_textEdit_cursorPositionChanged()
{
    settings.activeEdit = ui->textEdit;         //При изменении позиции курсора окно запоминается как последнее активное
}

void Dairy::on_textEdit_2_cursorPositionChanged()
{
    settings.activeEdit = ui->textEdit_2;       //При изменении позиции курсора окно запоминается как последнее активное
}

void Dairy::on_TextColor_clicked()
{
    on_actionChoiceTextColor_triggered();       //Выбор цвета текста при нажатии на соответствующий квадратик
}

void Dairy::on_BackColor_clicked()
{
    on_actionChoiceBackColor_triggered();       //Выбор цвета выделения текста при нажатии на соответствующий квадратик
}

void Dairy::on_textEdit_textChanged()
{
    changeFlag = true;
}

void Dairy::on_textEdit_2_textChanged()
{
    changeFlag = true;
}
