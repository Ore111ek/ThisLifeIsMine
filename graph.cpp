#include "graph.h"

TGraph::TGraph(QCustomPlot *plot)
{
    customPlot = plot;
    colors[0] = QColor(214, 6, 6);colors[1] = QColor(42, 42, 199);colors[2] = QColor(35, 232, 35);
    colors[3] = QColor(225, 232, 35);colors[4] = QColor(209, 35, 232);colors[5] = QColor(240, 100, 0);
    colors[6] = QColor(77, 227, 232);colors[7] = QColor(230, 44, 195);colors[8] = QColor(11, 103, 153);
    colors[9] = QColor(26, 115, 37);colors[10] = QColor(107, 112, 33);colors[11] = QColor(107, 10, 10);
    colors[12] = QColor(186, 186, 186);colors[13] = QColor(255, 255, 255);colors[14] = QColor(0,0,0);
}


void TGraph::blackTheme(){
    customPlot->xAxis->setBasePen(QPen(Qt::white, 1));
    customPlot->yAxis->setBasePen(QPen(Qt::white, 1));
    customPlot->xAxis2->setBasePen(QPen(Qt::white, 1));
    customPlot->yAxis2->setBasePen(QPen(Qt::white, 1));
    customPlot->xAxis->setTickPen(QPen(Qt::white, 1));
    customPlot->yAxis->setTickPen(QPen(Qt::white, 1));
    customPlot->xAxis->setSubTickPen(QPen(Qt::white, 1));
    customPlot->yAxis->setSubTickPen(QPen(Qt::white, 1));
    customPlot->xAxis->setTickLabelColor(Qt::white);
    customPlot->yAxis->setTickLabelColor(Qt::white);
    customPlot->xAxis->setLabelColor(Qt::white);
    customPlot->yAxis->setLabelColor(Qt::white);
    customPlot->xAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    customPlot->yAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    customPlot->xAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
    customPlot->yAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
    customPlot->xAxis->grid()->setSubGridVisible(true);
    customPlot->yAxis->grid()->setSubGridVisible(true);
    customPlot->xAxis->grid()->setZeroLinePen(Qt::NoPen);
    customPlot->yAxis->grid()->setZeroLinePen(Qt::NoPen);
    customPlot->xAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
    customPlot->yAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
    QLinearGradient plotGradient;
    plotGradient.setStart(0, 0);
    plotGradient.setFinalStop(0, 350);
    plotGradient.setColorAt(0, QColor(80, 80, 80));
    plotGradient.setColorAt(1, QColor(50, 50, 50));
    customPlot->setBackground(plotGradient);
    QLinearGradient axisRectGradient;
    axisRectGradient.setStart(0, 0);
    axisRectGradient.setFinalStop(0, 350);
    axisRectGradient.setColorAt(0, QColor(80, 80, 80));
    axisRectGradient.setColorAt(1, QColor(30, 30, 30));
    customPlot->axisRect()->setBackground(axisRectGradient);
}
void TGraph::whiteTheme(){
    customPlot->xAxis->setBasePen(QPen(Qt::black, 1));
    customPlot->yAxis->setBasePen(QPen(Qt::black, 1));
    customPlot->xAxis2->setBasePen(QPen(Qt::black, 1));
    customPlot->yAxis2->setBasePen(QPen(Qt::black, 1));
    customPlot->xAxis->setTickPen(QPen(Qt::black, 1));
    customPlot->yAxis->setTickPen(QPen(Qt::black, 1));
    customPlot->xAxis->setSubTickPen(QPen(Qt::black, 1));
    customPlot->yAxis->setSubTickPen(QPen(Qt::black, 1));
    customPlot->xAxis->setTickLabelColor(Qt::black);
    customPlot->yAxis->setTickLabelColor(Qt::black);
    customPlot->xAxis->setLabelColor(Qt::black);
    customPlot->yAxis->setLabelColor(Qt::black);
    customPlot->xAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    customPlot->yAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    customPlot->xAxis->grid()->setSubGridPen(QPen(Qt::black, 1, Qt::DotLine));
    customPlot->yAxis->grid()->setSubGridPen(QPen(Qt::black, 1, Qt::DotLine));
    customPlot->xAxis->grid()->setSubGridVisible(true);
    customPlot->yAxis->grid()->setSubGridVisible(true);
    customPlot->xAxis->grid()->setZeroLinePen(Qt::NoPen);
    customPlot->yAxis->grid()->setZeroLinePen(Qt::NoPen);
    customPlot->xAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
    customPlot->yAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
    //customPlot->setBackground(QBrush(Qt::white));
    //customPlot->axisRect()->setBackground(QBrush(Qt::white));
    QLinearGradient plotGradient;
    plotGradient.setStart(0, 0);
    plotGradient.setFinalStop(0, 350);
    plotGradient.setColorAt(0, Qt::white);
    plotGradient.setColorAt(1, Qt::white);
    customPlot->setBackground(plotGradient);
    QLinearGradient axisRectGradient;
    axisRectGradient.setStart(0, 0);
    axisRectGradient.setFinalStop(0, 350);
    axisRectGradient.setColorAt(0, Qt::white);
    axisRectGradient.setColorAt(1, Qt::white);
    customPlot->axisRect()->setBackground(axisRectGradient);
}


