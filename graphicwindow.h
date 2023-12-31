#ifndef GRAPHICWINDOW_H
#define GRAPHICWINDOW_H

#include <QDialog>

#include "qcustomplot.h"

namespace Ui {
class GraphicWindow;
}

class GraphicWindow : public QDialog
{
    Q_OBJECT

public:
    explicit GraphicWindow(QWidget *parent = nullptr);
    ~GraphicWindow();

public slots:
    void rec_airportName(QString airportName);
    void rec_requestCongestionYear(QVector<QPair<QString, QString>> graphicData);
    void rec_requestCongestionDayForYear(QVector<QPair<QString, QString>> graphicData);

private slots:
    void on_pb_exit_clicked();

private:
    Ui::GraphicWindow *ui;
    QCPBars *pGraphicBars;
    QCPGraph *pGraphicGraph;

    const QString stratMonth = "Январь";

    QVector<QPair<QString, QString>> graphicDataForDay;
    void displayGraphicForDay(QVector<QPair<QString, QString>> graphicData);

    void clearAllGraphics();
    void clearGraphicGraph();
};

#endif // GRAPHICWINDOW_H
