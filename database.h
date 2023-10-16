#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQueryModel>
#include <QComboBox>
#include <QSqlTableModel>
#include <QTableView>

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
    void requestToDBListIn(QString request);
    QSqlError getLastError(void);
    bool checkDatabase(QVector<QString> dataForConnect);

signals:
   void sig_SendDataFromDB(const QComboBox *pComboBox);
   void sig_SendDataFromDBIn(const QTableView *pTableView);
   void sig_SendStatusConnection(bool);

private:
    QSqlDatabase* pDatabase;
    QSqlQueryModel *pQueryModel;
    QComboBox *pComboBox;
    QSqlTableModel *pTableModel;
    QTableView *pTableView;
};

#endif // DATABASE_H