void TGraph::drawLinearGraph1(QDate date, std::vector<TStats> data)
{
    customPlot->setLocale(QLocale(QLocale::Russian, QLocale::RussianFederation));
    int max = 0;
    if(data.size()==1 && data[0].stats.length()==12){
        customPlot->addGraph();
        QColor color = QColor(20, 110, 150, 150);

        customPlot->graph()->setLineStyle(QCPGraph::lsLine);
        customPlot->graph()->setPen(QPen(color.lighter(200)));
        customPlot->graph()->setBrush(QBrush(color));
        customPlot->graph()->setName(data[0].name);
        // generate random walk data:
        QSharedPointer<QCPAxisTickerText> textTickerX(new QCPAxisTickerText);
        QVector<QCPGraphData> timeData(12);
        for (int i=0; i<data[0].stats.length(); ++i)
        {
          date = QDate(date.year(),i+1,1);
          textTickerX->addTick(i+1, date.toString("MMMM\n" + data[0].stats[i]));
          timeData[i].key = i+1;
          timeData[i].value = data[0].stats[i].toInt();
          if(timeData[i].value > max)
              max = timeData[i].value;
        }
        customPlot->graph()->data()->set(timeData);
        customPlot->xAxis->setTicker(textTickerX);
      // configure left axis text labels:
      customPlot->xAxis->setLabel("Дата");
      customPlot->yAxis->setLabel("Сумма");
      customPlot->xAxis->setRange(0, 13);
    }
    else if(data[0].stats.length()==1){

          customPlot->addGraph();
          QColor color = QColor(20, 110, 150, 150);

          customPlot->graph()->setLineStyle(QCPGraph::lsLine);
          customPlot->graph()->setPen(QPen(color.lighter(200)));
          customPlot->graph()->setBrush(QBrush(color));
          customPlot->graph()->setName("Итоговая сумма");
          // generate random walk data:
          QSharedPointer<QCPAxisTickerText> textTickerX(new QCPAxisTickerText);
          QVector<QCPGraphData> timeData(data.size());
          for (int i=0; i<data.size(); ++i)
          {
            textTickerX->addTick(i+1, data[i].name + "\n" + data[i].stats[0]);
            timeData[i].key = i+1;
            timeData[i].value = data[i].stats[0].toInt();
            if(timeData[i].value > max)
                max = timeData[i].value;
          }
          customPlot->graph()->data()->set(timeData);
          customPlot->xAxis->setTicker(textTickerX);
        // configure left axis text labels:
        customPlot->xAxis->setLabel("Привычки");
        customPlot->yAxis->setLabel("Сумма");
        customPlot->xAxis->setRange(0, data.size()+1);

    }else{
        // set locale to english, so we get english month names:

        // seconds of current time, we'll use it as starting point in time for data:

        double now = QDateTime(date).toTime_t() + 3600*3;
        for (int gi = 0; gi < data.size(); ++gi)
        {
          customPlot->addGraph();
          QColor color = colors[gi%15];
          if(gi < 5)
            color = QColor(20+200/4.0*gi, 70*(1.6-gi/4.0), 150, 150);
          if(gi >=5 && gi < 9)
            color = QColor(20+200/4.0*(gi-5), 150, 70*(1.6-(gi-5)/4.0), 150);
          if(gi >= 9)
            color = QColor(150,70*(1.6-(gi-9)/4.0), 20+200/4.0*(gi-9), 150);
          if(gi >= 14){
            color = QColor(colors[gi%15]);
            color.setAlpha(150);
          }
          customPlot->graph()->setLineStyle(QCPGraph::lsLine);
          customPlot->graph()->setPen(QPen(color.lighter(200)));
          customPlot->graph()->setBrush(QBrush(color));
          customPlot->graph()->setName(data[gi].name);
          // generate random walk data:
          QVector<QCPGraphData> timeData(data[gi].stats.length());
          for (int i=0; i<data[gi].stats.length(); ++i)
          {
            timeData[i].key = now + 24*3600*i;
            timeData[i].value = data[gi].stats[i].toULongLong();
            if(timeData[i].value > max)
                max = timeData[i].value;
          }
          customPlot->graph()->data()->set(timeData);
        }
        // configure bottom axis to show date instead of number:
        QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);
        dateTicker->setDateTimeFormat("d MMMM");
        customPlot->xAxis->setTicker(dateTicker);
        // configure left axis text labels:

        customPlot->xAxis->setLabel("Дата");
        customPlot->yAxis->setLabel("Результаты");

        customPlot->xAxis->setRange(now, now+24*3600*(data[0].stats.length()-1));
    }


    QSharedPointer<QCPAxisTickerText> textTickerY(new QCPAxisTickerText);
    //textTicker->addTick(20, "Хорошее\nначало");
    //textTicker->addTick(100, "Отличный\nрезультат");
    textTickerY->addTick(max/10, QString::number(max/10));
    textTickerY->addTick(max/5, QString::number(max/5));
    textTickerY->addTick(max/2, QString::number(max/2));
    textTickerY->addTick(max, QString::number(max));

    customPlot->yAxis->setTicker(textTickerY);
    // set a more compact font size for bottom and left axis tick labels:
    customPlot->xAxis->setTickLabelFont(QFont(QFont().family(), 8));
    customPlot->yAxis->setTickLabelFont(QFont(QFont().family(), 8));
    // set axis labels:
    customPlot->xAxis->setTickLabels(true);
    customPlot->yAxis->setTickLabels(true);
    // make top and right axes visible but without ticks and labels:
    customPlot->xAxis2->setVisible(true);
    customPlot->yAxis2->setVisible(true);
    customPlot->xAxis2->setTicks(false);
    customPlot->yAxis2->setTicks(false);
    customPlot->xAxis2->setTickLabels(false);
    customPlot->yAxis2->setTickLabels(false);
    // set axis ranges to show all data:
    customPlot->yAxis->setRange(0, max + max/10);
    // show legend with slightly transparent background brush:
    customPlot->legend->setVisible(true);
    customPlot->legend->setBrush(QColor(255, 255, 255, 150));
}

