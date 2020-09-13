#ifndef DAIRY_H
#define DAIRY_H

#include <QMainWindow>

namespace Ui {
class Dairy;
}

class Dairy : public QMainWindow
{
    Q_OBJECT

public:
    explicit Dairy(QWidget *parent = nullptr);
    ~Dairy();

private:
    Ui::Dairy *ui;
};

#endif // DAIRY_H
