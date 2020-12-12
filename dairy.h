#ifndef DAIRY_H
#define DAIRY_H

#include <QMainWindow>
#include <QFile>
#include <QTextStream>
#include <QTextEdit>
#include "datewindow.h"
#include "journal.h"

/*!
    \file
    \brief Заголовочный файл с описанием классов для интерфейсной части дневника

    Файл содержит описание классов TDiarySettings и Diary
*/
/*!
    \brief Класс настроек для дневника

    Объект класса представляет собой контейнер для хранения текущих интерфейсных настроек дневника
*/
class TDiarySettings{
public:
    QString correctHtml(QString Html, QString param, QString newValue);///< Изменение параметра в HTML строке
    QString findParamHtml(QString Html, QString param);///< Поиск значения параметра в HTML строке
    QFont mainFont;                 ///< Шрифт текста записи
    QColor pageColor;               ///< Сплошной цвет страницы
    QString gradientStyleDirect;    ///< Стиль CSS градиента для первой страницы
    QString gradientStyleReverse;   ///< Стиль CSS градиента для второй страницы
    QColor backgroundColor;         ///< Цвет выделения текста
    QColor textColor;               ///< Цвет текста записи
    QTextEdit *activeEdit = nullptr;
};

namespace Ui {
class Dairy;
}
/*!
    \brief Дневник

    Объект класса представляет собой дневник, включающий в себя обработчики событий,
    журнал для записей, файл с индексами и контейнер с текущими настройками интерфейса
*/
class Dairy : public QMainWindow
{
    Q_OBJECT

public:
    explicit Dairy(QWidget *parent = nullptr, QString name = "Corporate");  ///< Конструктор
    ~Dairy();                   ///< Деструктор для освобождения памяти
private:
    QString username;           ///< Логин пользователя
    TDate date;                 ///< Дата
    TJournal records;           ///< Журнал с записями
    TDiarySettings settings;    ///< Настройки интерфейса дневника
    bool rewritingFlag = false; ///< Флаг режима перезаписи
    bool changeFlag = false;    ///< Флаг изменения открытой записи
    DateWindow *Cal;            ///< Окно календаря для выбора даты
    Ui::Dairy *ui;  ///< Указатель на интерфейс дневника

private slots:
    void closeEvent(QCloseEvent *event);

    void on_saveButton_clicked();               ///< Сохранить запись в журнал

    void on_findButton_clicked();               ///< Открыть в дневнике выбранную дату

    void on_pushButton_clicked();               ///< Открыть календарь для выбора даты

    void on_fontSize_valueChanged(int arg1);    ///< Изменение размера шрифта текста

    void on_actionFont_triggered();             ///< Выбор пользователем шрифта текста

    void on_actionColor_triggered();            ///< Изменение цвета выделенного текста

    void on_actionBackground_triggered();       ///< Изменение цвета фона выделенного текста

    void on_actionPageGradient_triggered();     ///< Выбор градиента для страниц

    void on_actionColorPage_triggered();        ///< Выбор пользователем сплошного цвета страниц

    void on_actionChoiceTextColor_triggered();  ///< Выбор пользователем цвета текста

    void on_actionChoiceBackColor_triggered();  ///< Выбор пользователем цвета для выделения текста

    void on_actionSetFont_triggered();          ///< Изменение шрифта текста

    void on_actionSetPageColor_triggered();     ///< Установка цвета страниц

    void on_fontSelect_activated(const QString &arg1);  ///< Изменение шрифта текста при выборе в Select

    void on_textColorButton_clicked();

    void on_BackColorButton_clicked();

    void on_textEdit_cursorPositionChanged();

    void on_textEdit_2_cursorPositionChanged();

    void on_TextColor_clicked();

    void on_BackColor_clicked();

    void on_textEdit_textChanged();

    void on_textEdit_2_textChanged();

};

#endif // DAIRY_H
