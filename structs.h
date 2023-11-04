#ifndef STRUCTS_H
#define STRUCTS_H

#define PROG_VER_MAJOR 1
#define PROG_VER_MINOR 4
#define PROG_VER_MICRO 2

#define POSTGRE_DRIVER "QPSQL"
#define SETTINGS_FILE "settings.ini"
#define GROUP_MAIN_WINDOW "MainWindowSize"
#define GROUP_DB "DataForConnectToDB"

#define DEFAULT_SAVE_FORM_SIZE true
#define DEFAULT_FORM_WIDTH 800
#define DEFAULT_FORM_HEIGHT 600

#define DEFAULT_HOSTNAME "981757-ca08998.tmweb.ru"
#define DEFAULT_DBNAME "demo"
#define DEFAULT_PORT 5432
#define DEFAULT_LOGIN "netology_usr_cpp"
#define DEFAULT_PASS "CppNeto3"
#define DEFAULT_SHOW_PASS false
#define DEFAULT_SAVE_PASS true
#define DEFAULT_AUTOCONNECT true
#define DEFAULT_NUM_OF_CONN_ATTEMPTS 0
#define DEFAULT_TIMEOUT 5

#define NUM_DATA_FOR_APP 3
#define NUM_DATA_FOR_CONNECT_TO_DB 10

enum FieldsAppSettings{
    saveFormSize = 0,
    formWidth = 1,
    formHeight = 2,
};

enum FieldsForConnect{
    hostName = 0,
    dbName = 1,
    port = 2,
    login = 3,
    pass = 4,
    showPass = 5,
    savePass = 6,
    autoConnect = 7,
    numOfConnectionAttempts = 8,
    timeout = 9
};

enum RouteType{
    arrival = 0,
    departure = 1
};

#endif // STRUCTS_H
