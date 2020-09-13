#ifndef HOBBY_H
#define HOBBY_H

#include <QMainWindow>

namespace Ui {
class Hobby;
}

class Hobby : public QMainWindow
{
    Q_OBJECT

public:
    explicit Hobby(QWidget *parent = nullptr);
    ~Hobby();

private:
    Ui::Hobby *ui;
};

#endif // HOBBY_H
