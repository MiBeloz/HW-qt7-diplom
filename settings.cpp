#include "settings.h"

Settings::Settings(QObject *parent) : QObject(parent)
{
    pSettings = new QSettings(SETTINGS_FILE, QSettings::IniFormat, this);
}

void Settings::writeSettingsApp(QVector<QString> &appSettings)
{
    pSettings->beginGroup(GROUP_MAIN_WINDOW);
    pSettings->setValue("formWidth", appSettings[formWidth]);
    pSettings->setValue("formHeight", appSettings[formHeight]);
    pSettings->endGroup();
}

void Settings::writeSettingsDB(QVector<QString> &dbSettings)
{
    pSettings->beginGroup(GROUP_DB);
    pSettings->setValue("hostName", dbSettings[hostName]);
    pSettings->setValue("dbName", dbSettings[dbName]);
    pSettings->setValue("port", dbSettings[port]);
    pSettings->setValue("login", dbSettings[login]);
    pSettings->setValue("pass", dbSettings[pass]);
    pSettings->endGroup();
}

void Settings::readSettingsAll(QVector<QString> &appSettings, QVector<QString> &dbSettings)
{
    QVector<QVariant> appData(NUM_DATA_FOR_APP, "");
    QVector<QVariant> dbData(NUM_DATA_FOR_CONNECT_TO_DB, "");

    pSettings->beginGroup(GROUP_MAIN_WINDOW);
    appData[formWidth] = pSettings->value("formWidth", 800);
    appData[formHeight] = pSettings->value("formHeight", 600);
    pSettings->endGroup();

    pSettings->beginGroup(GROUP_DB);
    dbData[hostName] = pSettings->value("hostName", QString(""));
    dbData[dbName] = pSettings->value("dbName", QString(""));
    dbData[port] = pSettings->value("port", QString(""));
    dbData[login] = pSettings->value("login", QString(""));
    dbData[pass] = pSettings->value("pass", QString(""));
    pSettings->endGroup();

    for (int i = 0; i < NUM_DATA_FOR_APP; ++i){
        appSettings[i] = appData[i].toString();
    }

    for (int i = 0; i < NUM_DATA_FOR_CONNECT_TO_DB; ++i){
        dbSettings[i] = dbData[i].toString();
    }
}
