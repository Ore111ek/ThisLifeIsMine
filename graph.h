#ifndef GRAPH_H
#define GRAPH_H

#include <QString>
#include <vector>
#include "datewindow.h"
#include "qcustomplot.h"
/*!
    \file
    \brief Заголовочный файл с описанием классов для интерфейсной части дневника

    Файл содержит описание классов TDiarySettings и Diary
*/
/*!
    \brief Данные

    Объект класса представляет собой контейнер для хранения имени графика и его данных
*/
struct TStats{
    QString name;
    QStringList stats;
};
/*!
    \brief Граф

    Объект класса представляет собой граф с возможностью его отрисовки в интерфейсе
*/
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

    QColor colors[15];
    private:
     QCustomPlot *customPlot;
};


#endif // GRAPH_H
