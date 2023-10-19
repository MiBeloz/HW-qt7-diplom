#ifndef GRAPHIC_H
#define GRAPHIC_H

#include <QObject>
#include <QMap>

//При использовании динамической библиотеки необходимо подключить заголовочный файл,
//но не добавлять его в проект.
#include "qcustomplot.h"

class Graphic : public QObject
{
    Q_OBJECT   

public:
    Graphic(QCustomPlot* customPlot);
    void AddDataToGrahp(QVector<double> months, QVector<double> arrivalData);
    void ClearGraph(QCustomPlot* customPlot);
    void UpdateGraph(QCustomPlot* customPlot);

private:
    QCPBars *pArrival;
    //QCPBars *pDeparture;
};

#endif // GRAPHIC_H
