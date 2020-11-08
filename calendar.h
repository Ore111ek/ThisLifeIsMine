#ifndef CALENDAR_H
#define CALENDAR_H

#include <QMainWindow>
#include "QtWidgets"

namespace Ui {
class Calendar;
}

class Calendar : public QMainWindow
{
    Q_OBJECT

public:
    explicit Calendar(QWidget *parent = nullptr);
    ~Calendar();

private slots:

private:
    Ui::Calendar *ui;
};

#endif // CALENDAR_H
