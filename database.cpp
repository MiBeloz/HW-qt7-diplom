#include "database.h"

DataBase::DataBase(QObject *parent)
    : QObject{parent}
{
    pDataBase = new QSqlDatabase();
}

DataBase::~DataBase()
{
    delete pDataBase;
}

void DataBase::addDataBase(QString driver, QString nameDB)
{
    *pDataBase = QSqlDatabase::addDatabase(driver, nameDB);
}

void DataBase::connectToDataBase(QVector<QString> &dataForConnect)
{
    pDataBase->setHostName(dataForConnect[hostName]);
    pDataBase->setDatabaseName(dataForConnect[dbName]);
    pDataBase->setPort(dataForConnect[port].toInt());
    pDataBase->setUserName(dataForConnect[login]);
    pDataBase->setPassword(dataForConnect[pass]);

    bool status;
    status = pDataBase->open();
    emit sig_SendStatusConnection(status);
}

void DataBase::disconnectFromDataBase(QString nameDb)
{
    *pDataBase = QSqlDatabase::database(nameDb);
    pDataBase->close();
}

void DataBase::requestToDB(QString request, int requestType)
{

}

QSqlError DataBase::getLastError()
{
    return pDataBase->lastError();
}
