#include "database.h"

DataBase::DataBase(QObject *parent)
    : QObject{parent}
{
    pDatabase = new QSqlDatabase();
    pQueryModelTable = new QSqlQueryModel(this);
    pQueryModelAirports = new QSqlQueryModel(this);
    pTableView = new QTableView(nullptr);
    pComboBox = new QComboBox(nullptr);
    pSqlQueryStatYear = new QSqlQuery;
    //pSqlQueryStatDaysForYear = new QSqlQuery;

    connect(&ftrWtchQueryModelAirports, &QFutureWatcher<void>::finished, this, [&]{
        pComboBox->setModel(pQueryModelAirports);
        emit sig_SendDataAirports(pComboBox);
    });

    connect(&ftrWtchQueryModelTable, &QFutureWatcher<void>::finished, this, [&]{
        pTableView->setModel(pQueryModelTable);
        pTableView->hideColumn(0);
        emit sig_SendDataFlights(pTableView);
    });

    connect(&ftrWtchSqlQueryStatYear, &QFutureWatcher<void>::finished, this, [&]{
        QVector<QPair<QString, QString>> requestResultYear;
        while(pSqlQueryStatYear->next()){
            QString value = pSqlQueryStatYear->value(0).toString();
            QString key = pSqlQueryStatYear->value(1).toString();

            //qDebug() << key;
            //qDebug() << value;

            QString keyYear = key.mid(0, 4);
            QString keyMonth = key.mid(5, 2);
            int keyMonthInt = keyMonth.toInt();
            key = intToStrMonth(keyMonthInt) + ' ' + keyYear;

            //qDebug() << key;
            //qDebug() << value;

            requestResultYear.push_back(qMakePair(key, value));
        }

        QVector<QPair<QString, QString>> requestResultDayForYear;
        while(pSqlQueryStatDaysForYear->next()){
            QString value = pSqlQueryStatDaysForYear->value(0).toString();
            QString key = pSqlQueryStatDaysForYear->value(1).toString();

            //qDebug() << key;
            //qDebug() << value;

            QString keyYear = key.mid(0, 4);
            QString keyMonth = key.mid(5, 2);
            int keyMonthInt = keyMonth.toInt();
            key = intToStrMonth(keyMonthInt) + ' ' + keyYear;

            //qDebug() << key;
            //qDebug() << value;

            requestResultDayForYear.push_back(qMakePair(key, value));
        }

        emit sig_SendCongestionYear(requestResultYear);
        //emit sig_SendCongestionDayForYear(requestResultDayForYear);
    });
}

