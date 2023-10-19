#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQueryModel>
#include <QComboBox>
#include <QTableView>
#include <QSqlQuery>

#include <QSortFilterProxyModel>

#include "structs.h"

class DataBase : public QObject
{
    Q_OBJECT
public:
    explicit DataBase(QObject *parent = nullptr);
    ~DataBase();

    void addDatabaseDriver(QString driver);
    void addDatabaseData(QVector<QString> dataForConnect);
    void connectToDatabase();
    void disconnectFromDatabase();
    void requestListAirportsToDB();
    void requestListFlightsToDB(QString airportCode, QString requestDate, routeType type);
    void requestStatYear();
    QSqlError getLastError(void);
    bool isChange(QVector<QString> dataForConnect);

signals:
   void sig_SendDataAirportsFromDB(const QComboBox *pComboBox);
   void sig_SendDataFlightsFromDB(const QTableView *pTableView);
   void sig_SendStatusConnection(bool);
   void sig_SendDataStatYear(QMap<QString, QString> result);

private:
    QSqlDatabase* pDatabase;
    QSqlQueryModel *pQueryModelTable;
    QSqlQueryModel *pQueryModelAirports;
    QTableView *pTableView;
    QComboBox *pComboBox;
    QSqlQuery *pSqlQuery;
};

#endif // DATABASE_H
