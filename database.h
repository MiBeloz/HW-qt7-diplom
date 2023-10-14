#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>

#include "structs.h"

class DataBase : public QObject
{
    Q_OBJECT
public:
    explicit DataBase(QObject *parent = nullptr);
    ~DataBase();

    void addDataBase(QString driver, QString nameDB = "");
    void connectToDataBase(QVector<QString> &dataForConnect);
    void disconnectFromDataBase(QString nameDb = "");
    void requestToDB(QString request, int requestType);
    QSqlError getLastError(void);

signals:
   //void sig_SendDataFromDB(const QTableView *tableView, int typeR);
   void sig_SendStatusConnection(bool);

private:
    QSqlDatabase* pDataBase;
};

#endif // DATABASE_H
