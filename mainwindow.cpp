#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    dataForConnect.resize(NUM_DATA_FOR_CONNECT_TO_DB);
    readSettings();

    pDatabase = new DataBase(this);

    QObject::connect(pDatabase, &DataBase::sig_SendStatusConnection, this, &MainWindow::ReceiveStatusConnection);

    pDatabase->addDataBase(POSTGRE_DRIVER);
    pDatabase->connectToDataBase(dataForConnect);
}

MainWindow::~MainWindow()
{
    writeSettings();
    delete ui;
}

void MainWindow::writeSettings()
{
    QSettings settings(SETTINGS_FILE, QSettings::IniFormat);

    settings.beginGroup("MainWindowSize");
    settings.setValue("width", width());
    settings.setValue("height", height());
    settings.endGroup();

    settings.beginGroup("DataForConnectToDB");
    settings.setValue("hostName", dataForConnect[hostName]);
    settings.setValue("dbName", dataForConnect[dbName]);
    settings.setValue("port", dataForConnect[port]);
    settings.setValue("login", dataForConnect[login]);
    settings.setValue("pass", dataForConnect[pass]);
    settings.endGroup();
}

void MainWindow::readSettings()
{
    QSettings settings(SETTINGS_FILE, QSettings::IniFormat);
    QList<QVariant> dbData(NUM_DATA_FOR_CONNECT_TO_DB, "");

    settings.beginGroup("MainWindowSize");
    const auto mwWidth = settings.value("width", 800);
    const auto mwHeight = settings.value("height", 600);
    settings.endGroup();

    settings.beginGroup("DataForConnectToDB");
    dbData[hostName] = settings.value("hostName", QString(""));
    dbData[dbName] = settings.value("dbName", QString(""));
    dbData[port] = settings.value("port", QString(""));
    dbData[login] = settings.value("login", QString(""));
    dbData[pass] = settings.value("pass", QString(""));
    settings.endGroup();

    setGeometry(0, 0, mwWidth.toInt(), mwHeight.toInt());
    for (int i = 0; i < NUM_DATA_FOR_CONNECT_TO_DB; ++i){
        dataForConnect[i] = dbData[i].toString();
    }
}

void MainWindow::ReceiveStatusConnection(bool status)
{
    if (status)
        ui->statusbar->showMessage("Подключено");
    else
        ui->statusbar->showMessage("Отключено");
}
