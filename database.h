#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>

#define POSTGRE_DRIVER "QPSQL"
#define NUM_DATA_FOR_CONNECT_TO_DB 5

enum fieldsForConnect{
    hostName = 0,
    dbName = 1,
    port = 2,
    login = 3,
    pass = 4
};

class DataBase : public QObject
{
    Q_OBJECT
public:
    explicit DataBase(QObject *parent = nullptr);
    ~DataBase();

    void addDataBase(QString driver, QString nameDB = "");
    void connectToDataBase(QList<QString> &dataForConnect);
    void disconnectFromDataBase(QString nameDb = "");
    void requestToDB(QString request, int requestType);
    QSqlError getLastError(void);

signals:
   //void sig_SendDataFromDB(const QTableView *tableView, int typeR);
   void sig_SendStatusConnection(bool);

private:
    QSqlDatabase* dataBase;
};

#endif // DATABASE_H