DataBase::~DataBase()
{
    delete pTableView;
    delete pComboBox;
    delete pSqlQueryStatYear;
    //delete pSqlQueryStatDaysForYear;
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

void DataBase::requestListAirports()
{
    ftrQueryModelAirports = QtConcurrent::run([&]{
        pQueryModelAirports->setQuery("SELECT airport_name->>\'ru\' as \"airportName\", airport_code "
                                      "FROM bookings.airports_data "
                                      "ORDER BY \"airportName\"",
                                      *pDatabase);
    });

    ftrWtchQueryModelAirports.setFuture(ftrQueryModelAirports);
}

void DataBase::requestListFlights(QString airportCode, QString requestDate, routeType type)
{
    ftrQueryModelTable = QtConcurrent::run([&]{
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
    });

    ftrWtchQueryModelTable.setFuture(ftrQueryModelTable);
}

void DataBase::requestCongestionYear(QString airportCode)
{
    ftrSqlQueryStatYear = QtConcurrent::run([&]{
        *pSqlQueryStatYear = QSqlQuery(*pDatabase);

        pSqlQueryStatYear->exec("SELECT count(flight_no), date_trunc(\'month\', scheduled_departure) as \"Month\" "
                                "FROM bookings.flights f "
                                "WHERE (scheduled_departure::date > date(\'2016-08-31\') "
                                "and scheduled_departure::date <= date(\'2017-08-31\')) "
                                "and (departure_airport = \'" + airportCode + "\' or arrival_airport = \'" + airportCode + "\') "
                                "GROUP BY \"Month\"");

//        QVector<QPair<QString, QString>> requestResultYear;
//        while(pSqlQueryStatYear->next()){
//            QString value = pSqlQueryStatYear->value(0).toString();
//            QString key = pSqlQueryStatYear->value(1).toString();

//            //qDebug() << key;
//            //qDebug() << value;

//            QString keyYear = key.mid(0, 4);
//            QString keyMonth = key.mid(5, 2);
//            int keyMonthInt = keyMonth.toInt();
//            key = intToStrMonth(keyMonthInt) + ' ' + keyYear;

//            //qDebug() << key;
//            //qDebug() << value;

//            requestResultYear.push_back(qMakePair(key, value));
//        }

        //qDebug() << pSqlQueryStatYear->isNull(0);
        while (pSqlQueryStatYear->isNull(0)){
            //qDebug() << pSqlQueryStatYear->isNull(0);


        //while (!pSqlQueryStatYear->isActive()){

            //*pSqlQueryStatDaysForYear = QSqlQuery(*pDatabase);
            pSqlQueryStatDaysForYear->exec("SELECT count(flight_no), date_trunc(\'day\', scheduled_departure) as \"Day\" "
                                       "FROM bookings.flights f "
                                       "WHERE (scheduled_departure::date > date(\'2016-08-31\') "
                                       "and scheduled_departure::date <= date(\'2017-08-31\')) "
                                       "and (departure_airport = \'" + airportCode + "\' or arrival_airport = \'" + airportCode + "\') "
                                       "GROUP BY \"Day\"");



//        QVector<QPair<QString, QString>> requestResultDayForYear;
//        while(pSqlQueryStatYear->next()){
//            QString value = pSqlQueryStatYear->value(0).toString();
//            QString key = pSqlQueryStatYear->value(1).toString();

//            //qDebug() << key;
//            //qDebug() << value;

//            QString keyYear = key.mid(0, 4);
//            QString keyMonth = key.mid(5, 2);
//            int keyMonthInt = keyMonth.toInt();
//            key = intToStrMonth(keyMonthInt) + ' ' + keyYear;

//            //qDebug() << key;
//            //qDebug() << value;

//            requestResultDayForYear.push_back(qMakePair(key, value));
//        }
//            while (pSqlQueryStatDaysForYear->isActive()) {
//                continue;
//            }
        }


    });

    ftrWtchSqlQueryStatYear.setFuture(ftrSqlQueryStatYear);
}

void DataBase::requestCongestionDayForYear(QString airportCode)
{
//    *pSqlQueryStatDaysForYear = QSqlQuery(*pDatabase);

//    pSqlQueryStatDaysForYear->exec("SELECT count(flight_no), date_trunc(\'day\', scheduled_departure) as \"Day\" "
//                                   "FROM bookings.flights f "
//                                   "WHERE (scheduled_departure::date > date(\'2016-08-31\') "
//                                   "and scheduled_departure::date <= date(\'2017-08-31\')) "
//                                   "and (departure_airport = \'" + airportCode + "\' or arrival_airport = \'" + airportCode + "\') "
//                                   "GROUP BY \"Day\"");



//    QVector<QPair<QString, QString>> requestResult;
//    while(pSqlQueryStatDaysForYear->next()){
//        QString value = pSqlQueryStatDaysForYear->value(0).toString();
//        QString key = pSqlQueryStatDaysForYear->value(1).toString();

//        //qDebug() << key;
//        //qDebug() << value;

//        QString keyYear = key.mid(0, 4);
//        QString keyMonth = key.mid(5, 2);
//        int keyMonthInt = keyMonth.toInt();
//        key = intToStrMonth(keyMonthInt) + ' ' + keyYear;

//        //qDebug() << key;
//        //qDebug() << value;

//        requestResult.push_back(qMakePair(key, value));
//    }

//    emit sig_SendCongestionDayForYear(requestResult);
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

QString DataBase::intToStrMonth(int month)
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
