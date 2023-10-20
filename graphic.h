//#pragma comment(lib,"qcustomplot.h")
#ifndef GRAPHIC_H
#define GRAPHIC_H

#include <QObject>

//При использовании динамической библиотеки необходимо подключить заголовочный файл,
//но не добавлять его в проект.
#include "qcustomplot.h"

enum GraphicType{
    graphicBars = 0,
    graphicGraph = 1
};

class Graphic : public QObject
{
    Q_OBJECT   

public:
    Graphic(QCustomPlot* customPlotBars, QCustomPlot* customPlotGraph);
    void prepareXYaxis(QCustomPlot* customPlot, QVector<QPair<QString, QString>> graphicData, GraphicType graphicType, QString selectMonth = "");
    void AddDataToGrahp(QVector<QPair<QString, QString> > graphicData, GraphicType graphicType, QString selectMonth = "");
    void setupLegend(QCustomPlot* customPlot);
    void ClearGraph(QCustomPlot* customPlot);
    void UpdateGraph(QCustomPlot* customPlot);

private:
    QCPBars *pGraphicBars;
    QCPGraph *pGraphicGraph;
};

#endif // GRAPHIC_H
