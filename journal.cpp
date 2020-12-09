#include "journal.h"

#include "QDate"
#include <QFile>
#include <QTextStream>

/*!
    \file
    \brief Файл с реализацией классов для работы с файлами

    Файл содержит реализацию классов TJournal, TRecord, TIndexes и TDate
*/
/*!
\param[in] filename - имя файла с индексами, принадлежащего пользователю
*/
void TIndexes::initFile(QString filename){
    file.setFileName(filename);
};
/*!
\param[in] targetDate - искомая дата в строчном формате "2020.04.15"
\return Позицию искомой даты
\exception -1 Если дата не найдена
*/
qint64 TIndexes::checkIndex(QString targetDate){    //targetDate в формате "2020.04.15"
    QString str; qint64 pos;    //pos - значение индекса, str - строка файла
    targetDate.truncate(targetDate.length()-3); //Отсечение числа переданной даты
    QTextStream stream(&file);      //Подключение потока к файлу
    file.open(QIODevice::ReadOnly | QIODevice::Text);   //Открытие файла
    str = stream.readLine();        //Считывание первой строки
    QStringList list = str.split(QLatin1Char('&')); //Разделение строки "2020.10&45" на дату и значение индекса
    while(list[0]!=targetDate && !stream.atEnd()){  //Если строка не является искомой датой и файл не закончился
        str = stream.readLine();    //то считываем следующую строку и проверяем её
        list = str.split(QLatin1Char('&')); //Разделение строки на дату и значение индекса
    }
    if(stream.atEnd() && list[0]!=targetDate) pos = -1;    //Если файл закончился и искомая дата не встретилась, то вернём -1
    else pos = (qint64)list[1].toULongLong();        //Иначе считываем позицию в файле, когда начинается искомая запись
    file.close();                   //Закрытие файла
    return pos;                     //Возвращаем позицию записи искомой даты или -1
};
/*!
\param[in] date - дата создаваемого индекса в строчном формате "2020.04.15"
\param[in] value - значение создаваемого индекса
\exception Бездействие Если индекс уже существует
*/
void TIndexes::saveNew(QString date, qint64 value){   //date в формате "2020.04.15", pos - значение индекса
    if (checkIndex(date)==-1){          //Проверка, что такого индекса ещё не было
        date.truncate(date.length()-3); //Отсечение числа переданной даты
        QTextStream in(&file);          //Подключаем к файлу поток ввода in
        file.open(QIODevice::Append | QIODevice::Text); //Открытие файла для добавления
        in << date + "&" + QString::number(value) << endl;   //Запись в файл в формате "2020.10&45"
        file.close();                   //Закрытие файла
    }
};
/*!
\param[in] date - дата добавленной записи в строчном формате "2020.04.15"
\param[in] position - позиция последней записи в журнале до добавления
\param[in] length - длина добавленной записи
\param[in] flag - флаг добавления записи в конец журнала
*/
//Добавлена запись даты date длиной length. До добавления последняя запись начиналась с position
void TIndexes::updateIndexes(QString date, qint64 position, qint64 length, bool flag){ //flag: true - добавление в конец
    date.truncate(date.length()-3);     //Отсечение числа переданной даты.  flag: false - добавление в середину файла
    QString line;
    QFile newFile("newBook.txt");       //Создание файла для записи туда обновленной информации
    QTextStream out(&file);             //Подключение потоков к файлам
    QTextStream in(&newFile);
    file.open(QIODevice::ReadOnly | QIODevice::Text);   //Открытие файлов для передачи индексов из file в newFile
    newFile.open(QIODevice::WriteOnly | QIODevice::Text);
    while(!out.atEnd()){        //Пока в исходном файле есть строки
        line = out.readLine();  //Считываем очередную строку
        if(line[0]!='&' && line.split(QLatin1Char('&'))[0]<=date)   //Если строка имеет дату меньше, чем у новой записи
            in << line << endl;                                     //Индекс не меняется, просто переписываем в newFile
        else if(line[0]!='&' && line.split(QLatin1Char('&'))[0]>date){  //Если больше
            QStringList list = line.split(QLatin1Char('&'));    //то разделяем строку "2020.10&45" и увеличиваем
            in << list[0] + '&' + QString::number(list[1].toULongLong()+(unsigned long long)length+1) << endl;  //значение индекса на длину новой записи
        }else if(line[0]=='&' && flag){//Если строка не индекс, а позиция начала последней записи и новая запись не в конце файла
            in << '&' + QString::number(line.replace(0,1,' ').trimmed().toULongLong()+(unsigned long long)length+1) << endl;//Добавить длину запись
        }else if(line[0]=='&' && !flag){//Если строка - позиция начала последней записи и новая запись в конце файла
            in << '&' + QString::number(position) << endl;  //То теперь последняя запись начинается с position
        }
    }                                   //Замена файла file на newFile
    file.close();                       //Закрытие файлов
    newFile.close();
    QString oldname = file.fileName();  //Запоминаем название старого файла
    file.remove();                      //Удаляем старый файл
    newFile.rename(oldname);            //Переименовываем новый
};
/*!
\param[in] selDate - дата для преобразования
\return Дату в строковом формате "9 Декабря 2020 Ср"
*/
//Возвращает переданную дату в формате "9 Декабря 2020 Ср"
QString TDate::dateString(QDate selDate){
    QString month = selDate.toString("MMMM");  //month содержит текущий месяц
    QString s = selDate.toString("d MMMM");    //s содержит число и месяц
    s.truncate(s.length()-1);                               //Удаляем из s последний символ месяца
    if (month == "Март" || month == "Август") s = s + "та";  //В зависимости от текущего месяца добавляем к s
    else s = s + "я";                                       //символ 'а' или 'я' для родительного падежа
    return s + selDate.toString(" yyyy ddd");  //Прибавляем к s год и день недели
}
/*!
\param[in] selDate - дата для поиска даты понедельника
\return Дату понедельника на той же неделе, что и selDate
*/
QDate TDate::nearMonday(QDate selDate){
    while(selDate.dayOfWeek()!=1){
        selDate = selDate.addDays(-1);
    }
    return selDate;
}
QString TDate::weekString(QDate Monday){
    QString string = "";
    if(Monday.month() == Monday.addDays(7).month()){
        string = Monday.toString("MMMM yyyy");
    }
    else {
        string = Monday.toString("MMMM-") + Monday.addDays(7).toString("MMMM ") + Monday.toString("yyyy");
        if((Monday.year() != Monday.addDays(7).year()))
            string += Monday.addDays(7).toString("-yyyy");
    }
    return string;
}
/*!
Конструктор по умолчанию для объекта класса TDate. Инициализация полей датой создания объекта
*/
//Конструктор TDate для инициализации поля текущей даты
TDate::TDate(){
    currentDate = QDate::currentDate().toString("yyyy.MM.dd"); //Формирование строки из текущей даты
    selectedDate = currentDate;                     //Дневник по умолчанию открывается на текущей дате
};
/*!
Конструктор для объекта класса TRecord
\param[in] ntext - текст записи
\param[in] ndate - дата события, описываемого в записи
\param[in] nchangeDate - дата последнего изменения текста
*/
//Конструктор TRecord для инициализации полей одной записи в дневнике
TRecord::TRecord(QString ntext, QString ndate, QString nchangeDate)
{
    text = ntext;               //Инициализация текста записи
    date = ndate;               //Инициализация даты события
    changeDate = nchangeDate;   //Инициализация даты последнего изменения текста
}
/*!
\param[in] filename - имя файла журнала, принадлежащего пользователю
*/
//Установка связи журнала с определённым файлом, принадлежащем пользователю
void TJournal::initFile(QString filename){
    file.setFileName(filename);
};
/*!
\param[in] rec - запись для добавления в журнал
*/
//Добавить в журнал новую запись rec
void TJournal::saveNew(TRecord rec){
    QTextStream inf(&file);          //Подключаем к файлу поток ввода in
    file.open(QIODevice::Append | QIODevice::Text); //Открытие файла для добавления
    QTextStream book(&bookmarks.file);          //Подключаем к файлу поток ввода/вывода
    bookmarks.file.open(QIODevice::ReadWrite | QIODevice::Text); //Открытие файла
    qint64 position = inf.pos();    //position - позиция конца файла
    QString line;
    if(position != 0){              //Файл не пустой
        file.close();               //Смена режима работы с файлом
        file.open(QIODevice::ReadWrite | QIODevice::Text); //Открытие файла для добавления
        line = book.readLine();     //Чтение первой строки из файла из индексами
        while(line[0]!='&'){        //Поиск строки, в которой указана позиция последней записи в журнале
            line = book.readLine();
        }
        bookmarks.file.close();     //Закрыть файл с индексами
        line.replace(0,1,' ');      //В строке размещается позиция последней записи в журнале
        inf.seek((qint64)line.trimmed().toULongLong()); //Открываем в журнале последнюю запись
        line = inf.readLine();      //Считываем последнюю запись
        if('&' + rec.date < line){  //Если Дата добавляемой записи меньше последней
            QFile newFile("newDairy.txt");  //то создаём новый файл, в котором отсортированно запишем все записи
            QString targetDate = '&' + rec.date;    //targetDate - дата записи, которой нужно найти место
            QTextStream outf(&newFile);     //Поток для нового файла и его открытие для записи
            newFile.open(QIODevice::WriteOnly | QIODevice::Text);
            bool flag = true;           //flag: true - Новая запись ещё не вставлена в файл
            inf.seek(0);                //Встаём в начала исходного файла
            while(!inf.atEnd()){        //Пока исходный файл не закончен
                line = inf.readLine();  //Читаем очередную строку исходного журнала
                if (flag && line[0] == '&' && line > targetDate){//Если новая запись ещё не вставлена и мы нашли дату больше
                    bookmarks.updateIndexes(rec.date, position, (rec.date+rec.text+rec.changeDate).size() + 7, true);
                    position = outf.pos();    //то обновляем индексы при вставке записи, position - позиция новой записи
                    bookmarks.saveNew(rec.date, position);  //Создаём для новой записи новый индекс, если потребуется
                    outf << '&' + rec.date << endl;     //Записываем в новый журнал новую запись
                    outf << rec.text << endl;           //перед запиью с бОльшей датой
                    outf << rec.changeDate << endl;
                    flag = false;                       //Запись вставлена, поиск места больше не нужен
                }
                outf << line << endl;                   //Переписывание всех строк из старого журнала в новый
            }
            file.close();                               //Замена старого журнала file новым newFile
            newFile.close();                            //Закрытие файлов
            QString oldname = file.fileName();          //Запоминаем имя старого журнала
            file.remove();                              //Удаляем старый журнал
            newFile.rename(oldname);                    //Переименовываем новый журнал
        }else{                                          //Иначе, если дата новой записи больше даты самой последней записи
            bookmarks.saveNew(rec.date, position);      //То добавляем индекс для новой записи, передавая позицию конца журнала
            bookmarks.updateIndexes(rec.date, position, (rec.date+rec.text+rec.changeDate).size() + 7, false);
            inf.seek(position);             //Обновляем индексы и встаём на конец журнала
            inf << '&' + rec.date << endl;  //Запись в файл даты события,
            inf << rec.text << endl;        //текста записи
            inf << rec.changeDate << endl;  //и даты последнего его изменения
            file.close();                   //Закрытие файла
        }
    } else {                            //Иначе, если журнал пуст
        book << "&0" << endl;           //Записываем в файл индексов позицию начала последней(первой) записи
        bookmarks.file.close();         //Закрытие файла индексов
        bookmarks.saveNew(rec.date, 0); //Добавление первого индекса в позиции 0
        inf << '&' + rec.date << endl;  //Запись в файл даты события,
        inf << rec.text << endl;        //текста записи
        inf << rec.changeDate << endl;  //и даты последнего его изменения
        file.close();                   //Закрытие файла
    }
/* Вариант без использования индексов
    QTextStream in(&file);          //Подключаем к файлу поток ввода in
    file.open(QIODevice::Append | QIODevice::Text); //Открытие файла для добавления
    qint64 position = file.pos();
    in << '&' + rec.date << endl;   //Запись в файл даты события,
    in << rec.text << endl;         //текста записи
    in << rec.changeDate << endl;   //и даты последнего его изменения
    file.close();                   //Закрытие файла
*/
};
/*!
\param[in] targetDate - дата искомой записи в журнале
\return Позицию текста искомой даты в журнале
*/
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
/*!
\param[in] rec - запись с изменённым текстом
*/
//Перезаписать запись rec (просто изменить text и обновить поле changeDate - дата изменения text)
void TJournal::rewrite(TRecord rec){    //Передаётся изменённая запись rec
    QString line;
    QFile newFile("newDairy.txt");      //Создаём новый файл, в который перепишем обновлённый журнал
    QString targetDate = '&' + rec.date;//targetDate - дата записи, текст которой необходимо изменить
    QTextStream out(&file);             //Подключаем потоки к файлам
    QTextStream in(&newFile);
    file.open(QIODevice::ReadOnly | QIODevice::Text);       //Открываем файлы
    newFile.open(QIODevice::WriteOnly | QIODevice::Text);
    bool flag = true;       //flag = false - остановить запись в новый файл
    while(!out.atEnd()){    //Пока исходный файл ещё не закончен
        line = out.readLine();      //Считываем очередную строку из файла
        if (line == targetDate){    //Если строка - дата искомой записи
            in << line << endl;     //Переписываем такую же дату записи
            in << rec.text << endl; //но записываем новые text и changeDate
            in << rec.changeDate << endl;
            line = out.readLine();  //Пропускаем старую запись
            while(line[0]!='&' && !out.atEnd())//Ищем начало следующей записи
                line = out.readLine();
            if (out.atEnd())    //Если файл закончен, то мы изменяли последнюю запись
                flag = false;   //Записывать в файл больше нечего
        }
        if (flag)   //Если процесс записи не остановлен
            in << line << endl; //Записываем в newFile очередную строку
    }                                   //Замена файла file на newFile
    file.close();                       //Закрытие файлов
    newFile.close();
    QString oldname = file.fileName();  //Запоминаем название старого файла
    file.remove();                      //Удаляем старый файл
    newFile.rename(oldname);            //Переименовываем новый файл
};
/*!
\param[in] pos - позиция читаемой записи в журнале
\return Текст читаемой записи
*/
//Чтение из файла записи, имеющей позицию pos
QString TJournal::readText(qint64 pos){
    QString text = "";          //Переменная для текста читаемой записи
    QTextStream stream(&file);  //Подключение потока к файлу
    file.open(QIODevice::ReadOnly | QIODevice::Text);   //Открытие файла для чтения
    stream.seek(pos);           //Переход потока на позицию pos
    QString line = stream.readLine();   //Чтение строки из потока(файла)
    int count = 0;                      //Установка счётчика строк
    while(line[0]!='&' && !stream.atEnd()){ //Пока файл не кончился и строка не начинается с символа начала записи
        count++;                    //Прибавить к счётчику ещё одну строку читаемой записи
        line = stream.readLine();   //Считывание следующей
    }
    if (stream.atEnd())         //Если файл закончен
        count++;
    stream.seek(pos);           //Перемещение потока на начало записи
    for(int i=1;i<count;i++){   //Теперь мы знаем, что нужно считать count строк
        line = stream.readLine();   //Чтение строки
        text = text + line + '\n';  //и её добавление к тексту записи
    }
    file.close();   //Закрытие файла
    return text;    //Возврат текста записи
};
