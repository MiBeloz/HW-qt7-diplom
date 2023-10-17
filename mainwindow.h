#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDebug>

#include <QMainWindow>
#include <QLabel>
#include <QResizeEvent>
#include <QScreen>
#include <QMessageBox>
#include <QTimer>


#include "database.h"
#include "settings.h"
#include "formsettings.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void rec_SaveSettings(QVector<QString> appSettings, QVector<QString> dbSettings);
    void rec_ReadyReadSettings(QVector<QString> appSettings, QVector<QString> dbSettings);
    void rec_StatusConnection(bool status);
    void rec_TimerTimeout();
    void rec_SendDataFromDB(const QComboBox *pComboBox);
    void rec_SendDataFromDBIn(const QTableView *pTableView);
    void rec_SendDataFromDBOut(const QTableView *pTableView);

    void rec_on_pMsg_buttonClicked();
    void rec_on_stopConnection_buttonClicked();
    void on_settings_triggered();

    void on_pb_getFlight_clicked();

private:
    Ui::MainWindow *ui;
    QMessageBox *pMsg;
    QTimer *pTimer;
    FormSettings *pFormSettings;
    QVector<QString> dataForConnect;
    QVector<QString> dataForApp;
    Settings *pSettings;
    DataBase *pDatabase;

    QLabel lb_statusPixmap;
    QLabel lb_statusText;
    QPushButton *stopConnection;
    QPixmap pixmapStatus;
    int connectionAttempts = 0;
    int secondsPassed = 0;

    QString queryAirports = "SELECT airport_name->>\'ru\' as \"airportName\", airport_code "
                            "FROM bookings.airports_data";

    QString queryArrival = "SELECT flight_no, scheduled_arrival, ad.airport_name->>\'ru\' as \"Name\" "
                             "FROM bookings.flights f "
                             "JOIN bookings.airports_data ad on ad.airport_code = f.departure_airport "
                             "WHERE f.arrival_airport  = ";

    QString queryDeparture = "SELECT flight_no, scheduled_departure, ad.airport_name->>\'ru\' as \"Name\" "
                           "FROM bookings.flights f "
                           "JOIN bookings.airports_data ad on ad.airport_code = f.arrival_airport "
                           "WHERE f.departure_airport  = ";


    void setEnabledWidgets(bool);
    void resizeEvent(QResizeEvent *event) override;
    void moveToTopCenter();
};
#endif // MAINWINDOW_H
