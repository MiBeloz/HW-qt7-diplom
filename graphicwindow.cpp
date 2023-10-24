#include "graphicwindow.h"
#include "ui_graphicwindow.h"

GraphicWindow::GraphicWindow(QWidget *parent) :
    QDialog(parent, Qt::WindowTitleHint),
    ui(new Ui::GraphicWindow)
{
    ui->setupUi(this);

    QLinearGradient gradient(0, 0, 0, 400);
    gradient.setColorAt(0, QColor(Qt::white));
    gradient.setColorAt(0.5, QColor(200, 200, 200));
    gradient.setColorAt(1, QColor(130, 130, 130));
    ui->cusPlot_year->setBackground(QBrush(gradient));
    ui->cusPlot_day->setBackground(QBrush(gradient));

    ui->cusPlot_year->legend->setVisible(true);
    ui->cusPlot_year->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignHCenter);
    ui->cusPlot_year->legend->setBrush(QColor(255, 255, 255, 100));
    ui->cusPlot_year->legend->setBorderPen(Qt::NoPen);
    ui->cusPlot_day->legend->setVisible(true);
    ui->cusPlot_day->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignHCenter);
    ui->cusPlot_day->legend->setBrush(QColor(255, 255, 255, 100));
    ui->cusPlot_day->legend->setBorderPen(Qt::NoPen);
    QFont legendFont("Times", 10, QFont::Bold);
    legendFont.setPointSize(10);
    ui->cusPlot_year->legend->setFont(legendFont);
    ui->cusPlot_year->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    ui->cusPlot_day->legend->setFont(legendFont);
    ui->cusPlot_day->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);

    pGraphicBars = new QCPBars(ui->cusPlot_year->xAxis, ui->cusPlot_year->yAxis);
    pGraphicBars->setAntialiased(false);
    pGraphicBars->setName("Прилеты/Вылеты за год");
    pGraphicBars->setPen(QPen(QColor(Qt::black)));
    pGraphicBars->setBrush(QColor(0, 162, 232));

    pGraphicGraph = new QCPGraph(ui->cusPlot_day->xAxis, ui->cusPlot_day->yAxis);
    pGraphicGraph->setAntialiased(true);
    pGraphicGraph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ScatterShape::ssDisc));
    pGraphicGraph->setName("Прилеты/Вылеты за год по дням");
    pGraphicGraph->setPen(QPen(QColor(0, 162, 232), 3));

    QObject::connect(ui->cb_month, &QComboBox::currentTextChanged, this, [&]{
        clearGraphicGraph();
        displayGraphicForDay(graphicDataForDay);
    });
}

GraphicWindow::~GraphicWindow()
{
    delete ui;
}

void GraphicWindow::rec_airportName(QString airportName)
{
    ui->lb_airport->setText(airportName);
}

void GraphicWindow::rec_requestCongestionYear(QVector<QPair<QString, QString>> graphicData)
{
    QVector<double> months;
    QVector<QString> labels;

    for (int i = 0; i < graphicData.size(); ++i){
        months << i + 1;
        labels << graphicData[i].first;
    }

    QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
    textTicker->addTicks(months, labels);
    ui->cusPlot_year->xAxis->setTicker(textTicker);
    ui->cusPlot_year->xAxis->setAntialiased(true);
    ui->cusPlot_year->xAxis->setTickLabelRotation(90);
    ui->cusPlot_year->xAxis->setBasePen(QPen(Qt::black));
    ui->cusPlot_year->xAxis->setTickPen(QPen(Qt::black));
    ui->cusPlot_year->xAxis->grid()->setVisible(true);
    ui->cusPlot_year->xAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));
    ui->cusPlot_year->xAxis->setTickLabelColor(Qt::black);
    ui->cusPlot_year->xAxis->setLabelColor(Qt::black);
    ui->cusPlot_year->xAxis->setRange(0, graphicData.size() + 1);

    ui->cusPlot_year->yAxis->setPadding(5);
    ui->cusPlot_year->yAxis->setLabel("Загруженность(прилеты/вылеты):");
    ui->cusPlot_year->yAxis->setBasePen(QPen(Qt::black));
    ui->cusPlot_year->yAxis->setTickPen(QPen(Qt::black));
    ui->cusPlot_year->yAxis->setSubTickPen(QPen(Qt::black));
    ui->cusPlot_year->yAxis->grid()->setSubGridVisible(true);
    ui->cusPlot_year->yAxis->setTickLabelColor(Qt::black);
    ui->cusPlot_year->yAxis->setLabelColor(Qt::black);
    ui->cusPlot_year->yAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::SolidLine));
    ui->cusPlot_year->yAxis->grid()->setSubGridPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));

    double maxValue = 0;
    for (int i = 0; i < graphicData.size(); ++i){
        pGraphicBars->addData(i + 1, graphicData[i].second.toDouble());

        if (graphicData[i].second.toDouble() > maxValue){
            maxValue = graphicData[i].second.toDouble();
        }
    }
    ui->cusPlot_year->yAxis->setRange(0, maxValue * 1.2);

    ui->cusPlot_year->replot();
}

