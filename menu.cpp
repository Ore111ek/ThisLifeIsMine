#include "menu.h"
#include "ui_menu.h"

Menu::Menu(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Menu)
{
    ui->setupUi(this);
    win_a.show();
}

Menu::~Menu()
{
    delete ui;
}

void Menu::on_action_triggered()
{
    win_c.show();
}

void Menu::on_action_2_triggered()
{
    win_d.show();
}

void Menu::on_action_3_triggered()
{
    win_t.show();
}

void Menu::on_action_4_triggered()
{
    win_n.show();
}

void Menu::on_action_5_triggered()
{
    win_h.show();
}

void Menu::on_action_6_triggered()
{
    win_b.show();
}

void Menu::on_action_7_triggered()
{
    win_p.show();
}
