#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //ui->cbox_listAirports->setVisible(false);

    stopConnection = new QPushButton("Отмена", this);
    stopConnection->setVisible(false);
    lb_statusText.setMinimumSize(250, 22);
    ui->statusbar->addWidget(&lb_statusPixmap);
    ui->statusbar->addWidget(&lb_statusText);
    ui->statusbar->addWidget(stopConnection);

    pMsg = new QMessageBox(this);
    pFormSettings = new FormSettings(this);
    pTimer = new QTimer(this);
    pTimer->setTimerType(Qt::TimerType::PreciseTimer);
    pTimer->setInterval(1000);

    dataForConnect.resize(NUM_DATA_FOR_CONNECT_TO_DB);
    dataForApp.resize(NUM_DATA_FOR_APP);

    pDatabase = new DataBase(this);
    pSettings = new Settings(this);

    QObject::connect(pDatabase, &DataBase::sig_SendStatusConnection, this, &MainWindow::ReceiveStatusConnection);
    QObject::connect(pSettings, &Settings::sig_sendSettings, this, &MainWindow::ReceiveSendSettings);
    QObject::connect(pFormSettings, &FormSettings::sig_saveSettings, pSettings, &Settings::writeSettingsAll);
    QObject::connect(pFormSettings, &FormSettings::sig_saveSettings, this, &MainWindow::ReceiveSaveSettings);
    QObject::connect(pMsg, &QMessageBox::buttonClicked, this, [&]{
        pTimer->start();
        ui->settings->setEnabled(false);
        stopConnection->setVisible(true);
        if (dataForConnect[numOfConnectionAttempts].toInt() == connectionAttempts + 1){
            ui->settings->setEnabled(true);
            stopConnection->setVisible(false);
            lb_statusText.setText("Отключено");
        }
        else{
            lb_statusText.setText("Отключено\tПереподключение через: " + dataForConnect[timeout]);
        }});
    QObject::connect(pTimer, &QTimer::timeout, this, &MainWindow::ReceiveTimerTimeout);
    QObject::connect(stopConnection, &QPushButton::clicked, this, [&]{
        pTimer->stop();
        secondsPassed = 0;
        connectionAttempts = 0;
        ui->settings->setEnabled(true);
        stopConnection->setVisible(false);
        lb_statusText.setText("Отключено");
    });

    QObject::connect(pDatabase, &DataBase::sig_SendDataFromDB, this, &MainWindow::ReceiveSendDataFromDB);
    QObject::connect(pDatabase, &DataBase::sig_SendDataFromDBIn, this, &MainWindow::ReceiveSendDataFromDBIn);

    pSettings->readSettingsAll(dataForApp, dataForConnect);
    pSettings->writeSettingsAll(dataForApp, dataForConnect);
}

MainWindow::~MainWindow()
{
    if (dataForApp[saveFormSize] == "true"){
        dataForApp[formWidth] = QString::number(width());
        dataForApp[formHeight] = QString::number(height());
    }
    pSettings->writeSettingsAll(dataForApp, dataForConnect);
    delete ui;
}

void MainWindow::ReceiveSaveSettings(QVector<QString> appSettings, QVector<QString> dbSettings)
{
    if (dataForApp[formWidth] != appSettings[formWidth] || dataForApp[formHeight] != appSettings[formHeight]){
        setGeometry(0, 0, appSettings[formWidth].toInt(), appSettings[formHeight].toInt());
        moveToTopCenter();
    }

    if(!pDatabase->checkDatabase(dbSettings)){
        pDatabase->disconnectFromDatabase();
        pFormSettings->close();
        pDatabase->addDatabaseData(dbSettings);
        pDatabase->connectToDatabase();
    }

    dataForApp = std::move(appSettings);
    dataForConnect = std::move(dbSettings);
}

