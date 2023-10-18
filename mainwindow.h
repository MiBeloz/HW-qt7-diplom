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
    void rec_sendDataAirportsFromDB(const QComboBox *pComboBox);
    void rec_sendDataFlightsFromDB(const QTableView *pTableView);

    void rec_on_pMsg_buttonClicked();
    void rec_on_stopConnection_buttonClicked();
    void on_menubar_settings_triggered();

    void on_pb_getFlights_clicked();

    void on_menubar_connect_triggered();

    void on_menubar_disconnect_triggered();

    void on_pb_clear_tv_flights_clicked();

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

    void setEnabledWidgets(bool);
    void resizeEvent(QResizeEvent *event) override;
    void moveToTopCenter();
};
#endif // MAINWINDOW_H
