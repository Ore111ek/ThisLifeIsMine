#ifndef NOTES_H
#define NOTES_H

#include <QMainWindow>
#include <QtWidgets>
#include <vector>
#include "journal.h"

class TNote{
public:
    TNote(QTextEdit *alabel, QTextEdit *atext, QPushButton *adelButton, QColor col1, QColor col2);
    QTextEdit *label;
    QTextEdit *text;
    QPushButton *delButton;
    QColor colors[2];
};

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

class Notes : public QMainWindow
{
    Q_OBJECT

public:
    explicit Notes(QWidget *parent = nullptr, QString username = "Corporate");
    QString username;           ///< Логин пользователя
    TGrid Grid;
    std::vector <TNote> notes;
    std::vector <TNote> foundNotes;
    QFile file;
    bool searchFlag = false;
    QColor Colors[2];
   // bool updateFlag = false;
    void initFile(QString name);
    void deleteNote(int index);
    ~Notes();
private slots:
    void readNotes();

    void saveNotes();

    void deleteNote();

    void deleteAll();

    void on_createButton_clicked();

    void on_findButton_clicked();

    void on_showAllButton_clicked();

    void on_BackColor_clicked();

private:
    Ui::Notes *ui;
};

#endif // NOTES_H