void MainWindow::ReceiveSendSettings(QVector<QString> appSettings, QVector<QString> dbSettings)
{
    dataForApp = std::move(appSettings);
    dataForConnect = std::move(dbSettings);

    setGeometry(0, 0, dataForApp[formWidth].toInt(), dataForApp[formHeight].toInt());
    moveToTopCenter();

    pDatabase->addDatabaseDriver(POSTGRE_DRIVER);
    pDatabase->addDatabaseData(dataForConnect);
    if (dataForConnect[autoConnect] == "true"){
        pDatabase->connectToDatabase();
    }
    else{
        pixmapStatus.load(":/status/disconnect.png");
        pixmapStatus = pixmapStatus.scaled(32, 32);
        lb_statusPixmap.setPixmap(pixmapStatus);
        lb_statusText.setText("Отключено");
        stopConnection->setVisible(false);
    }
}

void MainWindow::ReceiveStatusConnection(bool status)
{
    if (status){
        pixmapStatus.load(":/status/connect.png");
        pixmapStatus = pixmapStatus.scaled(32, 32);
        lb_statusPixmap.setPixmap(pixmapStatus);
        lb_statusText.setText("Подключено");
        ui->settings->setEnabled(true);
        stopConnection->setVisible(false);
        secondsPassed = 0;
        connectionAttempts = 0;

        pDatabase->requestToDBListAirports(QUERY_LIST_AIROPORTS);
    }
    else{
        pixmapStatus.load(":/status/disconnect.png");
        pixmapStatus = pixmapStatus.scaled(32, 32);
        lb_statusPixmap.setPixmap(pixmapStatus);

        pDatabase->disconnectFromDatabase();
        lb_statusText.setText("Отключено");
        stopConnection->setVisible(false);
        pMsg->setIcon(QMessageBox::Critical);
        pMsg->setText(pDatabase->getLastError().text());
        pMsg->show();
    }
}

void MainWindow::ReceiveTimerTimeout()
{
    secondsPassed += 1;

    if (dataForConnect[numOfConnectionAttempts].toInt() != connectionAttempts + 1){
        lb_statusText.setText("Отключено\tПереподключение через: " + QString::number(dataForConnect[timeout].toInt() - secondsPassed));
    }

    if (dataForConnect[numOfConnectionAttempts].toInt() == 0 && dataForConnect[timeout].toInt() == secondsPassed){
        secondsPassed = 0;
        connectionAttempts = 0;
        pTimer->stop();
        pDatabase->connectToDatabase();
    }
    else if (dataForConnect[numOfConnectionAttempts].toInt() != 0 && dataForConnect[timeout].toInt() == secondsPassed){
        secondsPassed = 0;
        connectionAttempts += 1;
        pTimer->stop();

        if (dataForConnect[numOfConnectionAttempts].toInt() == connectionAttempts){
            secondsPassed = 0;
            connectionAttempts = 0;
        }
        else{
            pDatabase->connectToDatabase();
        }
    }
}

void MainWindow::ReceiveSendDataFromDB(const QComboBox *pComboBox)
{
    ui->cbox_listAirports->setModel(pComboBox->model());
    //ui->cbox_listAirports->model()->sort(1);
    qDebug() << ui->cbox_listAirports->itemText(0);
    qDebug() << ui->cbox_listAirports->model()->data(ui->cbox_listAirports->model()->index(0,1)).toString();
}

void MainWindow::ReceiveSendDataFromDBIn(const QTableView *pTableView)
{
    ui->tv_flights->setModel(pTableView->model());
}

void MainWindow::on_settings_triggered()
{
    pFormSettings->ReceiveSendSettings(dataForApp, dataForConnect);
    pFormSettings->setModal(true);
    pFormSettings->show();
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    if (event->type() == QEvent::Resize && dataForApp[saveFormSize] == "true"){
        dataForApp[formWidth] = QString::number(width());
        dataForApp[formHeight] = QString::number(height());

        pSettings->writeSettingsApp(dataForApp);
    }
}

void MainWindow::moveToTopCenter()
{
    auto screen = QGuiApplication::primaryScreen(); //опеределянм главный экран
    QRect rect = screen->geometry(); //размер экрана
    QPoint center = rect.center(); //координаты центра экрана
    center.setX(center.x() - (this->width() / 2));  // учитываем половину ширины окна
    center.setY(center.y() - (this->height() / 2));  // учитываем половину высоты окна
    move(center);    //перемещаем
}

void MainWindow::on_pb_getFlight_clicked()
{
    pDatabase->requestToDBListIn(QUERY_LIST_IN);
}
