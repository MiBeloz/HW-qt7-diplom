#include "database.h"

DataBase::DataBase(QObject *parent)
    : QObject{parent}
{
    pDatabase = new QSqlDatabase();
    pQueryModel = nullptr;
    pComboBox = nullptr;
    pTableModel = nullptr;
    pTableView = nullptr;
}

DataBase::~DataBase()
{
    delete pTableView;
    delete pTableModel;
    delete pComboBox;
    delete pQueryModel;
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
    if (status){
        pQueryModel = new QSqlQueryModel(nullptr);
        pComboBox = new QComboBox(nullptr);
        pTableModel = new QSqlTableModel(nullptr);
        pTableView = new QTableView(nullptr);
    }

    emit sig_SendStatusConnection(status);
}

void DataBase::disconnectFromDatabase()
{
    delete pTableView;
    delete pTableModel;
    delete pComboBox;
    delete pQueryModel;
    pDatabase->close();
}

void DataBase::requestToDBListAirports(QString request)
{
    pQueryModel->setQuery(request, *pDatabase);

    //pQueryModel->sort(0);
    pComboBox->setModel(pQueryModel);

    emit sig_SendDataFromDB(pComboBox);
}

void DataBase::requestToDBListIn(QString request)
{
//    pTableModel->setTable("bookings.flights");
//    pTableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
//    pTableModel->select();
//    pTableModel->setHeaderData(0, Qt::Horizontal, tr("Номер рейса"));
//    pTableModel->setHeaderData(1, Qt::Horizontal, tr("Время вылета"));
//    pTableModel->setHeaderData(2, Qt::Horizontal, tr("Аэропорт отправления"));

    pQueryModel->setQuery("SELECT flight_no, scheduled_arrival, ad.airport_name->>\'ru\' as \"Name\" "
                          "FROM bookings.flights f "
                          "JOIN bookings.airports_data ad on ad.airport_code = f.departure_airport "
                          "WHERE f.arrival_airport  = \'YKS\'", *pDatabase);

    //pQueryModel->setQuery("SELECT airport_name->>\'ru\' as \"airportName\", airport_code FROM bookings.airports_data", *pDatabase);
    pQueryModel->setHeaderData(0, Qt::Horizontal, tr("Номер рейса"));
    pQueryModel->setHeaderData(1, Qt::Horizontal, tr("Время вылета"));
    pQueryModel->setHeaderData(2, Qt::Horizontal, tr("Аэропорт отправления"));

    pTableView->setModel(pQueryModel);
    pTableView->hideColumn(0);

    emit sig_SendDataFromDBIn(pTableView);
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
