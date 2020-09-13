#ifndef BUDGET_H
#define BUDGET_H

#include <QMainWindow>

namespace Ui {
class Budget;
}

class Budget : public QMainWindow
{
    Q_OBJECT

public:
    explicit Budget(QWidget *parent = nullptr);
    ~Budget();

private:
    Ui::Budget *ui;
};

#endif // BUDGET_H
