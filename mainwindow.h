#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QResizeEvent>
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

    void writeSettings();
    void readSettings();

private slots:
    void ReceiveSaveSettings(QVector<QString> appSettings, QVector<QString> dbSettings);
    void ReceiveSendSettings(QVector<QString> appSettings, QVector<QString> dbSettings);
    void ReceiveStatusConnection(bool status);
    void ReceiveTimerTimeout();

    void on_settings_triggered();

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

    void resizeEvent(QResizeEvent *event) override;
    void moveToTopCenter();
};
#endif // MAINWINDOW_H
