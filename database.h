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

    void addDatabaseDriver(QString driver);
    void addDatabaseData(QVector<QString> dataForConnect);
    void connectToDatabase();
    void disconnectFromDatabase();
    void requestToDB(QString request, int requestType);
    QSqlError getLastError(void);
    bool checkDatabase(QVector<QString> dataForConnect);

signals:
   //void sig_SendDataFromDB(const QTableView *tableView, int typeR);
   void sig_SendStatusConnection(bool);

private:
    QSqlDatabase* pDatabase;
};

#endif // DATABASE_H
