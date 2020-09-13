#ifndef TIMETABLE_H
#define TIMETABLE_H

#include <QMainWindow>

namespace Ui {
class Timetable;
}

class Timetable : public QMainWindow
{
    Q_OBJECT

public:
    explicit Timetable(QWidget *parent = nullptr);
    ~Timetable();

private:
    Ui::Timetable *ui;
};

#endif // TIMETABLE_H
