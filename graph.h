#ifndef GRAPH_H
#define GRAPH_H

#include <QString>
#include <vector>
#include "datewindow.h"
#include "journal.h"
#include "qcustomplot.h"

struct TStats{
    QString name;
    QStringList stats;
};

class TGraph{
public:
    TGraph(QCustomPlot *plot);
    void drawLinearGraph1(QDate date, std::vector<TStats> data);//
    //Если data.length == 1, то одна привычка, иначе несколько
    //Если data[i].stats.length > 31, то данные за каждый день года
    //если data[i].stats.length == 1, то передана сумма
    //если 1 < data[i].stats.length < 32, то переданы данные за каждый день месяца
    void drawLinearGraph2(QDate date, std::vector<TStats> data);
    void drawLinearGraph3(QDate date, std::vector<TStats> data);
    void drawBarsGraph1(QDate date, std::vector<TStats> data);
    void drawBarsGraph2(QDate date, std::vector<TStats> data);
    void blackTheme();
    void whiteTheme();
    QCustomPlot *customPlot;
    TJournal *records;
    QColor colors[15];
};


#endif // GRAPH_H