void TGraph::drawLinearGraph2(QDate date, std::vector<TStats> data)
{
    QPen pen;
    customPlot->setLocale(QLocale(QLocale::Russian, QLocale::RussianFederation));
    int max = 0;
    if(data.size()==1 && data[0].stats.length()==12){
        customPlot->addGraph();
        pen.setColor(QColor(qSin(1.2)*80+80, qSin(+0)*80+80, qSin(1.5)*80+80));
        customPlot->graph()->setPen(pen);
        customPlot->graph()->setLineStyle(QCPGraph::lsLine);
        customPlot->graph()->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 5));
        // generate data:
        customPlot->graph()->setName(data[0].name);
        // generate random walk data:
        QSharedPointer<QCPAxisTickerText> textTickerX(new QCPAxisTickerText);
        QVector<QCPGraphData> timeData(12);
        for (int i=0; i<data[0].stats.length(); ++i)
        {
          date = QDate(date.year(),i+1,1);
          textTickerX->addTick(i+1, date.toString("MMMM\n" + data[0].stats[i]));
          timeData[i].key = i+1;
          timeData[i].value = data[0].stats[i].toInt();
          if(timeData[i].value > max)
              max = timeData[i].value;
        }
        customPlot->graph()->data()->set(timeData);
        customPlot->xAxis->setTicker(textTickerX);
      // configure left axis text labels:
      customPlot->xAxis->setLabel("Дата");
      customPlot->yAxis->setLabel("Сумма");
      customPlot->xAxis->setRange(0, 13);
    }
    else if(data[0].stats.length()==1){
        double now = QDateTime(date).toTime_t() + 3600*3;
        customPlot->addGraph();
        pen.setColor(QColor(qSin(1.2)*80+80, qSin(+0)*80+80, qSin(1.5)*80+80));
        customPlot->graph()->setPen(pen);
        customPlot->graph()->setName("Итоговая сумма");

        customPlot->graph()->setLineStyle(QCPGraph::lsLine);

        customPlot->graph()->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 5));
        // generate data:
        QSharedPointer<QCPAxisTickerText> textTickerX(new QCPAxisTickerText);
        QVector<QCPGraphData> timeData(data.size());
        for (int i=0; i<data.size(); ++i)
        {
          textTickerX->addTick(i+1, data[i].name + "\n" + data[i].stats[0]);
          timeData[i].key = i+1;
          timeData[i].value = data[i].stats[0].toInt();
          if(timeData[i].value > max)
              max = timeData[i].value;
        }
        customPlot->graph()->data()->set(timeData);
        customPlot->xAxis->setTicker(textTickerX);
      customPlot->xAxis->setLabel("Привычки");
      customPlot->yAxis->setLabel("Результаты");
      customPlot->xAxis->setRange(0, data.size()+1);

    }else{
        double now = QDateTime(date).toTime_t() + 3600*3;
        for (int gi = 0; gi < data.size(); ++gi)
        {
          customPlot->addGraph();
          pen.setColor(QColor(qSin(gi*1+1.2)*80+80, qSin(gi*0.3+0)*80+80, qSin(gi*0.3+1.5)*80+80));
          customPlot->graph()->setPen(pen);
          customPlot->graph()->setName(data[gi].name);

          customPlot->graph()->setLineStyle(QCPGraph::lsLine);
         QCPScatterStyle::ScatterShape i = QCPScatterStyle::ssCircle;
         i = QCPScatterStyle::ScatterShape(i + gi);
          customPlot->graph()->setScatterStyle(QCPScatterStyle(i, 5));
          // generate data:
          QVector<QCPGraphData> timeData(data[gi].stats.length());
          for (int i=0; i<data[gi].stats.length(); ++i)
          {
            timeData[i].key = now + 24*3600*i;
            timeData[i].value = data[gi].stats[i].toULongLong();
            if(timeData[i].value > max)
                max = timeData[i].value;
          }
          customPlot->graph()->data()->set(timeData);
          }
        QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);
        dateTicker->setDateTimeFormat("d MMMM");
        customPlot->xAxis->setTicker(dateTicker);
             // configure left axis text labels:

        customPlot->xAxis->setLabel("Дата");
        customPlot->yAxis->setLabel("Результаты");
        customPlot->xAxis->setRange(now, now+24*3600*(data[0].stats.length()-1));
    }

    customPlot->legend->setVisible(true);
    customPlot->legend->setFont(QFont("Helvetica", 9));
    QSharedPointer<QCPAxisTickerText> textTickerY(new QCPAxisTickerText);
    textTickerY->addTick(max/10, QString::number(max/10));
    textTickerY->addTick(max/5, QString::number(max/5));
    textTickerY->addTick(max/2, QString::number(max/2));
    textTickerY->addTick(max, QString::number(max));
    customPlot->yAxis->setTicker(textTickerY);
    customPlot->yAxis->setRange(0, max + max/10);
    // set blank axis lines:
    customPlot->xAxis->setTicks(true);
    customPlot->yAxis->setTicks(true);
    customPlot->xAxis->setTickLabels(true);
    customPlot->yAxis->setTickLabels(true);
    // make top right axes clones of bottom left axes:
    customPlot->axisRect()->setupFullAxesBox();
}

