#include "graphic.h"

Graphic::Graphic(QCustomPlot* customPlot)
{
   pArrival = new QCPBars(customPlot->xAxis, customPlot->yAxis);
   //pDeparture = new QCPBars(customPlot->xAxis, customPlot->yAxis);

   pArrival->setAntialiased(false);  // gives more crisp, pixel aligned bar borders
   //pDeparture->setAntialiased(false);
   pArrival->setStackingGap(1);
   //pDeparture->setStackingGap(1);

   // set names and colors:
   pArrival->setName("Прилеты");
   pArrival->setPen(QPen(QColor(0, 168, 140).lighter(130)));
   pArrival->setBrush(QColor(0, 168, 140));
//   pDeparture->setName("Вылеты");
//   pDeparture->setPen(QPen(QColor(250, 170, 20).lighter(150)));
//   pDeparture->setBrush(QColor(250, 170, 20));

   // stack bars on top of each other:
   //pArrival->moveAbove(pDeparture);

   // prepare x axis with country labels:
   QVector<double> months;
   QVector<QString> labels;
   months << 1 << 2 << 3 << 4 << 5 << 6 << 7 << 8 << 9 << 10 << 11 << 12;
   labels << "Январь" << "Февраль" << "Март" << "Апрель" << "Май" << "Июнь" << "Июль" << "Август" << "Сентябрь" << "Октябрь" << "Ноябрь" << "Декабрь";
   QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
   textTicker->addTicks(months, labels);
   customPlot->xAxis->setTicker(textTicker);
   customPlot->xAxis->setTickLabelRotation(60);
   customPlot->xAxis->setSubTicks(false);
   customPlot->xAxis->setTickLength(0, 4);
   customPlot->xAxis->setRange(0, 8);
   customPlot->xAxis->setBasePen(QPen(Qt::white));
   customPlot->xAxis->setTickPen(QPen(Qt::white));
   customPlot->xAxis->grid()->setVisible(true);
   customPlot->xAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));
   customPlot->xAxis->setTickLabelColor(Qt::white);
   customPlot->xAxis->setLabelColor(Qt::white);

   // prepare y axis:
   customPlot->yAxis->setRange(0, 12.1);
   customPlot->yAxis->setPadding(5); // a bit more space to the left border
   customPlot->yAxis->setLabel("Power Consumption in\nKilowatts per Capita (2007)");
   customPlot->yAxis->setBasePen(QPen(Qt::white));
   customPlot->yAxis->setTickPen(QPen(Qt::white));
   customPlot->yAxis->setSubTickPen(QPen(Qt::white));
   customPlot->yAxis->grid()->setSubGridVisible(true);
   customPlot->yAxis->setTickLabelColor(Qt::white);
   customPlot->yAxis->setLabelColor(Qt::white);
   customPlot->yAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::SolidLine));
   customPlot->yAxis->grid()->setSubGridPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));

   // Add data:
//   QVector<double> arrivalData, departureData;
//   arrivalData   << 5 << 6 << 7 << 8 << 9 << 1 << 2 << 7 << 7 << 7 << 77 << 7;
//   departureData << 5 << 4 << 3 << 2 << 1 << 1 << 2 << 5 << 5 << 5 << 55 << 5;
//   pArrival->setData(months, arrivalData);
//   pDeparture->setData(months, departureData);

   // setup legend:
   customPlot->legend->setVisible(true);
   customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignHCenter);
   customPlot->legend->setBrush(QColor(255, 255, 255, 100));
   customPlot->legend->setBorderPen(Qt::NoPen);
   customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);  //В отличии от QtCharts QCustomPlot поддерживает встроенные функции масштабирования.
}

///*!
//    @brief Метод добавляет данные на график
//*/
void Graphic::AddDataToGrahp(QVector<double> months, QVector<double> arrivalData)
{
    //Добавляем данные на серию
    //ptrGraph[numGraph]->addData(x,y);


    //pArrival->setData(months, arrivalData);
}

///*!
//    @brief Метод очищает данные хранящиеся в памяти графиков
//*/
//void Graphic::ClearGraph(QCustomPlot* cPlot)
//{
//    //Очищаем данные
//    for(int i = 0; i<ptrGraph.size(); i++){
//       ptrGraph[i]->data().clear();
//    }

//    for(int i = 0; i< cPlot->graphCount(); i++){
//        cPlot->graph(i)->data()->clear();
//    }
//    //Обновляем отображение графика
//    cPlot->replot();

//}

///*!
//    @brief Слот обновляет отрисовку графика
//*/
void Graphic::UpdateGraph(QCustomPlot *cPlot){
    //Масштабируем оси
    cPlot->rescaleAxes();
    //Отрисовываем график
    cPlot->replot();
}





