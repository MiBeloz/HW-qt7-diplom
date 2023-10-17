#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    stopConnection = new QPushButton("Отмена", this);
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

    QObject::connect(pDatabase, &DataBase::sig_SendStatusConnection, this, &MainWindow::rec_StatusConnection);
    QObject::connect(pSettings, &Settings::sig_ReadyReadSettings, this, &MainWindow::rec_ReadyReadSettings);
    QObject::connect(pFormSettings, &FormSettings::sig_saveSettings, pSettings, &Settings::writeSettingsAll);
    QObject::connect(pFormSettings, &FormSettings::sig_saveSettings, this, &MainWindow::rec_SaveSettings);
    QObject::connect(pMsg, &QMessageBox::buttonClicked, this, &MainWindow::rec_on_pMsg_buttonClicked);
    QObject::connect(pTimer, &QTimer::timeout, this, &MainWindow::rec_TimerTimeout);
    QObject::connect(stopConnection, &QPushButton::clicked, this, &MainWindow::rec_on_stopConnection_buttonClicked);

    QObject::connect(pDatabase, &DataBase::sig_SendDataFromDB, this, &MainWindow::rec_SendDataFromDB);
    QObject::connect(pDatabase, &DataBase::sig_SendDataFromDBIn, this, &MainWindow::rec_SendDataFromDBIn);
    QObject::connect(pDatabase, &DataBase::sig_SendDataFromDBOut, this, &MainWindow::rec_SendDataFromDBOut);

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

void MainWindow::rec_SaveSettings(QVector<QString> appSettings, QVector<QString> dbSettings)
{
    if (dataForApp[formWidth] != appSettings[formWidth] || dataForApp[formHeight] != appSettings[formHeight]){
        setGeometry(0, 0, appSettings[formWidth].toInt(), appSettings[formHeight].toInt());
        moveToTopCenter();
    }

    if(pDatabase->isChange(dbSettings)){
        pDatabase->disconnectFromDatabase();
        pFormSettings->close();
        pDatabase->addDatabaseData(dbSettings);
        pDatabase->connectToDatabase();
    }

    dataForApp = std::move(appSettings);
    dataForConnect = std::move(dbSettings);
}

void MainWindow::rec_ReadyReadSettings(QVector<QString> appSettings, QVector<QString> dbSettings)
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
        rec_StatusConnection(false);
    }
}

void MainWindow::rec_StatusConnection(bool status)
{
    if (status){
        pixmapStatus.load(":/status/connect.png");
        pixmapStatus = pixmapStatus.scaled(32, 32);
        lb_statusPixmap.setPixmap(pixmapStatus);
        lb_statusText.setText("Подключено");
        ui->settings->setEnabled(true);
        stopConnection->setVisible(false);
        setEnabledWidgets(true);
        secondsPassed = 0;
        connectionAttempts = 0;

        pDatabase->requestToDBListAirports(queryAirports);
    }
    else{
        pixmapStatus.load(":/status/disconnect.png");
        pixmapStatus = pixmapStatus.scaled(32, 32);
        lb_statusPixmap.setPixmap(pixmapStatus);

        //pDatabase->disconnectFromDatabase();
        lb_statusText.setText("Отключено");
        stopConnection->setVisible(false);
        setEnabledWidgets(false);

        QString lastError = pDatabase->getLastError().text();
        if (lastError != ""){
            pMsg->setIcon(QMessageBox::Critical);
            pMsg->setText(pDatabase->getLastError().text());
            pMsg->show();
        }
    }
}

void MainWindow::rec_TimerTimeout()
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

void MainWindow::rec_SendDataFromDB(const QComboBox *pComboBox)
{
    ui->cbox_listAirports->setModel(pComboBox->model());
    //ui->cbox_listAirports->model()->sort(1);
    qDebug() << ui->cbox_listAirports->itemText(0);
    qDebug() << ui->cbox_listAirports->model()->data(ui->cbox_listAirports->model()->index(0,1)).toString();
    //qDebug() << pDatabase->getLastError().text();
    qDebug() << ui->dateE_date->text();
}

void MainWindow::rec_SendDataFromDBIn(const QTableView *pTableView)
{





    ui->tv_flights->setModel(pTableView->model());
}

void MainWindow::rec_SendDataFromDBOut(const QTableView *pTableView)
{
    ui->tv_flights->setModel(pTableView->model());
}

void MainWindow::rec_on_pMsg_buttonClicked()
{
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
    }
}

void MainWindow::rec_on_stopConnection_buttonClicked()
{
    pTimer->stop();
    secondsPassed = 0;
    connectionAttempts = 0;
    ui->settings->setEnabled(true);
    stopConnection->setVisible(false);
    lb_statusText.setText("Отключено");
}

void MainWindow::on_settings_triggered()
{
    pFormSettings->rec_SendSettings(dataForApp, dataForConnect);
    pFormSettings->setModal(true);
    pFormSettings->show();
}

void MainWindow::setEnabledWidgets(bool enabled)
{
    ui->grB_flights->setEnabled(enabled);
    ui->grB_selectZone->setEnabled(enabled);
    ui->grB_buttons->setEnabled(enabled);
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
    if (ui->rb_in->isChecked()){
        pDatabase->requestToDBListIn(queryArrival, ui->cbox_listAirports->model()->data(ui->cbox_listAirports->model()->index(ui->cbox_listAirports->currentIndex(),1)).toString());
    }
    if (ui->rb_out->isChecked()){
        pDatabase->requestToDBListOut(queryDeparture, ui->cbox_listAirports->model()->data(ui->cbox_listAirports->model()->index(ui->cbox_listAirports->currentIndex(),1)).toString());
    }
}