void TGraph::drawLinearGraph3(QDate date, std::vector<TStats> data)
{

    customPlot->setLocale(QLocale(QLocale::Russian, QLocale::RussianFederation));
    int max = 0;
    // seconds of current time, we'll use it as starting point in time for data:


    if(data.size()==1 && data[0].stats.length()==12){
        customPlot->addGraph();
        // generate data:
        customPlot->graph()->setLineStyle(QCPGraph::lsLine);
        customPlot->graph()->setPen(QPen(QColor(120, 120, 120), 2));
        customPlot->graph()->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, QPen(Qt::black, 1.5), QBrush(Qt::white), 9));
        customPlot->graph()->setName(data[0].name);
        // generate random walk data:
        QSharedPointer<QCPAxisTickerText> textTickerX(new QCPAxisTickerText);
        QVector<QCPGraphData> timeData(12);
        for (int i=0; i<data[0].stats.length(); ++i)
        {
          date = QDate(date.year(),i+1,1);
          textTickerX->addTick(i+1, date.toString("MMMM\n" + data[0].stats[i]));
          timeData[i].key = i+1;
          timeData[i].value = data[0].stats[i].toInt();
          if(timeData[i].value > max)
              max = timeData[i].value;
        }
        customPlot->graph()->data()->set(timeData);
        customPlot->xAxis->setTicker(textTickerX);
      // configure left axis text labels:
      customPlot->xAxis->setLabel("Дата");
      customPlot->yAxis->setLabel("Сумма");
      customPlot->xAxis->setRange(0, 13);
    }
    else if(data[0].stats.length()==1){
        double now = QDateTime(date).toTime_t() + 3600*3;
        customPlot->addGraph();
        customPlot->graph()->setLineStyle(QCPGraph::lsLine);
        customPlot->graph()->setPen(QPen(QColor(120, 120, 120), 2));
        customPlot->graph()->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, QPen(Qt::black, 1.5), QBrush(Qt::white), 9));
        customPlot->graph()->setName("Итоговая сумма");
        // generate data:
        QSharedPointer<QCPAxisTickerText> textTickerX(new QCPAxisTickerText);
        QVector<QCPGraphData> timeData(data.size());
        for (int i=0; i<data.size(); ++i)
        {
          textTickerX->addTick(i+1, data[i].name + "\n" + data[i].stats[0]);
          timeData[i].key = i+1;
          timeData[i].value = data[i].stats[0].toInt();
          if(timeData[i].value > max)
              max = timeData[i].value;
        }
        customPlot->graph()->data()->set(timeData);
        customPlot->xAxis->setTicker(textTickerX);
      customPlot->xAxis->setLabel("Привычки");
      customPlot->yAxis->setLabel("Результаты");
      customPlot->xAxis->setRange(0, data.size()+1);

    }else{
    double now = QDateTime(date).toTime_t() + 3600*3;
    for (int gi = 0; gi < data.size(); ++gi)
    {
      customPlot->addGraph();
      //QColor color(20+200/4.0*gi,70*(1.6-gi/4.0), 150, 150);

      customPlot->graph()->setLineStyle(QCPGraph::lsLine);
      customPlot->graph()->setPen(QPen(QColor(120, 120, 120), 2));
     // QColor color;
       // if(gi<=5)
       //     color = QColor(50*gi,125*gi*gi/3 - 625*gi/3 + 250,250 - 50*gi, 250);
       // if(gi>5)
        //    color = QColor(40*(gi-6),250 - 50*(gi-6), 125*(gi-6)*(gi-6)/3 - 625*(gi-6)/3 + 250, 250);
      customPlot->graph()->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, QPen(Qt::black, 1.5), QBrush(colors[gi%15]), 9));
      //customPlot->graph()->setBrush(QBrush(color));
      customPlot->graph()->setName(data[gi].name);
      // generate random walk data:
      QVector<QCPGraphData> timeData(data[gi].stats.length());
      for (int i=0; i<data[gi].stats.length(); ++i)
      {
        timeData[i].key = now + 24*3600*i;
        timeData[i].value = data[gi].stats[i].toULongLong();
        if(timeData[i].value > max)
            max = timeData[i].value;
      }
      customPlot->graph()->data()->set(timeData);

    }
    // configure bottom axis to show date instead of number:

    QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);
    dateTicker->setDateTimeFormat("d MMMM");
    customPlot->xAxis->setTicker(dateTicker);
    customPlot->xAxis->setLabel("Дата");
    customPlot->yAxis->setLabel("Результаты");
    customPlot->xAxis->setRange(now, now+24*3600*(data[0].stats.length()-1));
    }

    // configure left axis text labels:
    QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
    textTicker->addTick(max/10, QString::number(max/10));
    textTicker->addTick(max/5, QString::number(max/5));
    textTicker->addTick(max/2, QString::number(max/2));
    textTicker->addTick(max, QString::number(max));

    customPlot->yAxis->setTicker(textTicker);

    customPlot->xAxis->setTickLabels(true);
    customPlot->yAxis->setTickLabels(true);
    customPlot->xAxis2->setVisible(true);
    customPlot->yAxis2->setVisible(true);
    customPlot->xAxis2->setTicks(false);
    customPlot->yAxis2->setTicks(false);
    customPlot->xAxis2->setTickLabels(false);
    customPlot->yAxis2->setTickLabels(false);
    // set axis ranges to show all data:

    customPlot->yAxis->setRange(0, max + max/10);
    // show legend with slightly transparent background brush:
    customPlot->legend->setVisible(true);

