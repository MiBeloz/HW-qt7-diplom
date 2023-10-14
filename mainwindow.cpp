#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    dataForConnect.resize(NUM_DATA_FOR_CONNECT_TO_DB);
    dataForApp.resize(NUM_DATA_FOR_APP);

    pDatabase = new DataBase(this);
    pSettings = new Settings(this);

    QObject::connect(pDatabase, &DataBase::sig_SendStatusConnection, this, &MainWindow::ReceiveStatusConnection);

    pSettings->readSettingsAll(dataForApp, dataForConnect);
    setGeometry(0, 0, dataForApp[formWidth].toInt(), dataForApp[formHeight].toInt());

    pDatabase->addDataBase(POSTGRE_DRIVER);
    pDatabase->connectToDataBase(dataForConnect);
}

MainWindow::~MainWindow()
{
    dataForApp[formWidth] = QString::number(width());
    dataForApp[formHeight] = QString::number(height());
    pSettings->writeSettingsApp(dataForApp);
    delete ui;
}

void MainWindow::ReceiveStatusConnection(bool status)
{
    //ui->statusbar->removeWidget(&lb_statusPixmap);
    //ui->statusbar->removeWidget(&lb_statusText);
    if (status){
        pixmapStatus.load(":/status/connect.png");
        pixmapStatus = pixmapStatus.scaled(32, 32);
        lb_statusPixmap.setPixmap(pixmapStatus);
        lb_statusText.setText("Подключено");
        QFont font = lb_statusText.font();
        font.setPixelSize(12);
        lb_statusText.setFont(font);
    }
    else{
        pixmapStatus.load(":/status/disconnect.png");
        pixmapStatus = pixmapStatus.scaled(32, 32);
        lb_statusPixmap.setPixmap(pixmapStatus);
        lb_statusText.setText("Отключено");
    }
    ui->statusbar->addWidget(&lb_statusPixmap);
    ui->statusbar->addWidget(&lb_statusText);
    QFont font = lb_statusText.font();
    font.setPixelSize(14);
    lb_statusText.setFont(lb_statusText.font());
}
