#ifndef SETTINGS_H
#define SETTINGS_H

#include <QMainWindow>
/*!
    \file
    \brief Заголовочный файл с описанием классов для интерфейсной части настроек

    Файл содержит описание класса Settings
*/
namespace Ui {
class Settings;
}
/*!
    \brief Настройки

    Объект класса представляет собой окно для изменения настроек, изменения пароля
*/
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
