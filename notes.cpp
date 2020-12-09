#include "notes.h"
#include "ui_notes.h"
#include <QColorDialog>

  // Удаляет контент данной ячейки
void TGrid::removeCell(QGridLayout *layout, int row, int column, bool deleteWidgets) {
      for (int i = layout->count() - 1; i >= 0; i--) {
        int r, c, rs, cs;
        layout->getItemPosition(i, &r, &c, &rs, &cs);
        if ((r <= row && r + rs > row) &&
            (c <= column && c + cs > column)) {
          // Найдена ячейка для удаления
          QLayoutItem *item = layout->takeAt(i);
          if (deleteWidgets) {
            deleteChildWidgets(item);
          }
          delete item;
        }
      }
 }
  // Deletes all child widgets of the given layout item.
void TGrid::deleteChildWidgets(QLayoutItem *item) {
    QLayout *layout = item->layout();
    if (layout) {
      // Process all child items recursively.
      int itemCount = layout->count();
      for (int i = 0; i < itemCount; i++) {
        deleteChildWidgets(layout->itemAt(i));
      }
    }
    delete item->widget();
}

TNote::TNote(QTextEdit *alabel, QTextEdit *atext, QPushButton *adelButton, QColor col1 = QColor(255, 249, 57), QColor col2 = QColor(245, 255, 174)){
    label = alabel;
    text = atext;
    delButton = adelButton;
    colors[0] = col1;
    colors[1] = col2;
}

Notes::Notes(QWidget *parent, QString user) :
    QMainWindow(parent),
    ui(new Ui::Notes)
{
    username = user;
    ui->setupUi(this);
    initFile(username + "Notes.txt");
    Grid.layout = new QGridLayout;
    Grid.layout->setSpacing(0);
    Grid.layout->setSizeConstraint(QLayout::SetMinimumSize);
    QWidget *container = new QWidget;
    container->setLayout(Grid.layout);
    ui->scrollArea->setWidget(container);
    ui->scrollArea->setWidgetResizable(true);
    readNotes();
    Colors[0] = QColor(255, 249, 57);
    Colors[1] = QColor(245, 255, 174);
}

TGrid::~TGrid(){
    delete layout;
}

Notes::~Notes()
{
    saveNotes();
    delete ui;
}

void Notes::on_createButton_clicked()
{
    QString style;
    QVBoxLayout *note = new QVBoxLayout;
    QHBoxLayout *headlayout = new QHBoxLayout;
    QWidget *head = new QWidget;
    QWidget *noteWidget = new QWidget;
    QTextEdit *label = new QTextEdit;
    QPushButton *delButton = new QPushButton;
    QTextEdit *text = new QTextEdit;
    style = "QPushButton{background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgb(235, 235, 235), stop:1 rgb(249, 249, 249));";
    style += "padding: 3px; border: 1px solid black; border-bottom: 0px; border-left: 0px;}";
    delButton->setMaximumSize(30,30);
    delButton->setIcon(QIcon(":/icons/img/cancel_2.png"));
    delButton->setStyleSheet(style);
    delButton->setObjectName("delButton_" + QString::number(notes.size()));
    connect(delButton, SIGNAL(clicked()),
            this, SLOT(deleteNote()));
    style = "QTextEdit{background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgb(235, 235, 235), stop:1 rgb(249, 249, 249));";
    style += "padding: 3px 0px 3px 20px; border: 1px solid black; border-bottom: 0px; border-right: 0px;}";
    label->setStyleSheet(style);
    label->setMaximumHeight(30);
    label->verticalScrollBar()->hide();
    label->setObjectName("title_" + QString::number(notes.size()));
    //style = "QTextEdit{background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgb(255, 249, 57), stop:1 rgb(245, 255, 174));";
    style = "QTextEdit{background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 ";
    style += Colors[0].name() +  ", stop:1 " + Colors[1].name() + ");";
    style += "border: 1px solid black; border-top: 1px solid rgb(208, 208, 208);}";
    text->setStyleSheet(style);
    text->setMinimumSize(200,200);
    text->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    headlayout->setSizeConstraint(QLayout::SetMaximumSize);
    headlayout->setSpacing(0);
    headlayout->setMargin(0);
    headlayout->addWidget(label);
    headlayout->addWidget(delButton);
    head->setLayout(headlayout);
    //note->setSizeConstraint(QLayout::SetMinimumSize);
    note->setSizeConstraint(QLayout::SetFixedSize);
    note->setSpacing(0);
    note->addWidget(head);
    note->addWidget(text);
    noteWidget->setLayout(note);
    Grid.layout->addWidget(noteWidget, Grid.layout->count()/3, Grid.layout->count()%3);
    notes.push_back(TNote(label,text,delButton,Colors[0],Colors[1]));
}

void Notes::on_findButton_clicked()
{
    saveNotes();
    deleteAll();
    searchFlag = true;
    readNotes();
    Colors[0] = QColor(255, 249, 57);
    Colors[1] = QColor(245, 255, 174);
}

