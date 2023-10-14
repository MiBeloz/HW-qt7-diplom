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

public slots:
    void writeSettingsApp(QVector<QString> &appSettings);
    void writeSettingsDB(QVector<QString> &dbSettings);
    void readSettingsAll(QVector<QString> &appSettings, QVector<QString> &dbSettings);

signals:

private:
    QSettings *pSettings;

};

#endif // SETTINGS_H
