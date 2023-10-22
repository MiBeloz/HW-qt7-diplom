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
    void requestListFlights(QString airportCode, QString requestDate, routeType type);
    void requestCongestionYear(QString airportCode);
    void requestCongestionDayForYear(QString airportCode);
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

    QSqlQueryModel *pQueryModelTable;
    QSqlQueryModel *pQueryModelAirports;

    QSqlQuery *pSqlQueryStatYear;
    QSqlQuery *pSqlQueryStatDaysForYear;

    QTableView *pTableView;
    QComboBox *pComboBox;

    QFuture<void> ftrQueryModelAirports;
    QFutureWatcher<void> ftrWtchQueryModelAirports;
    QFuture<void> ftrQueryModelTable;
    QFutureWatcher<void> ftrWtchQueryModelTable;

    QFuture<void> ftrSqlQueryStatYear;
    QFutureWatcher<void> ftrWtchSqlQueryStatYear;

    QString intToStrMonth(int month);
};

#endif // DATABASE_H