void GraphicWindow::rec_requestCongestionDayForYear(QVector<QPair<QString, QString> > graphicData)
{
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

    graphicDataForDay = graphicData;
    displayGraphicForDay(graphicDataForDay);
}

void GraphicWindow::on_pb_exit_clicked()
{
    clearAllGraphics();
    close();
}

void GraphicWindow::displayGraphicForDay(QVector<QPair<QString, QString> > graphicData)
{
    QVector<double> days;
    QVector<QString> labels;

    for (int i = 0, j = 0; i < graphicData.size(); ++i){
        if (graphicData[i].first == ui->cb_month->currentText()){
            days << ++j;
            labels << QString::number(j);
        }
    }

    QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
    textTicker->addTicks(days, labels);
    ui->cusPlot_day->xAxis->setTicker(textTicker);
    ui->cusPlot_day->xAxis->setLabel("Дни:");
    ui->cusPlot_day->xAxis->setBasePen(QPen(Qt::black));
    ui->cusPlot_day->xAxis->setTickPen(QPen(Qt::black));
    ui->cusPlot_day->xAxis->grid()->setVisible(true);
    ui->cusPlot_day->xAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));
    ui->cusPlot_day->xAxis->setTickLabelColor(Qt::black);
    ui->cusPlot_day->xAxis->setLabelColor(Qt::black);

    ui->cusPlot_day->yAxis->setLabel("Загруженность(прилеты/вылеты):");
    ui->cusPlot_day->yAxis->setBasePen(QPen(Qt::black));
    ui->cusPlot_day->yAxis->setTickPen(QPen(Qt::black));
    ui->cusPlot_day->yAxis->setSubTickPen(QPen(Qt::black));
    ui->cusPlot_day->yAxis->grid()->setSubGridVisible(true);
    ui->cusPlot_day->yAxis->setTickLabelColor(Qt::black);
    ui->cusPlot_day->yAxis->setLabelColor(Qt::black);
    ui->cusPlot_day->yAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::SolidLine));
    ui->cusPlot_day->yAxis->grid()->setSubGridPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));

    double maxValue = 0;
    for (int i = 0, j = 0; i < graphicData.size(); ++i){
        if (graphicData[i].first == ui->cb_month->currentText()){
            pGraphicGraph->addData(j + 1, graphicData[i].second.toDouble());
            ui->cusPlot_day->xAxis->setRange(0, j + 1);
            j++;

            if (graphicData[i].second.toDouble() > maxValue){
                maxValue = graphicData[i].second.toDouble();
            }
        }
    }
    ui->cusPlot_day->yAxis->setRange(0, maxValue * 1.2);

    ui->cusPlot_day->replot();
}

void GraphicWindow::clearAllGraphics()
{
    for(int i = 0; i < ui->cusPlot_year->graphCount(); i++){
        ui->cusPlot_year->graph(i)->data()->clear();
    }
    ui->cusPlot_year->replot();
    for(int i = 0; i < ui->cusPlot_day->graphCount(); i++){
        ui->cusPlot_day->graph(i)->data()->clear();
    }
    ui->cusPlot_day->replot();

    pGraphicBars->data()->clear();
    pGraphicGraph->data()->clear();

    graphicDataForDay.clear();

    ui->cb_month->clear();
}

void GraphicWindow::clearGraphicGraph()
{
    ui->cusPlot_year->replot();
    for(int i = 0; i < ui->cusPlot_day->graphCount(); i++){
        ui->cusPlot_day->graph(i)->data()->clear();
    }
    ui->cusPlot_day->replot();

    pGraphicGraph->data()->clear();
}