//    customPlot->rescaleAxes();
//    customPlot->yAxis->setRange(0, 2);
}


void TGraph::drawBarsGraph1(QDate date, std::vector<TStats> data)
{
    // set dark background gradient:
//    QLinearGradient gradient(0, 0, 0, 400);
//    gradient.setColorAt(0, QColor(90, 90, 90));
//    gradient.setColorAt(0.38, QColor(105, 105, 105));
//    gradient.setColorAt(1, QColor(70, 70, 70));
//    customPlot->setBackground(QBrush(gradient));

    customPlot->setLocale(QLocale(QLocale::Russian, QLocale::RussianFederation));
    QVector<double> ticks;
    QVector<QString> labels;
    int max = 0;
    // seconds of current time, we'll use it as starting point in time for data:


    if(data.size()==1 && data[0].stats.length()==12){




        for(int i = 0; i < data[0].stats.length(); i++){
            date = QDate(date.year(),i+1,1);
            ticks <<i+1;
            labels << date.toString("MMMM\n" + data[0].stats[i]);
        }
        QSharedPointer<QCPAxisTickerText> textTickerX(new QCPAxisTickerText);
        textTickerX->addTicks(ticks, labels);
        customPlot->xAxis->setTicker(textTickerX);
        QCPBars *bars = new QCPBars(customPlot->xAxis, customPlot->yAxis);
        bars->setAntialiased(false); // gives more crisp, pixel aligned bar borders
        bars->setName(data[0].name);
        bars->setPen(QPen(QColor(250, 170, 20).lighter(150)));
        bars->setBrush(QColor(250, 170, 20));
        // stack bars on top of each other:
            int maxlocal = 0;
          // generate random walk data:
            QVector<double> barsData;
          for (int i=0; i<data[0].stats.length(); ++i)
          {
              barsData  << data[0].stats[i].toInt();
              if(data[0].stats[i].toInt() > maxlocal)
                maxlocal = data[0].stats[i].toInt();
          }
          bars->setData(ticks, barsData);

          bars->setWidth(0.95);
          max = maxlocal;

        customPlot->xAxis->setLabel("Привычки");
        customPlot->yAxis->setLabel("Сумма");
        customPlot->xAxis->setRange(0, data.size()+1);



        QVector<QCPGraphData> timeData(12);
        for (int i=0; i<data[0].stats.length(); ++i)
        {
          date = QDate(date.year(),i+1,1);
          textTickerX->addTick(i+1, date.toString("MMMM\n" + data[0].stats[i]));
          timeData[i].key = i+1;
          timeData[i].value = data[0].stats[i].toInt();
          if(timeData[i].value > max)
              max = timeData[i].value;
        }
      // configure left axis text labels:
      customPlot->xAxis->setLabel("Дата");
      customPlot->yAxis->setLabel("Сумма");
      customPlot->xAxis->setRange(0, 13);
      customPlot->yAxis->setRange(0, max + max/10);
    }
    else if(data[0].stats.length()==1){

        for(int i = 0; i < data.size(); i++){
            ticks <<i+1;
            labels << data[i].name + "\n" + data[i].stats[0];
        }
        QSharedPointer<QCPAxisTickerText> textTickerX(new QCPAxisTickerText);
        textTickerX->addTicks(ticks, labels);
        customPlot->xAxis->setTicker(textTickerX);
        QCPBars *bars = new QCPBars(customPlot->xAxis, customPlot->yAxis);
        bars->setAntialiased(false); // gives more crisp, pixel aligned bar borders
        bars->setName("Итоговая сумма");
        bars->setPen(QPen(QColor(250, 170, 20).lighter(150)));
        bars->setBrush(QColor(250, 170, 20));
        // stack bars on top of each other:
            int maxlocal = 0;
          // generate random walk data:
            QVector<double> barsData;
          for (int i=0; i<data.size(); ++i)
          {
              barsData  << data[i].stats[0].toInt();
              if(data[i].stats[0].toInt() > maxlocal)
                maxlocal = data[i].stats[0].toInt();
          }
          bars->setData(ticks, barsData);

          bars->setWidth(0.95);
          max = maxlocal;

        customPlot->xAxis->setLabel("Привычки");
        customPlot->yAxis->setLabel("Сумма");
        customPlot->xAxis->setRange(0, data.size()+1);
        customPlot->yAxis->setRange(0, max + max/10);
    }else{
        QCPBars *prevbars = nullptr;
        double now = QDateTime(date).toTime_t() + 3600*3;

        for(int i = 0; i < data[0].stats.length(); i++){
            ticks << now+24*3600*(i);
            labels << QString::number(i+1);
        }
        QSharedPointer<QCPAxisTickerText> textTickerX(new QCPAxisTickerText);
        textTickerX->addTicks(ticks, labels);
        customPlot->xAxis->setTicker(textTickerX);


        for (int gi = 0; gi < data.size(); ++gi)
        {
            QCPBars *bars = new QCPBars(customPlot->xAxis, customPlot->yAxis);
            bars->setAntialiased(false); // gives more crisp, pixel aligned bar borders
            bars->setStackingGap(1);
            bars->setName(data[gi].name);
            bars->setPen(QPen(colors[gi%15].lighter(130)));
            bars->setBrush(colors[gi%15].darker(100));
            // stack bars on top of each other:
            if(prevbars != nullptr)
              bars->moveAbove(prevbars);
            prevbars = bars;

            int maxlocal = 0;
          // generate random walk data:
            QVector<double> barsData;
          for (int i=0; i<data[gi].stats.length(); ++i)
          {
              barsData  << data[gi].stats[i].toInt();
              if(data[gi].stats[i].toInt() > maxlocal)
                maxlocal = data[gi].stats[i].toInt();
          }
          bars->setData(ticks, barsData);

          bars->setWidth(24*3600*(data[0].stats.length()-1)/data[0].stats.length()*9/10);
          max += maxlocal;
        }
        QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);
        dateTicker->setDateTimeFormat("d MMMM");
        customPlot->xAxis->setTicker(dateTicker);
        // configure left axis text labels:
        customPlot->xAxis->setLabel("Дата");
        customPlot->yAxis->setLabel("Результаты");
        customPlot->xAxis->setRange(now - 24*3600, now+24*3600*(data[0].stats.length()));
        customPlot->yAxis->setRange(0, max);
    }

    QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
    textTicker->addTick(max/10, QString::number(max/10));
    textTicker->addTick(max/5, QString::number(max/5));
    textTicker->addTick(max/2, QString::number(max/2));
    textTicker->addTick(max, QString::number(max));

    customPlot->yAxis->setTicker(textTicker);

    customPlot->xAxis->setTickLabels(true);
    customPlot->yAxis->setTickLabels(true);
    // make top and right axes visible but without ticks and labels:
    customPlot->xAxis2->setVisible(true);
    customPlot->yAxis2->setVisible(true);
    customPlot->xAxis2->setTicks(false);
    customPlot->yAxis2->setTicks(false);
    customPlot->xAxis2->setTickLabels(false);
    customPlot->yAxis2->setTickLabels(false);
    // set axis ranges to show all data:
    // show legend with slightly transparent background brush:
    customPlot->legend->setVisible(true);
    customPlot->xAxis->setTickLabelFont(QFont(QFont().family(), 8));
    customPlot->yAxis->setTickLabelFont(QFont(QFont().family(), 8));
}


