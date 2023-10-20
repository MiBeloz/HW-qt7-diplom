#ifndef GRAPHICWINDOW_H
#define GRAPHICWINDOW_H

#include <QDialog>

#include "qcustomplot.h"
#include "graphic.h"

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
    void rec_sendAirportName(QString airportName);
    void rec_requestCongestionYear(QVector<QPair<QString, QString>> graphicData);
    void rec_requestCongestionDayForYear(QVector<QPair<QString, QString>> graphicData);

private slots:
    void on_pb_exit_clicked();

private:
    Ui::GraphicWindow *ui;
    Graphic *graphic;
    QVector<QPair<QString, QString> > graphicDataMMM;
;

    QVector<double> ConstructMouse(int numForm, QVector<double> x);
};

#endif // GRAPHICWINDOW_H