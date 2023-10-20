#include "graphic.h"

Graphic::Graphic(QCustomPlot* customPlotBars, QCustomPlot* customPlotGraph)
{
    pGraphicBars = new QCPBars(customPlotBars->xAxis, customPlotBars->yAxis);
    pGraphicBars->setAntialiased(false);  // gives more crisp, pixel aligned bar borders
    pGraphicBars->setStackingGap(1);
    // set names and colors:
    pGraphicBars->setName("Прилеты/Вылеты за год");
    pGraphicBars->setPen(QPen(QColor(0, 168, 140).lighter(130)));
    pGraphicBars->setBrush(QColor(0, 168, 140));

    pGraphicGraph = new QCPGraph(customPlotGraph->xAxis, customPlotGraph->yAxis);
    pGraphicGraph->setAntialiased(false);  // gives more crisp, pixel aligned bar borders
    // set names and colors:
    pGraphicGraph->setName("Прилеты/Вылеты за год по месяцам");
    pGraphicGraph->setPen(QPen(QColor(0, 168, 140).lighter(130)));
    pGraphicGraph->setBrush(QColor(0, 168, 140));
}

void Graphic::prepareXYaxis(QCustomPlot *customPlot, QVector<QPair<QString, QString> > graphicData, GraphicType graphicType, QString selectMonth)
{
    // set dark background gradient:
    QLinearGradient gradient(0, 0, 0, 400);
    gradient.setColorAt(0, QColor(90, 90, 90));
    gradient.setColorAt(0.38, QColor(105, 105, 105));
    gradient.setColorAt(1, QColor(70, 70, 70));
    customPlot->setBackground(QBrush(gradient));

    if (graphicType == GraphicType::graphicBars){
        // prepare x axis:
        QVector<double> months;
        QVector<QString> labels;

        for (int i = 0; i < graphicData.size(); ++i){
            months << i + 1;
            labels << graphicData[i].first;
        }

        QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
        textTicker->addTicks(months, labels);
        customPlot->xAxis->setTicker(textTicker);
        customPlot->xAxis->setTickLabelRotation(80);
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
        customPlot->yAxis->setLabel("Загруженность(прилеты/вылеты):");
        customPlot->yAxis->setBasePen(QPen(Qt::white));
        customPlot->yAxis->setTickPen(QPen(Qt::white));
        customPlot->yAxis->setSubTickPen(QPen(Qt::white));
        customPlot->yAxis->grid()->setSubGridVisible(true);
        customPlot->yAxis->setTickLabelColor(Qt::white);
        customPlot->yAxis->setLabelColor(Qt::white);
        customPlot->yAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::SolidLine));
        customPlot->yAxis->grid()->setSubGridPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));
    }

    if (graphicType == GraphicType::graphicGraph){
        // prepare x axis:
        QVector<double> days;
        QVector<QString> labels;

        for (int i = 0, j = 0; i < graphicData.size(); ++i){
            if (graphicData[i].first == selectMonth){
                days << ++j;
                labels << QString::number(j);
            }
        }

        QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
        textTicker->addTicks(days, labels);
        customPlot->xAxis->setTicker(textTicker);
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
        customPlot->yAxis->setLabel("Загруженность(прилеты/вылеты):");
        customPlot->yAxis->setBasePen(QPen(Qt::white));
        customPlot->yAxis->setTickPen(QPen(Qt::white));
        customPlot->yAxis->setSubTickPen(QPen(Qt::white));
        customPlot->yAxis->grid()->setSubGridVisible(true);
        customPlot->yAxis->setTickLabelColor(Qt::white);
        customPlot->yAxis->setLabelColor(Qt::white);
        customPlot->yAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::SolidLine));
        customPlot->yAxis->grid()->setSubGridPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));
    }
}

void Graphic::AddDataToGrahp(QVector<QPair<QString, QString> > graphicData, GraphicType graphicType, QString selectMonth)
{
    if (graphicType == GraphicType::graphicBars){
        for (int i = 0; i < graphicData.size(); ++i){
            pGraphicBars->addData(i + 1, graphicData[i].second.toDouble());
        }
    }

    if (graphicType == GraphicType::graphicGraph){
        for (int i = 0, j = 0; i < graphicData.size(); ++i){
            if (graphicData[i].first == selectMonth){
                pGraphicGraph->addData(j + 1, graphicData[i].second.toDouble());
                j++;
            }
        }
    }
}

void Graphic::ClearGraph(QCustomPlot* customPlot)
{
    for(int i = 0; i< customPlot->graphCount(); i++){
        customPlot->graph(i)->data()->clear();
    }

    customPlot->replot();
}

void Graphic::UpdateGraph(QCustomPlot *customPlot)
{
    customPlot->rescaleAxes();
    customPlot->replot();
}

void Graphic::setupLegend(QCustomPlot *customPlot)
{
    // setup legend:
    customPlot->legend->setVisible(true);
    customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignHCenter);
    customPlot->legend->setBrush(QColor(255, 255, 255, 100));
    customPlot->legend->setBorderPen(Qt::NoPen);
    QFont legendFont("Times", 10, QFont::Bold);
    legendFont.setPointSize(10);
    customPlot->legend->setFont(legendFont);
    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
}





