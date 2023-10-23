#include "database.h"

Database::Database(QObject *parent)
    : QObject{parent}
{
    pDatabase = new QSqlDatabase();
    pQueryModelTable = new QSqlQueryModel(this);
    pQueryModelAirports = new QSqlQueryModel(this);
    pTableView = new QTableView(nullptr);
    pComboBox = new QComboBox(nullptr);
    pSqlQuery = new QSqlQuery;

    connect(&ftrWtchQueryModelAirports, &QFutureWatcher<void>::finished, this, [&]{
        pComboBox->setModel(pQueryModelAirports);
        emit sig_sendDataAirports(pComboBox);
    });

    connect(&ftrWtchQueryModelTable, &QFutureWatcher<void>::finished, this, [&]{
        pTableView->setModel(pQueryModelTable);
        pTableView->hideColumn(0);
        emit sig_sendDataFlights(pTableView);
    });

    connect(&ftrWtchSqlQueryYear, &QFutureWatcher<void>::finished, this, [&]{
        QVector<QPair<QString, QString>> requestResult;
        while(pSqlQuery->next()){
            QString value = pSqlQuery->value(0).toString();
            QString key = pSqlQuery->value(1).toString();

            QString keyYear = key.mid(0, 4);
            QString keyMonth = key.mid(5, 2);
            int keyMonthInt = keyMonth.toInt();
            key = intToStrMonth(keyMonthInt) + ' ' + keyYear;

            qDebug() << key;
            qDebug() << value;

            requestResult.push_back(qMakePair(key, value));
        }

        emit sig_sendCongestionYear(requestResult);

        ftrSqlQueryDayForYear = QtConcurrent::run([&]{
                *pSqlQuery = QSqlQuery(*pDatabase);

                QSqlError err;
                if(pSqlQuery->exec("SELECT count(flight_no), date_trunc(\'day\', scheduled_departure) as \"Day\" "
                                "FROM bookings.flights f "
                                "WHERE (scheduled_departure::date > date(\'2016-08-31\') "
                                "and scheduled_departure::date <= date(\'2017-08-31\')) "
                                "and (departure_airport = \'" + m_airportCode + "\' or arrival_airport = \'" + m_airportCode + "\') "
                                "GROUP BY \"Day\"") == false){
                    err = pSqlQuery->lastError();
                    emit sig_sendStatusRequestCongestion(err);}
                });

        ftrWtchSqlQueryDayForYear.setFuture(ftrSqlQueryDayForYear);
    });

    connect(&ftrWtchSqlQueryDayForYear, &QFutureWatcher<void>::finished, this, [&]{
        QVector<QPair<QString, QString>> requestResult;
        while(pSqlQuery->next()){
            QString value = pSqlQuery->value(0).toString();
            QString key = pSqlQuery->value(1).toString();

            QString keyYear = key.mid(0, 4);
            QString keyMonth = key.mid(5, 2);
            int keyMonthInt = keyMonth.toInt();
            key = intToStrMonth(keyMonthInt) + ' ' + keyYear;

            qDebug() << key;
            qDebug() << value;

            requestResult.push_back(qMakePair(key, value));
        }

        emit sig_sendCongestionDayForYear(requestResult);
    });
}

Database::~Database()
{
    delete pTableView;
    delete pComboBox;
    delete pSqlQuery;
    delete pDatabase;
}

void Database::addDatabaseDriver(QString driver)
{
    *pDatabase = QSqlDatabase::addDatabase(driver);
}

void Database::addDatabaseData(QVector<QString> dataForConnect)
{
    pDatabase->setHostName(dataForConnect[hostName]);
    pDatabase->setDatabaseName(dataForConnect[dbName]);
    pDatabase->setPort(dataForConnect[port].toInt());
    pDatabase->setUserName(dataForConnect[login]);
    pDatabase->setPassword(dataForConnect[pass]);
}