void Notes::readNotes(){
//    QString line;
//    QStringList widgets;
//    QTextStream out(&file);             //Подключаем потоки к файлам
//    file.open(QIODevice::ReadOnly | QIODevice::Text);       //Открываем файлы
//    while(!out.atEnd()){
//        line = out.readLine();
//        widgets = line.split(QString(" }"));
//        if(!searchFlag || widgets[0]==ui->findEdit->text()){
//            Colors[0] = widgets[2];
//            Colors[1] = widgets[3];
//            on_createButton_clicked();
//            notes[notes.size()-1].label->setText(widgets[0]);
//            notes[notes.size()-1].text->setText(widgets[1]);
//            notes[notes.size()-1].colors[0] = widgets[2];
//            notes[notes.size()-1].colors[1] = widgets[3];
//        }else{
//            QTextEdit *label = new QTextEdit;
//            QPushButton *delButton = new QPushButton;
//            QTextEdit *text = new QTextEdit;
//            notes.push_back(TNote(label,text,delButton));
//            notes[notes.size()-1].label->setText(widgets[0]);
//            notes[notes.size()-1].text->setText(widgets[1]);
//            notes[notes.size()-1].colors[0] = widgets[2];
//            notes[notes.size()-1].colors[1] = widgets[3];
//        }
//    }
//    file.close();
    QString line;
    QStringList widgets;
    QTextStream out(&file);             //Подключаем потоки к файлам
    file.open(QIODevice::ReadOnly | QIODevice::Text);       //Открываем файлы
    line = out.readAll();
    widgets = line.split(QString(" }"));
    for(int i = 0; i < widgets.length()/4; i++){
        if(!searchFlag || widgets[i*4]==ui->findEdit->text()){
            Colors[0] = widgets[i*4+2];
            Colors[1] = widgets[i*4+3];
            on_createButton_clicked();
            notes[notes.size()-1].label->setText(widgets[i*4]);
            notes[notes.size()-1].text->setText(widgets[i*4+1]);
            notes[notes.size()-1].colors[0] = widgets[i*4+2];
            notes[notes.size()-1].colors[1] = widgets[i*4+3];
        }else{
            QTextEdit *label = new QTextEdit;
            QPushButton *delButton = new QPushButton;
            QTextEdit *text = new QTextEdit;
            notes.push_back(TNote(label,text,delButton));
            notes[notes.size()-1].label->setText(widgets[i*4]);
            notes[notes.size()-1].text->setText(widgets[i*4+1]);
            notes[notes.size()-1].colors[0] = widgets[i*4+2];
            notes[notes.size()-1].colors[1] = widgets[i*4+3];
        }
    }

    file.close();
}

void Notes::deleteNote(){
        int index = sender()->objectName().split(QString("_"))[1].toInt();
        deleteNote(index);
        if(searchFlag){
            saveNotes();
            deleteAll();
            readNotes();
            Colors[0] = QColor(255, 249, 57);
            Colors[1] = QColor(245, 255, 174);
        }
}

void Notes::deleteNote(int index){
        for(unsigned long long i = (unsigned long long)index; i < notes.size()-1; i++){
            notes[i].label->setText(notes[i+1].label->toPlainText());
            notes[i].text->setText(notes[i+1].text->toPlainText());
            notes[i].colors[0] = notes[i+1].colors[0];
            notes[i].colors[1] = notes[i+1].colors[1];
        }
        if(Grid.layout->count() > 0)
            Grid.removeCell(Grid.layout, (Grid.layout->count()-1)/3, (Grid.layout->count()-1)%3);
        notes.pop_back();
    }


void Notes::deleteAll(){
    while(notes.size() > 0){
        if(Grid.layout->count() > 0)
            Grid.removeCell(Grid.layout, (Grid.layout->count()-1)/3, (Grid.layout->count()-1)%3);
        notes.pop_back();
    }
}

void Notes::saveNotes(){
//        QTextStream in(&file);             //Подключаем потоки к файлам
//        file.open(QIODevice::WriteOnly | QIODevice::Text);       //Открываем файлы
//        for(int i = 0; i < notes.size(); i++){
//            in << notes[i].label->toPlainText() + " }" + notes[i].text->toPlainText() + " }";
//            in << notes[i].colors[0].name() + " }" + notes[i].colors[1].name() + " }" << endl;
//        }
//        file.close();
    QTextStream in(&file);             //Подключаем потоки к файлам
    file.open(QIODevice::WriteOnly | QIODevice::Text);       //Открываем файлы
    for(unsigned long long i = 0; i < notes.size(); i++){
        in << notes[i].label->toPlainText() + " }" + notes[i].text->toPlainText() + " }";
        in << notes[i].colors[0].name() + " }" + notes[i].colors[1].name() + " }";
    }
    file.close();
}

void Notes::initFile(QString name){
    file.setFileName(name);
}

void Notes::on_showAllButton_clicked()
{
    searchFlag = false;
    saveNotes();
    deleteAll();
    readNotes();
    Colors[0] = QColor(255, 249, 57);
    Colors[1] = QColor(245, 255, 174);
}

void Notes::on_BackColor_clicked()
{
    Colors[0] = QColorDialog::getColor(Qt::white,this,"Выберите верхний цвет");//Вывод окна для выбора цвета
    Colors[1] = QColorDialog::getColor(Qt::white,this,"Выберите нижний цвет");//Вывод окна для выбора цвета
    if (Colors[0].isValid() && Colors[1].isValid()){                //Если текущий цвет выделения в настройках корректен
        on_createButton_clicked();
    }                                                                         //к тексту, выделенному на последней активной странице
    ui->BackColor->setStyleSheet("QPushButton {background-color: " + Colors[0].name() + "; border: 1px solid #555; width:25px; height:25px;}");
}
