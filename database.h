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

class Database : public QObject
{
    Q_OBJECT
public:
    explicit Database(QObject *parent = nullptr);
    ~Database();

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
   void sig_sendDataAirports(const QComboBox *pComboBox);
   void sig_sendDataFlights(const QTableView *pTableView);
   void sig_sendStatusConnection(bool);
   void sig_sendCongestionYear(QVector<QPair<QString, QString>> requestResult);
   void sig_sendCongestionDayForYear(QVector<QPair<QString, QString>> requestResult);
   void sig_sendStatusRequestCongestion(QSqlError err);

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