void Database::connectToDatabase()
{
    emit sig_sendStatusConnection(pDatabase->open());
}

void Database::disconnectFromDatabase()
{
    pDatabase->close();
}

void Database::requestListAirports()
{
    ftrQueryModelAirports = QtConcurrent::run([&]{
        pQueryModelAirports->setQuery("SELECT airport_name->>\'ru\' as \"airportName\", airport_code "
                                      "FROM bookings.airports_data "
                                      "ORDER BY \"airportName\"",
                                      *pDatabase);
    });

    ftrWtchQueryModelAirports.setFuture(ftrQueryModelAirports);
}

void Database::requestListFlights(QString airportCode, QString requestDate, RouteType type)
{
    ftrQueryModelTable = QtConcurrent::run([&]{
        if (type == arrival){
            pQueryModelTable->setQuery("SELECT flight_no, scheduled_arrival, ad.airport_name->>\'ru\' as \"Name\" "
                                       "FROM bookings.flights f "
                                       "JOIN bookings.airports_data ad on ad.airport_code = f.departure_airport "
                                       "WHERE f.arrival_airport  = \'" + airportCode + "\' "
                                       "AND scheduled_departure::date = date(\'" + requestDate + "\')",
                                       *pDatabase);

            pQueryModelTable->setHeaderData(2, Qt::Horizontal, tr("Аэропорт отправления"));
        }
        if (type == departure){
            pQueryModelTable->setQuery("SELECT flight_no, scheduled_departure, ad.airport_name->>\'ru\' as \"Name\" "
                                       "FROM bookings.flights f "
                                       "JOIN bookings.airports_data ad on ad.airport_code = f.arrival_airport "
                                       "WHERE f.departure_airport  = \'" + airportCode + "\' "
                                       "AND scheduled_departure::date = date(\'" + requestDate + "\')",
                                       *pDatabase);

            pQueryModelTable->setHeaderData(2, Qt::Horizontal, tr("Аэропорт назначения"));
        }

        pQueryModelTable->setHeaderData(0, Qt::Horizontal, tr("Номер рейса"));
        pQueryModelTable->setHeaderData(1, Qt::Horizontal, tr("Время вылета"));
    });

    ftrWtchQueryModelTable.setFuture(ftrQueryModelTable);
}

void Database::requestCongestion(QString airportCode)
{
    m_airportCode = airportCode;
    ftrSqlQueryYear = QtConcurrent::run([&]{
        *pSqlQuery = QSqlQuery(*pDatabase);

        QSqlError err;
        if(pSqlQuery->exec("SELECT count(flight_no), date_trunc(\'month\', scheduled_departure) as \"Month\" "
                        "FROM bookings.flights f "
                        "WHERE (scheduled_departure::date > date(\'2016-08-31\') "
                        "and scheduled_departure::date <= date(\'2017-08-31\')) "
                        "and (departure_airport = \'" + m_airportCode + "\' or arrival_airport = \'" + m_airportCode + "\') "
                        "GROUP BY \"Month\"") == false){
            err = pSqlQuery->lastError();
            emit sig_sendStatusRequestCongestion(err);}
        });

    ftrWtchSqlQueryYear.setFuture(ftrSqlQueryYear);
}

QSqlError Database::getLastError()
{
    return pDatabase->lastError();
}

bool Database::isChange(QVector<QString> dataForConnect)
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

QString Database::intToStrMonth(int month)
{
    switch(month){
    case(1):
        return "Январь";
    case(2):
        return "Февраль";
    case(3):
        return "Март";
    case(4):
        return "Апрель";
    case(5):
        return "Май";
    case(6):
        return "Июнь";
    case(7):
        return "Июль";
    case(8):
        return "Август";
    case(9):
        return "Сентябрь";
    case(10):
        return "Октябрь";
    case(11):
        return "Ноябрь";
    case(12):
        return "Декабрь";
    default:
        return "Неизвестный месяц";
    }
}
