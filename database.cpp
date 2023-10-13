#include "database.h"

DataBase::DataBase(QObject *parent)
    : QObject{parent}
{
    dataBase = new QSqlDatabase();
}

DataBase::~DataBase()
{
    delete dataBase;
}

void DataBase::addDataBase(QString driver, QString nameDB)
{
    *dataBase = QSqlDatabase::addDatabase(driver, nameDB);
}

void DataBase::connectToDataBase(QList<QString> &dataForConnect)
{
    dataBase->setHostName(dataForConnect[hostName]);
    dataBase->setDatabaseName(dataForConnect[dbName]);
    dataBase->setPort(dataForConnect[port].toInt());
    dataBase->setUserName(dataForConnect[login]);
    dataBase->setPassword(dataForConnect[pass]);

    bool status;
    status = dataBase->open();
    emit sig_SendStatusConnection(status);
}

void DataBase::disconnectFromDataBase(QString nameDb)
{
    *dataBase = QSqlDatabase::database(nameDb);
    dataBase->close();
}

void DataBase::requestToDB(QString request, int requestType)
{

}

QSqlError DataBase::getLastError()
{
    return dataBase->lastError();
}
