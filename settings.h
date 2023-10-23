#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QSettings>

#include "structs.h"

class Settings : public QObject
{
    Q_OBJECT
public:
    explicit Settings(QObject *parent = nullptr);

signals:
     void sig_readyReadSettings(QVector<QString> appSettings, QVector<QString> dbSettings);

public slots:
    void writeSettingsApp(QVector<QString> appSettings);
    void writeSettingsAll(QVector<QString> appSettings, QVector<QString> dbSettings);
    void readSettingsAll(QVector<QString> appSettings, QVector<QString> dbSettings);

private:
    QSettings *pSettings;

};

#endif // SETTINGS_H
