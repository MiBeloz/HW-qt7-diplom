#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQueryModel>
#include <QComboBox>
#include <QTableView>
#include <QSqlQuery>
#include <QtConcurrent>

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
    void requestListAirports();
    void requestListFlights(QString airportCode, QString requestDate, RouteType type);
    void requestCongestion(QString airportCode);
    QSqlError getLastError(void);
    bool isChange(QVector<QString> dataForConnect);

signals:
   void sig_SendDataAirports(const QComboBox *pComboBox);
   void sig_SendDataFlights(const QTableView *pTableView);
   void sig_SendStatusConnection(bool);
   void sig_SendCongestionYear(QVector<QPair<QString, QString>> requestResult);
   void sig_SendCongestionDayForYear(QVector<QPair<QString, QString>> requestResult);

private:
    QSqlDatabase* pDatabase;

    QSqlQueryModel *pQueryModelAirports;
    QFuture<void> ftrQueryModelAirports;
    QFutureWatcher<void> ftrWtchQueryModelAirports;

    QSqlQueryModel *pQueryModelTable;
    QFuture<void> ftrQueryModelTable;
    QFutureWatcher<void> ftrWtchQueryModelTable;

    QSqlQuery *pSqlQuery;
    QFuture<void> ftrSqlQueryYear;
    QFutureWatcher<void> ftrWtchSqlQueryYear;
    QFuture<void> ftrSqlQueryDayForYear;
    QFutureWatcher<void> ftrWtchSqlQueryDayForYear;
    QString m_airportCode;

    QTableView *pTableView;
    QComboBox *pComboBox;

    QString intToStrMonth(int month);
};

#endif // DATABASE_H
