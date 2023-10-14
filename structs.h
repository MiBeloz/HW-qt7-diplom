#ifndef STRUCTS_H
#define STRUCTS_H

#define POSTGRE_DRIVER "QPSQL"
#define SETTINGS_FILE "settings.ini"
#define GROUP_MAIN_WINDOW "MainWindowSize"
#define GROUP_DB "DataForConnectToDB"

#define NUM_DATA_FOR_CONNECT_TO_DB 5
#define NUM_DATA_FOR_APP 2

enum fieldsForConnect{
    hostName = 0,
    dbName = 1,
    port = 2,
    login = 3,
    pass = 4
};

enum fieldsAppSettings{
    formWidth = 0,
    formHeight = 1
};

#endif // STRUCTS_H
