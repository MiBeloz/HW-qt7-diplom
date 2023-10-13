#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>

#include "database.h"

#define SETTINGS_FILE "settings.ini"

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
    QList<QString> dataForConnect;
    DataBase *pDatabase;
};
#endif // MAINWINDOW_H
