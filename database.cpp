#include "database.h"

DataBase::DataBase(QObject *parent)
    : QObject{parent}
{
    pDatabase = new QSqlDatabase();
}

DataBase::~DataBase()
{
    delete pDatabase;
}

void DataBase::addDatabaseDriver(QString driver)
{
    *pDatabase = QSqlDatabase::addDatabase(driver);
}

void DataBase::addDatabaseData(QVector<QString> dataForConnect)
{
    pDatabase->setHostName(dataForConnect[hostName]);
    pDatabase->setDatabaseName(dataForConnect[dbName]);
    pDatabase->setPort(dataForConnect[port].toInt());
    pDatabase->setUserName(dataForConnect[login]);
    pDatabase->setPassword(dataForConnect[pass]);
}

void DataBase::connectToDatabase()
{
    bool status;
    status = pDatabase->open();
    emit sig_SendStatusConnection(status);
}

void DataBase::disconnectFromDatabase()
{
    pDatabase->close();
}

void DataBase::requestToDB(QString request, int requestType)
{

}

QSqlError DataBase::getLastError()
{
    return pDatabase->lastError();
}

bool DataBase::checkDatabase(QVector<QString> dataForConnect)
{
    if (pDatabase->hostName() != dataForConnect[hostName] ||
        pDatabase->databaseName() != dataForConnect[dbName] ||
        pDatabase->port() != dataForConnect[port].toInt() ||
        pDatabase->userName() != dataForConnect[login] ||
        pDatabase->password() != dataForConnect[pass]){
        return false;
    }
    return true;
}
