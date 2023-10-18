#include "database.h"

DataBase::DataBase(QObject *parent)
    : QObject{parent}
{
    pDatabase = new QSqlDatabase();
    pQueryModelTable = new QSqlQueryModel(this);
    pQueryModelAirports = new QSqlQueryModel(this);
    pTableView = new QTableView(nullptr);
    pComboBox = new QComboBox(nullptr);
}

DataBase::~DataBase()
{
    delete pTableView;
    delete pComboBox;
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
    emit sig_SendStatusConnection(pDatabase->open());
}

void DataBase::disconnectFromDatabase()
{
    pDatabase->close();
}

void DataBase::requestListAirportsToDB()
{
    pQueryModelAirports->setQuery("SELECT airport_name->>\'ru\' as \"airportName\", airport_code "
                                  "FROM bookings.airports_data",
                                  *pDatabase);

    pComboBox->setModel(pQueryModelAirports);

    emit sig_SendDataAirportsFromDB(pComboBox);
}

void DataBase::requestListFlightsToDB(QString airportCode, QString requestDate, requestType type)
{
    if (type == arrival){
        pQueryModelTable->setQuery("SELECT flight_no, scheduled_arrival, ad.airport_name->>\'ru\' as \"Name\" "
                                   "FROM bookings.flights f "
                                   "JOIN bookings.airports_data ad on ad.airport_code = f.departure_airport "
                                   "WHERE f.arrival_airport  = \'" + airportCode + "\' "
                                   "AND scheduled_departure::date = date(\'" + requestDate + "\')",
                                   *pDatabase);
    }
    if (type == departure){
        pQueryModelTable->setQuery("SELECT flight_no, scheduled_departure, ad.airport_name->>\'ru\' as \"Name\" "
                                   "FROM bookings.flights f "
                                   "JOIN bookings.airports_data ad on ad.airport_code = f.arrival_airport "
                                   "WHERE f.departure_airport  = \'" + airportCode + "\' "
                                   "AND scheduled_departure::date = date(\'" + requestDate + "\')",
                                   *pDatabase);
    }

    pQueryModelTable->setHeaderData(0, Qt::Horizontal, tr("Номер рейса"));
    pQueryModelTable->setHeaderData(1, Qt::Horizontal, tr("Время вылета"));
    if (type == arrival){
        pQueryModelTable->setHeaderData(2, Qt::Horizontal, tr("Аэропорт отправления"));
    }
    if (type == departure){
        pQueryModelTable->setHeaderData(2, Qt::Horizontal, tr("Аэропорт назначения"));
    }

    pTableView->setModel(pQueryModelTable);
    pTableView->hideColumn(0);

    emit sig_SendDataFlightsFromDB(pTableView);
}

QSqlError DataBase::getLastError()
{
    return pDatabase->lastError();
}

bool DataBase::isChange(QVector<QString> dataForConnect)
{
    if (pDatabase->hostName() != dataForConnect[hostName] ||
        pDatabase->databaseName() != dataForConnect[dbName] ||
        pDatabase->port() != dataForConnect[port].toInt() ||
        pDatabase->userName() != dataForConnect[login] ||
        pDatabase->password() != dataForConnect[pass]){
        return true;
    }
    return false;
}
