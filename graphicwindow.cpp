#include "graphicwindow.h"
#include "ui_graphicwindow.h"

GraphicWindow::GraphicWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GraphicWindow)
{
    ui->setupUi(this);

    graphClass = new Graphic(ui->cusPlot_year);
}

GraphicWindow::~GraphicWindow()
{
    delete ui;
    delete graphClass;
}

void GraphicWindow::rec_sendStatYear(QVector<QPair<QString, QString>> result)
{
//    QVector<double> months;
//    QVector<QString> labels;
//    months << 1 << 2 << 3 << 4 << 5 << 6 << 7 << 8 << 9 << 10 << 11 << 12;
//    labels << "Январь" << "Февраль" << "Март" << "Апрель" << "Май" << "Июнь" << "Июль" << "Август" << "Сентябрь" << "Октябрь" << "Ноябрь" << "Декабрь";
//    QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
//    textTicker->addTicks(months, labels);
//    ui->cusPlot_year->xAxis->setTicker(textTicker);



    QVector<double> arrivalData;
    for (auto it = result.begin(); it != result.end(); ++it){
        //labels << it.key();
        //arrivalData << it.value().toDouble();
    }

//    QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
//    textTicker = new QSharedPointer<QCPAxisTickerText>(new QCPAxisTickerText);
//    textTicker->data()->addTicks(months, labels);
//    //textTicker->addTicks(months, labels);
//    ui->cusPlot_year->xAxis->setTicker(*textTicker);
//    ui->cusPlot_year->xAxis->setTickLabelRotation(60);
//    ui->cusPlot_year->xAxis->setSubTicks(false);
//    ui->cusPlot_year->xAxis->setTickLength(0, 4);
//    ui->cusPlot_year->xAxis->setRange(0, 8);
//    ui->cusPlot_year->xAxis->setBasePen(QPen(Qt::white));
//    ui->cusPlot_year->xAxis->setTickPen(QPen(Qt::white));
//    ui->cusPlot_year->xAxis->grid()->setVisible(true);
//    ui->cusPlot_year->xAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));
//    ui->cusPlot_year->xAxis->setTickLabelColor(Qt::white);
//    ui->cusPlot_year->xAxis->setLabelColor(Qt::white);

//    ui->cusPlot_year->yAxis->setRange(0, 12.1);
//    ui->cusPlot_year->yAxis->setPadding(5); // a bit more space to the left border
//    ui->cusPlot_year->yAxis->setLabel("Power Consumption in\nKilowatts per Capita (2007)");
//    ui->cusPlot_year->yAxis->setBasePen(QPen(Qt::white));
//    ui->cusPlot_year->yAxis->setTickPen(QPen(Qt::white));
//    ui->cusPlot_year->yAxis->setSubTickPen(QPen(Qt::white));
//    ui->cusPlot_year->yAxis->grid()->setSubGridVisible(true);
//    ui->cusPlot_year->yAxis->setTickLabelColor(Qt::white);
//    ui->cusPlot_year->yAxis->setLabelColor(Qt::white);
//    ui->cusPlot_year->yAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::SolidLine));
//    ui->cusPlot_year->yAxis->grid()->setSubGridPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));

    //graphClass->AddDataToGrahp(months, arrivalData);

//    ui->cusPlot_year->legend->setVisible(true);
//    ui->cusPlot_year->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignHCenter);
//    ui->cusPlot_year->legend->setBrush(QColor(255, 255, 255, 100));
//    ui->cusPlot_year->legend->setBorderPen(Qt::NoPen);
//    ui->cusPlot_year->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
}

void GraphicWindow::on_pb_exit_clicked()
{
//    graphClass->ClearGraph(ui->cusPlot_year);

//       /*
//           Создаем контейнеры для хранения данных
//       */
//       QVector<double> x;
//       QVector<double> y;

//       /*
//           считываем шаг сетки и граничные значения
//       */
//       double step = 0.1;
//       //Шаг не может быть равен нулю
//       if(step <= 0){
//           step = 0.1;
//           //ui->dsB_step->setValue(0.1);
//       }

////       double minVal = ui->dsB_minRange->value();
////       double maxVal = ui->dsB_maxRange->value() + step; //Учитываем ноль
//       double minVal = 0;
//       double maxVal = 100 + step; //Учитываем ноль

//       /*
//           Формируем сетку значений, ресайзим вектор и заполняем его
//       */
//       double steps = round(((maxVal-minVal)/step));
//       x.resize(steps);
//       x[0] = minVal; //Начинакем с минимального значения
//       for(int i = 1; i<steps; i++){
//           x[i] = x[i-1]+step;
//       }

//       /*
//        * Заполняем массив значений в зависимости
//        * от выбранного флажка
//        */
//       y.resize(steps);
//       //Если выбран косинус

//           minVal = -7;
//           step = 0.01;
//           maxVal = 7+step;
//           steps = round(((maxVal-minVal)/step));
//           x.clear();
//           x.resize(steps);
//           x[0] = minVal;
//           for(int i = 1; i<steps; i++){
//               x[i] = x[i-1]+step;
//           }

//           QVector<double> data;
//           data.resize(steps);
//           y.clear();
//           y.resize(steps);

//           for(int i = 0; i<2; i++){
//               data = ConstructMouse(i, x);
//               graphClass->AddDataToGrahp(x, data, i);
//               data.clear();
//           }



       graphClass->UpdateGraph(ui->cusPlot_year);
}


//QVector<double> GraphicWindow::ConstructMouse(int numForm, QVector<double> x)
//{

//    QVector<double> result;
//    result.resize(x.size());

//    double tmpX = 0;
//    if(numForm == 0){

//        for(int i = 0; i<x.size(); i++){
//            tmpX = abs(x[i]);

//            if(tmpX>7){

//                continue;

//            }
//            else if (tmpX < 0.5){

//                result[i] = 2.25;

//            }
//            else if(0.5 <= tmpX  && tmpX < 0.75 ){

//                result[i] = 3 * tmpX + 0.75;

//            }
//            else if (0.75 <= tmpX && tmpX< 1.0){

//                result[i] = 9 - 8 * tmpX;

//            }
//            else if(1 <= tmpX && tmpX < 3){

//                result[i] = (1.5 - 0.5 * tmpX - 3 * sqrt(10) / 7 * (sqrt(3 - tmpX*tmpX + 2 * tmpX) - 2));

//            }
//            else if(3 <= tmpX && tmpX <= 7){

//                result[i] = 3 * sqrt(-((tmpX / 7)*(tmpX / 7)) + 1);

//            }

//        }
//    }
//    else if(numForm == 1){

//        for(int i = 0; i< x.size(); i++){

//            tmpX = abs(x[i]);

//            if(tmpX > 7){
//                continue;
//            }
//            else if(0 <= tmpX && tmpX< 4 ){

//                result[i] = (abs(tmpX / 2) - (3*sqrt(33) - 7) / 112 * tmpX*tmpX + sqrt(1 - (abs(tmpX - 2) - 1)*(abs(tmpX - 2) - 1)) - 3);
//            }
//            else if(4 <= tmpX && tmpX <= 7){

//                result[i] = -3 * sqrt(-((tmpX / 7)*(tmpX / 7)) + 1);
//            }
//        }
//    }

//    return result;
//}
