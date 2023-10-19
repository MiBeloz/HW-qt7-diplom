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
    void rec_sendStatYear(QVector<QPair<QString, QString>> result);

private slots:
    void on_pb_exit_clicked();

private:
    Ui::GraphicWindow *ui;
    QCPGraph* graphic;
    Graphic* graphClass;

    QVector<double> ConstructMouse(int numForm, QVector<double> x);
};

#endif // GRAPHICWINDOW_H
