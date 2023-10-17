#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQueryModel>
#include <QComboBox>
#include <QTableView>

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
    void requestToDBListAirports(QString request);
    void requestToDBListIn(QString request, QString arrivalAirport);
    void requestToDBListOut(QString request, QString arrivalAirport);
    QSqlError getLastError(void);
    bool isChange(QVector<QString> dataForConnect);

signals:
   void sig_SendDataFromDB(const QComboBox *pComboBox);
   void sig_SendDataFromDBIn(const QTableView *pTableView);
   void sig_SendDataFromDBOut(const QTableView *pTableView);
   void sig_SendStatusConnection(bool);

private:
    QSqlDatabase* pDatabase;
    QSqlQueryModel *pQueryModelTable;
    QSqlQueryModel *pQueryModelAirports;
    QTableView *pTableView;
    QComboBox *pComboBox;
};

#endif // DATABASE_H
