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

    pGraphicWindow = new GraphicWindow(this);

    QObject::connect(pDatabase, &DataBase::sig_SendStatusConnection, this, &MainWindow::rec_StatusConnection);
    QObject::connect(pSettings, &Settings::sig_ReadyReadSettings, this, &MainWindow::rec_ReadyReadSettings);
    QObject::connect(pFormSettings, &FormSettings::sig_saveSettings, this, &MainWindow::rec_SaveSettings);
    QObject::connect(pMsg, &QMessageBox::buttonClicked, this, &MainWindow::rec_on_pMsg_buttonClicked);
    QObject::connect(pTimer, &QTimer::timeout, this, &MainWindow::rec_TimerTimeout);
    QObject::connect(stopConnection, &QPushButton::clicked, this, &MainWindow::rec_on_stopConnection_buttonClicked);

    QObject::connect(ui->cbox_listAirports, &QComboBox::currentTextChanged, this, [&]{ui->pb_getFlights->setEnabled(true);});
    QObject::connect(ui->rb_arrival, &QRadioButton::toggled, this, [&]{ui->pb_getFlights->setEnabled(true);});
    QObject::connect(ui->rb_departure, &QRadioButton::toggled, this, [&]{ui->pb_getFlights->setEnabled(true);});
    QObject::connect(ui->de_departureDate, &QDateEdit::dateChanged, this, [&]{ui->pb_getFlights->setEnabled(true);});

    QObject::connect(pDatabase, &DataBase::sig_SendDataAirports, this, &MainWindow::rec_sendDataAirports);
    QObject::connect(pDatabase, &DataBase::sig_SendDataFlights, this, &MainWindow::rec_sendDataFlights);

    QObject::connect(this, &MainWindow::sig_sendAirportName, pGraphicWindow, &GraphicWindow::rec_sendAirportName);
    QObject::connect(pDatabase, &DataBase::sig_SendCongestionYear, pGraphicWindow, &GraphicWindow::rec_requestCongestionYear);
    QObject::connect(pDatabase, &DataBase::sig_SendCongestionDayForYear, pGraphicWindow, &GraphicWindow::rec_requestCongestionDayForYear);

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

    pSettings->writeSettingsAll(dataForApp, dataForConnect);
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
        ui->menubar_settings->setEnabled(true);
        stopConnection->setVisible(false);
        setEnabledWidgets(true);
        on_pb_clear_tv_flights_clicked();
        ui->menubar_connect->setEnabled(false);
        ui->menubar_disconnect->setEnabled(true);
        ui->pb_getFlights->setEnabled(true);
        secondsPassed = 0;
        connectionAttempts = 0;

        pDatabase->requestListAirports();
    }
    else{
        pixmapStatus.load(":/status/disconnect.png");
        pixmapStatus = pixmapStatus.scaled(32, 32);
        lb_statusPixmap.setPixmap(pixmapStatus);
        lb_statusText.setText("Отключено");
        stopConnection->setVisible(false);
        setEnabledWidgets(false);
        ui->menubar_connect->setEnabled(true);
        ui->menubar_disconnect->setEnabled(false);

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

void MainWindow::rec_sendDataAirports(const QComboBox *pComboBox)
{
    ui->cbox_listAirports->setModel(pComboBox->model());
    ui->cbox_listAirports->setCurrentIndex(0);
}

void MainWindow::rec_sendDataFlights(const QTableView *pTableView)
{
    ui->tv_flights->setModel(pTableView->model());

    ui->tv_flights->resizeColumnToContents(0);
    ui->tv_flights->resizeColumnToContents(1);
    ui->tv_flights->resizeColumnToContents(2);

    ui->pb_clear_tv_flights->setEnabled(true);
}

void MainWindow::rec_on_pMsg_buttonClicked()
{
    pTimer->start();
    ui->menubar_settings->setEnabled(false);
    stopConnection->setVisible(true);
    if (dataForConnect[numOfConnectionAttempts].toInt() == connectionAttempts + 1){
        ui->menubar_settings->setEnabled(true);
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
    ui->menubar_settings->setEnabled(true);
    stopConnection->setVisible(false);
    lb_statusText.setText("Отключено");
}

void MainWindow::on_menubar_settings_triggered()
{
    pFormSettings->rec_SendSettings(dataForApp, dataForConnect);
    pFormSettings->setModal(true);
    pFormSettings->show();
}

void MainWindow::setEnabledWidgets(bool enabled)
{
    ui->grB_flights->setEnabled(enabled);
    ui->grB_selectZone->setEnabled(enabled);
    ui->grB_actions->setEnabled(enabled);
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

void MainWindow::on_pb_getFlights_clicked()
{
    QString requestDate = ui->de_departureDate->text().mid(6) + '-' + ui->de_departureDate->text().mid(3, 2) + '-' + ui->de_departureDate->text().mid(0, 2);

    qDebug() << "on_pb_getFlight_clicked-------------------------";
    qDebug() << "airport = " + ui->cbox_listAirports->itemText(ui->cbox_listAirports->currentIndex());
    qDebug() << "airportCode = " + ui->cbox_listAirports->model()->data(ui->cbox_listAirports->model()->index(ui->cbox_listAirports->currentIndex(),1)).toString();
    qDebug() << "date = " + ui->de_departureDate->text();
    qDebug() << "requestDate = " + requestDate;
    qDebug() << "------------------------------------------------";

    if (ui->rb_arrival->isChecked()){
        pDatabase->requestListFlights(ui->cbox_listAirports->model()->data(ui->cbox_listAirports->model()->index(ui->cbox_listAirports->currentIndex(),1)).toString(),
                                          requestDate,
                                          arrival);

        ui->pb_getFlights->setEnabled(false);
    }
    if (ui->rb_departure->isChecked()){
        pDatabase->requestListFlights(ui->cbox_listAirports->model()->data(ui->cbox_listAirports->model()->index(ui->cbox_listAirports->currentIndex(),1)).toString(),
                                          requestDate,
                                          departure);

        ui->pb_getFlights->setEnabled(false);
    }
}

void MainWindow::on_menubar_connect_triggered()
{
    pTimer->stop();
    secondsPassed = 0;
    connectionAttempts = 0;
    pDatabase->connectToDatabase();
}

void MainWindow::on_menubar_disconnect_triggered()
{
    pDatabase->disconnectFromDatabase();
    rec_StatusConnection(false);
}

void MainWindow::on_pb_clear_tv_flights_clicked()
{
    ui->tv_flights->setModel(nullptr);
    ui->pb_clear_tv_flights->setEnabled(false);
    ui->pb_getFlights->setEnabled(true);
}

void MainWindow::on_pb_congestion_clicked()
{
    emit sig_sendAirportName("Аэропорт: " + ui->cbox_listAirports->itemText(ui->cbox_listAirports->currentIndex()));
    pDatabase->requestCongestionYear(ui->cbox_listAirports->model()->data(ui->cbox_listAirports->model()->index(ui->cbox_listAirports->currentIndex(),1)).toString());
    //pDatabase->requestCongestionDayForYear(ui->cbox_listAirports->model()->data(ui->cbox_listAirports->model()->index(ui->cbox_listAirports->currentIndex(),1)).toString());
    pGraphicWindow->setModal(true);
    pGraphicWindow->show();
}
