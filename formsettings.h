#ifndef FORMSETTINGS_H
#define FORMSETTINGS_H

#include <QDialog>

#include "structs.h"

namespace Ui {
class FormSettings;
}

class FormSettings : public QDialog
{
    Q_OBJECT

public:
    explicit FormSettings(QWidget *parent = nullptr);
    ~FormSettings();

signals:
    void sig_saveSettings(QVector<QString> appSettings, QVector<QString> dbSettings);

public slots:
    void rec_sendSettings(QVector<QString> appSettings, QVector<QString> dbSettings);

private slots:
    void on_pb_save_clicked();
    void on_pb_cancel_clicked();
    void on_pb_default_clicked();

private:
    Ui::FormSettings *ui;
};

#endif // FORMSETTINGS_H
