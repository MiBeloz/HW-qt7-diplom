#include "graphicwindow.h"
#include "ui_graphicwindow.h"

GraphicWindow::GraphicWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GraphicWindow)
{
    ui->setupUi(this);

    graphic = new Graphic(ui->cusPlot_year, ui->cusPlot_day);

    connect(ui->cb_month, &QComboBox::currentTextChanged, this, [&](QString arg1){
            graphic->ClearGraph(ui->cusPlot_day);
            graphic->prepareXYaxis(ui->cusPlot_day, graphicDataMMM, graphicGraph, arg1);
            graphic->AddDataToGrahp(graphicDataMMM, graphicGraph, arg1);
            //graphic->setupLegend(ui->cusPlot_day);
            graphic->UpdateGraph(ui->cusPlot_day);
    });
}

GraphicWindow::~GraphicWindow()
{
    delete ui;
    delete graphic;
}

void GraphicWindow::rec_sendAirportName(QString airportName)
{
    ui->lb_airport->setText(airportName);
}

void GraphicWindow::rec_requestCongestionYear(QVector<QPair<QString, QString>> graphicData)
{
    graphic->prepareXYaxis(ui->cusPlot_year, graphicData, graphicBars);
    graphic->AddDataToGrahp(graphicData, graphicBars);
    //graphic->setupLegend(ui->cusPlot_year);
    graphic->UpdateGraph(ui->cusPlot_year);
}

void GraphicWindow::rec_requestCongestionDayForYear(QVector<QPair<QString, QString> > graphicData)
{
    graphicDataMMM = graphicData;

    const QString stratMonth = "Январь";
    int index_cb_month = 0;
    if (graphicData.size() > 0){
        QString month = graphicData[0].first;
        ui->cb_month->addItem(graphicData[0].first);
        if (graphicData[0].first.mid(0, 6) == stratMonth){
            index_cb_month = 0;
        }
        for (int i = 1, j = 0; i < graphicData.size(); ++i){
            QString nextMonth = graphicData[i].first;
            if (month != nextMonth){
                month = nextMonth;
                ui->cb_month->addItem(graphicData[i].first);
                j++;
                if (graphicData[i].first.mid(0, 6) == stratMonth){
                    index_cb_month = j;
                }
            }
        }
    }
    ui->cb_month->setCurrentIndex(index_cb_month);

//    graphic->prepareXYaxis(ui->cusPlot_day, graphicData, graphicGraph, ui->cb_month->currentText());
//    graphic->AddDataToGrahp(graphicData, graphicGraph, ui->cb_month->currentText());
//    //graphic->setupLegend(ui->cusPlot_day);
//    graphic->UpdateGraph(ui->cusPlot_day);
}

void GraphicWindow::on_pb_exit_clicked()
{
    close();
}
