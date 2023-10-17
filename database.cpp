#include "database.h"

DataBase::DataBase(QObject *parent)
    : QObject{parent}
{
    pDatabase = new QSqlDatabase();
    pQueryModelTable = nullptr;
    pQueryModelAirports = nullptr;
    pTableView = nullptr;
    pComboBox = nullptr;
}

DataBase::~DataBase()
{
    delete pQueryModelTable;
    delete pQueryModelAirports;
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
    bool status = pDatabase->open();

    if (status){
        pQueryModelTable = new QSqlQueryModel(nullptr);
        pQueryModelAirports = new QSqlQueryModel(nullptr);
        pTableView = new QTableView(nullptr);
        pComboBox = new QComboBox(nullptr);
    }
    else{///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        pQueryModelTable = nullptr;
        pQueryModelAirports = nullptr;
        pTableView = nullptr;
        pComboBox = nullptr;
    }

    emit sig_SendStatusConnection(status);
}

void DataBase::disconnectFromDatabase()
{
    delete pQueryModelTable;
    delete pQueryModelAirports;
    delete pTableView;
    delete pComboBox;

    pDatabase->close();
}

void DataBase::requestToDBListAirports(QString request)
{
    pQueryModelAirports->setQuery(request, *pDatabase);

    //pQueryModel->sort(0);
    pComboBox->setModel(pQueryModelAirports);

    emit sig_SendDataFromDB(pComboBox);
}

void DataBase::requestToDBListIn(QString request, QString arrivalAirport)
{
    pQueryModelTable->setQuery("SELECT flight_no, scheduled_arrival, ad.airport_name->>\'ru\' as \"Name\" "
                                "FROM bookings.flights f "
                                "JOIN bookings.airports_data ad on ad.airport_code = f.departure_airport "
                                "WHERE f.arrival_airport  = \'YKS\'"
                                " and scheduled_arrival::date = date(\'2016-12-18\')",
                                //date(\'2016-12-18\')",
                                *pDatabase);

    //pQueryModelTable->setQuery(request + "\'" + arrivalAirport + "\' and ad.airport_date = date(\'2016-12-18\')", *pDatabase);
    //pQueryModelTable->setQuery(request + "\'" + arrivalAirport + "\'", *pDatabase);

    pQueryModelTable->setHeaderData(0, Qt::Horizontal, tr("Номер рейса"));
    pQueryModelTable->setHeaderData(1, Qt::Horizontal, tr("Время вылета"));
    pQueryModelTable->setHeaderData(2, Qt::Horizontal, tr("Аэропорт отправления"));

//    QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel(nullptr);
//    proxyModel->setSourceModel(pQueryModelTable);
//    proxyModel->setFilterKeyColumn(2);
//    proxyModel->setFilterFixedString("Мирный");





    pTableView->setModel(pQueryModelTable);
    pTableView->hideColumn(0);

    emit sig_SendDataFromDBIn(pTableView);
}

void DataBase::requestToDBListOut(QString request, QString departureAirport)
{
    pQueryModelTable->setQuery(request + "\'" + departureAirport + "\'", *pDatabase);

    pQueryModelTable->setHeaderData(0, Qt::Horizontal, tr("Номер рейса"));
    pQueryModelTable->setHeaderData(1, Qt::Horizontal, tr("Время вылета"));
    pQueryModelTable->setHeaderData(2, Qt::Horizontal, tr("Аэропорт назначения"));

    pTableView->setModel(pQueryModelTable);
    pTableView->hideColumn(0);

    emit sig_SendDataFromDBIn(pTableView);
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
