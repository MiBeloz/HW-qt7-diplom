#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>

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
    void ReceiveStatusConnection(bool status);

private:
    Ui::MainWindow *ui;
    QVector<QString> dataForConnect;
    QVector<QString> dataForApp;
    Settings *pSettings;
    DataBase *pDatabase;

    QLabel lb_statusPixmap;
    QLabel lb_statusText;
    QPixmap pixmapStatus;
};
#endif // MAINWINDOW_H
