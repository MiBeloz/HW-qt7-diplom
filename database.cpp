#include "database.h"

DataBase::DataBase(QObject *parent)
    : QObject{parent}
{
    pDatabase = new QSqlDatabase();
    pQueryModelTable = new QSqlQueryModel(this);
    pQueryModelAirports = new QSqlQueryModel(this);
    pTableView = new QTableView(nullptr);
    pComboBox = new QComboBox(nullptr);
    pSqlQuery = new QSqlQuery;
}

DataBase::~DataBase()
{
    delete pTableView;
    delete pComboBox;
    delete pSqlQuery;
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

void DataBase::requestListFlightsToDB(QString airportCode, QString requestDate, routeType type)
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

void DataBase::requestCongestionYear(QString airportCode)
{
    *pSqlQuery = QSqlQuery(*pDatabase);

    pSqlQuery->exec("SELECT count(flight_no), date_trunc(\'month\', scheduled_departure) as \"Month\" "
                       "FROM bookings.flights f "
                       "WHERE (scheduled_departure::date > date(\'2016-08-31\') "
                       "and scheduled_departure::date <= date(\'2017-08-31\')) "
                       "and (departure_airport = \'YKS\' or arrival_airport = \'YKS\') "
                       "GROUP BY \"Month\"");



    QMap<QString, QString> requestResult;
    while(pSqlQuery->next()){
        QString key = pSqlQuery->value(0).toString();
        QString value = pSqlQuery->value(1).toString();

        qDebug() << key;
        qDebug() << value;

        //requestResult.
    }

    emit sig_SendCongestionYear(requestResult);
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
