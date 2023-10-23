#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QLabel>
#include <QResizeEvent>
#include <QScreen>
#include <QMessageBox>
#include <QTimer>

#include "about.h"
#include "database.h"
#include "settings.h"
#include "formsettings.h"
#include "graphicwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void sig_sendAirportName(QString airportName);

private slots:
    void rec_saveSettings(QVector<QString> appSettings, QVector<QString> dbSettings);
    void rec_readyReadSettings(QVector<QString> appSettings, QVector<QString> dbSettings);
    void rec_statusConnection(bool status);
    void rec_timerTimeout();
    void rec_sendDataAirports(const QComboBox *pComboBox);
    void rec_sendDataFlights(const QTableView *pTableView);
    void rec_statusRequestCongestion(QSqlError err);

    void rec_on_pMsg_buttonClicked();
    void rec_on_stopConnection_buttonClicked();
    void on_menubar_settings_triggered();
    void on_pb_getFlights_clicked();
    void on_menubar_connect_triggered();
    void on_menubar_disconnect_triggered();
    void on_pb_clear_tv_flights_clicked();
    void on_pb_congestion_clicked();
    void on_menubar_about_triggered();

private:
    Ui::MainWindow *ui;
    About *pAbout;
    QMessageBox *pMsg;
    QTimer *pTimer;
    FormSettings *pFormSettings;
    QVector<QString> dataForConnect;
    QVector<QString> dataForApp;
    Settings *pSettings;
    Database *pDatabase;
    GraphicWindow *pGraphicWindow;

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
