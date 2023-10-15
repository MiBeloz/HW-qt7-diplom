#include "settings.h"

Settings::Settings(QObject *parent) : QObject(parent)
{
    pSettings = new QSettings(SETTINGS_FILE, QSettings::IniFormat, this);
}

void Settings::writeSettingsApp(QVector<QString> appSettings)
{
    pSettings->beginGroup(GROUP_MAIN_WINDOW);
    pSettings->setValue("saveFormSize", appSettings[saveFormSize]);
    pSettings->setValue("formWidth", appSettings[formWidth]);
    pSettings->setValue("formHeight", appSettings[formHeight]);
    pSettings->endGroup();
}

void Settings::writeSettingsAll(QVector<QString> appSettings, QVector<QString> dbSettings)
{
    pSettings->beginGroup(GROUP_MAIN_WINDOW);
    pSettings->setValue("saveFormSize", appSettings[saveFormSize]);
    pSettings->setValue("formWidth", appSettings[formWidth]);
    pSettings->setValue("formHeight", appSettings[formHeight]);
    pSettings->endGroup();

    pSettings->beginGroup(GROUP_DB);
    pSettings->setValue("hostName", dbSettings[hostName]);
    pSettings->setValue("dbName", dbSettings[dbName]);
    pSettings->setValue("port", dbSettings[port]);
    pSettings->setValue("login", dbSettings[login]);
    if (dbSettings[savePass] == "true")
        pSettings->setValue("pass", dbSettings[pass]);
    else
        pSettings->setValue("pass", "");
    pSettings->setValue("showPass", dbSettings[showPass]);
    pSettings->setValue("savePass", dbSettings[savePass]);
    pSettings->setValue("autoConnect", dbSettings[autoConnect]);
    pSettings->setValue("numOfConnectionAttempts", dbSettings[numOfConnectionAttempts]);
    pSettings->setValue("timeConnect", dbSettings[timeout]);
    pSettings->endGroup();
}

void Settings::readSettingsAll(QVector<QString> appSettings, QVector<QString> dbSettings)
{
    QVector<QVariant> appData(NUM_DATA_FOR_APP);
    QVector<QVariant> dbData(NUM_DATA_FOR_CONNECT_TO_DB);

    pSettings->beginGroup(GROUP_MAIN_WINDOW);
    appData[saveFormSize] = pSettings->value("saveFormSize", DEFAULT_SAVE_FORM_SIZE);
    appData[formWidth] = pSettings->value("formWidth", DEFAULT_FORM_WIDTH);
    appData[formHeight] = pSettings->value("formHeight", DEFAULT_FORM_HEIGHT);
    pSettings->endGroup();

    pSettings->beginGroup(GROUP_DB);
    dbData[hostName] = pSettings->value("hostName", DEFAULT_HOSTNAME);
    dbData[dbName] = pSettings->value("dbName", DEFAULT_DBNAME);
    dbData[port] = pSettings->value("port", DEFAULT_PORT);
    dbData[login] = pSettings->value("login", DEFAULT_LOGIN);
    dbData[pass] = pSettings->value("pass", DEFAULT_PASS);
    dbData[showPass] = pSettings->value("showPass", DEFAULT_SHOW_PASS);
    dbData[savePass] = pSettings->value("savePass", DEFAULT_SAVE_PASS);
    dbData[autoConnect] = pSettings->value("autoConnect", DEFAULT_AUTOCONNECT);
    dbData[numOfConnectionAttempts] = pSettings->value("numOfConnectionAttempts", DEFAULT_NUM_OF_CONN_ATTEMPTS);
    dbData[timeout] = pSettings->value("timeConnect", DEFAULT_TIMEOUT);
    pSettings->endGroup();

    for (int i = 0; i < NUM_DATA_FOR_APP; ++i){
        appSettings[i] = appData[i].toString();
    }

    for (int i = 0; i < NUM_DATA_FOR_CONNECT_TO_DB; ++i){
        dbSettings[i] = dbData[i].toString();
    }

    emit sig_sendSettings(appSettings, dbSettings);
}
