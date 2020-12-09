#ifndef NOTES_H
#define NOTES_H

#include <QMainWindow>
#include <QtWidgets>
#include <vector>
/*!
    \file
    \brief Заголовочный файл с описанием классов для интерфейсной части заметок

    Файл содержит описание классов TNote, TGrid и Notes
*/
/*!
    \brief Класс заметки

    Объект класса представляет собой заметку
*/
class TNote{
public:
    TNote(QTextEdit *alabel, QTextEdit *atext, QPushButton *adelButton, QColor col1, QColor col2);
    QTextEdit *label;       ///< Название заметки(Заголовок)
    QTextEdit *text;        ///< Содержание заметки(Текст)
    QPushButton *delButton; ///< Кнопка удаления
    QColor colors[2];       ///< Цвета заметки
};
/*!
    \brief Класс сетки

    Объект класса представляет собой контейнер для хранения виджетов
*/
class TGrid{
public:
    QGridLayout *layout;
    void removeCell(QGridLayout *layout, int row, int column, bool deleteWidgets = true);
    void deleteChildWidgets(QLayoutItem *item);
    ~TGrid();
};

namespace Ui {
class Notes;
}
/*!
    \brief Заметки

    Объект класса представляет собой окно заметок, включающее в себя обработчики событий и файл для хранения заметок
*/
class Notes : public QMainWindow
{
    Q_OBJECT

public:
    explicit Notes(QWidget *parent = nullptr, QString username = "Corporate");
    ~Notes();
private:
    void initFile(QString name);
    void deleteNote(int index);

    QString username;           ///< Логин пользователя
    TGrid Grid;                 ///< Сетка заметок
    std::vector <TNote> notes;  ///< Все заметки
    std::vector <TNote> foundNotes; ///< Заметки после поиска
    QFile file;                 ///< Файл для хранения заметок
    bool searchFlag = false;    ///< Флаг использования поиска
    QColor Colors[2];           ///< Цвета для создания новой заметки
    Ui::Notes *ui;              ///< Указатель на графический интерфейс

private slots:
    void readNotes();

    void saveNotes();

    void deleteNote();

    void deleteAll();

    void on_createButton_clicked();

    void on_findButton_clicked();

    void on_showAllButton_clicked();

    void on_BackColor_clicked();
};

#endif // NOTES_H
