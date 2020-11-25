#ifndef SETTINGS_H
#define SETTINGS_H

#include <QMainWindow>

namespace Ui {
class Settings;
}

class Settings : public QMainWindow
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = nullptr, QString name = "Corporate");
    ~Settings();

private slots:
    void on_changeButton_clicked();

private:
    Ui::Settings *ui;
    QString username;
};

#endif // SETTINGS_H