void TGraph::drawBarsGraph2(QDate date, std::vector<TStats> data)
{
    customPlot->setLocale(QLocale(QLocale::Russian, QLocale::RussianFederation));
    int max = 0;

    if(data.size()==1 && data[0].stats.length()==12){

            int maxlocal = 0;
          // generate random walk data:
          QVector<double> x(data[0].stats.length()), y(data[0].stats.length());
          for (int i=0; i<x.size(); ++i)
          {
            x[i] = i + 1;
            y[i] = data[0].stats[i].toInt();
            if(y[i] > maxlocal)
                maxlocal = y[i];
          }
          max = maxlocal;
          QCPBars *bars = new QCPBars(customPlot->xAxis, customPlot->yAxis);
          bars->setWidth(0.95);
          bars->setData(x, y);
          bars->setPen(Qt::NoPen);
          bars->setBrush(QColor(10, 140, 70, 160));
          bars->setName(data[0].name);



        QSharedPointer<QCPAxisTickerText> textTickerX(new QCPAxisTickerText);
        for (int i=0; i<data[0].stats.length(); ++i)
        {
          date = QDate(date.year(),i+1,1);
          textTickerX->addTick(i+1, date.toString("MMMM\n" + data[0].stats[i]));
        }
        customPlot->xAxis->setTicker(textTickerX);
      // configure left axis text labels:
      customPlot->xAxis->setLabel("Дата");
      customPlot->yAxis->setLabel("Сумма");
      customPlot->xAxis->setRange(0, 13);
      customPlot->yAxis->setRange(0, max + max/10);
    }
    else if(data[0].stats.length()==1){
            int maxlocal = 0;
          // generate random walk data:
          QVector<double> x(data.size()), y(data.size());
          for (int i=0; i<x.size(); ++i)
          {
            x[i] = i + 1;
            y[i] = data[i].stats[0].toInt();
            if(y[i] > maxlocal)
                maxlocal = y[i];
          }
          max = maxlocal;
          QCPBars *bars = new QCPBars(customPlot->xAxis, customPlot->yAxis);
          bars->setWidth(0.95);
          bars->setData(x, y);
          bars->setPen(Qt::NoPen);
          bars->setBrush(QColor(10, 140, 70, 160));
          bars->setName("Итоговая сумма");
          QSharedPointer<QCPAxisTickerText> textTickerX(new QCPAxisTickerText);
          for (int i=0; i<data.size(); ++i)
          {
            textTickerX->addTick(i+1, data[i].name + "\n" + data[i].stats[0]);
          }
          customPlot->xAxis->setTicker(textTickerX);
        customPlot->xAxis->setLabel("Привычки");
        customPlot->yAxis->setLabel("Сумма");
        customPlot->xAxis->setRange(0, data.size()+1);
        customPlot->yAxis->setRange(0, max + max/10);
    }else{
        QCPBars *prevbars = nullptr;
        // seconds of current time, we'll use it as starting point in time for data:
        double now = QDateTime(date).toTime_t() + 3600*3;
        for (int gi = 0; gi < data.size(); ++gi)
        {
            int maxlocal = 0;
          // generate random walk data:
          QVector<double> x(data[gi].stats.length()), y(data[gi].stats.length());
          for (int i=0; i<x.size(); ++i)
          {
            x[i] = now + 24*3600*i;
            y[i] = data[gi].stats[i].toInt();
            if(y[i] > maxlocal)
                maxlocal = y[i];
          }
          max += maxlocal;
          QCPBars *bars = new QCPBars(customPlot->xAxis, customPlot->yAxis);
          bars->setWidth(24*3600*(data[0].stats.length()-1)/(double)x.size());
          bars->setData(x, y);
          bars->setPen(Qt::NoPen);
          QColor color = colors[gi%15];
          color.setAlpha(220 - gi*5);
          bars->setBrush(color);
          bars->setName(data[gi].name);
          if(prevbars != nullptr)
            bars->moveAbove(prevbars);
          prevbars = bars;
        }

        // configure bottom axis to show date instead of number:

        QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);
        dateTicker->setDateTimeFormat("d MMMM");
        customPlot->xAxis->setTicker(dateTicker);
        customPlot->xAxis->setLabel("Дата");
        customPlot->yAxis->setLabel("Результаты");
        customPlot->xAxis->setRange(now - 24*3600, now+24*3600*(data[0].stats.length()));
        customPlot->yAxis->setRange(0, max);
    }




    // configure left axis text labels:
    QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
    textTicker->addTick(max/10, QString::number(max/10));
    textTicker->addTick(max/5, QString::number(max/5));
    textTicker->addTick(max/2, QString::number(max/2));
    textTicker->addTick(max, QString::number(max));

    customPlot->yAxis->setTicker(textTicker);

    customPlot->xAxis->setTickLabels(true);
    customPlot->yAxis->setTickLabels(true);
    customPlot->xAxis2->setVisible(true);
    customPlot->yAxis2->setVisible(true);
    customPlot->xAxis2->setTicks(false);
    customPlot->yAxis2->setTicks(false);
    customPlot->xAxis2->setTickLabels(false);
    customPlot->yAxis2->setTickLabels(false);
    // set axis ranges to show all data:

    // show legend with slightly transparent background brush:
    customPlot->legend->setVisible(true);


    //customPlot->rescaleAxes();


}

