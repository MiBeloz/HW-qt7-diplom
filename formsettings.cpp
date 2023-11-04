#include "formsettings.h"
#include "ui_formsettings.h"

FormSettings::FormSettings(QWidget *parent) :
    QDialog(parent, Qt::WindowTitleHint),
    ui(new Ui::FormSettings)
{
    ui->setupUi(this);

    QObject::connect(ui->chB_saveMainWindowSize, &QCheckBox::clicked, this, [&](bool checked){
        if (checked){
            ui->grB_mainWindowSize->setEnabled(false);
        }
        else{
            ui->grB_mainWindowSize->setEnabled(true);
        }
    });
    QObject::connect(ui->chB_showPass, &QCheckBox::clicked, this, [&](bool checked){
        if (checked){
            ui->le_pass->setEchoMode(QLineEdit::Normal);
        }
        else{
            ui->le_pass->setEchoMode(QLineEdit::Password);
        }
    });
    QObject::connect(ui->chB_autoConnect, &QCheckBox::clicked, this, [&](bool checked){
        if (checked){
            ui->lb_numOfConnectionAttempts->setEnabled(true);
            ui->spB_numOfConnectionAttempts->setEnabled(true);
            ui->lb_timeout->setEnabled(true);
            ui->spB_timeout->setEnabled(true);
        }
        else{
            ui->lb_numOfConnectionAttempts->setEnabled(false);
            ui->spB_numOfConnectionAttempts->setEnabled(false);
            ui->lb_timeout->setEnabled(false);
            ui->spB_timeout->setEnabled(false);
        }
    });
    QObject::connect(ui->chB_saveMainWindowSize, &QCheckBox::stateChanged, this, [&]{
        ui->pb_save->setEnabled(true);
    });
    QObject::connect(ui->spB_mainWindowWidth, &QSpinBox::textChanged, this, [&]{
        ui->pb_save->setEnabled(true);
    });
    QObject::connect(ui->spB_mainWindowHeight, &QSpinBox::textChanged, this, [&]{
        ui->pb_save->setEnabled(true);
    });
    QObject::connect(ui->le_hostname, &QLineEdit::textChanged, this, [&]{
        ui->pb_save->setEnabled(true);
    });
    QObject::connect(ui->le_dbName, &QLineEdit::textChanged, this, [&]{
        ui->pb_save->setEnabled(true);
    });
    QObject::connect(ui->spB_port, &QSpinBox::textChanged, this, [&]{
        ui->pb_save->setEnabled(true);
    });
    QObject::connect(ui->le_login, &QLineEdit::textChanged, this, [&]{
        ui->pb_save->setEnabled(true);
    });
    QObject::connect(ui->le_pass, &QLineEdit::textChanged, this, [&]{
        ui->pb_save->setEnabled(true);
    });
    QObject::connect(ui->chB_showPass, &QCheckBox::stateChanged, this, [&]{
        ui->pb_save->setEnabled(true);
    });
    QObject::connect(ui->chB_savePass, &QCheckBox::stateChanged, this, [&]{
        ui->pb_save->setEnabled(true);
    });
    QObject::connect(ui->chB_autoConnect, &QCheckBox::stateChanged, this, [&]{
        ui->pb_save->setEnabled(true);
    });
    QObject::connect(ui->spB_numOfConnectionAttempts, &QSpinBox::textChanged, this, [&]{
        ui->pb_save->setEnabled(true);
    });
    QObject::connect(ui->spB_timeout, &QSpinBox::textChanged, this, [&]{
        ui->pb_save->setEnabled(true);
    });
}

FormSettings::~FormSettings()
{
    delete ui;
}

void FormSettings::rec_sendSettings(QVector<QString> appSettings, QVector<QString> dbSettings)
{
    if (appSettings[saveFormSize] == "true"){
        ui->chB_saveMainWindowSize->setChecked(true);
        ui->grB_mainWindowSize->setEnabled(false);
    }
    else{
        ui->chB_saveMainWindowSize->setChecked(false);
        ui->grB_mainWindowSize->setEnabled(true);
    }

    ui->spB_mainWindowWidth->setValue(appSettings[formWidth].toInt());
    ui->spB_mainWindowHeight->setValue(appSettings[formHeight].toInt());

    ui->le_hostname->setText(dbSettings[hostName]);
    ui->le_dbName->setText(dbSettings[dbName]);
    ui->spB_port->setValue(dbSettings[port].toInt());
    ui->le_login->setText(dbSettings[login]);
    ui->le_pass->setText(dbSettings[pass]);

    if (dbSettings[showPass] == "true"){
        ui->chB_showPass->setChecked(true);
        ui->le_pass->setEchoMode(QLineEdit::Normal);
    }
    else{
        ui->chB_showPass->setChecked(false);
        ui->le_pass->setEchoMode(QLineEdit::Password);
    }

    if (dbSettings[savePass] == "true"){
        ui->chB_savePass->setChecked(true);
    }
    else{
        ui->chB_savePass->setChecked(false);
    }

    if (dbSettings[autoConnect] == "true"){
        ui->chB_autoConnect->setChecked(true);
    }
    else{
        ui->chB_autoConnect->setChecked(false);
    }

    if (ui->chB_autoConnect->isChecked()){
        ui->lb_numOfConnectionAttempts->setEnabled(true);
        ui->spB_numOfConnectionAttempts->setEnabled(true);
        ui->lb_timeout->setEnabled(true);
        ui->spB_timeout->setEnabled(true);
    }
    else{
        ui->lb_numOfConnectionAttempts->setEnabled(false);
        ui->spB_numOfConnectionAttempts->setEnabled(false);
        ui->lb_timeout->setEnabled(false);
        ui->spB_timeout->setEnabled(false);
    }

    ui->spB_numOfConnectionAttempts->setValue(dbSettings[numOfConnectionAttempts].toInt());
    ui->spB_timeout->setValue(dbSettings[timeout].toInt());

    ui->pb_save->setEnabled(false);
}

void FormSettings::on_pb_save_clicked()
{
    QVector<QString> appSettings(NUM_DATA_FOR_APP);
    QVector<QString> dbSettings(NUM_DATA_FOR_CONNECT_TO_DB);

    if (ui->chB_saveMainWindowSize->isChecked()){
        appSettings[saveFormSize] = "true";
    }
    else{
        appSettings[saveFormSize] = "false";
    }

    appSettings[formWidth] = ui->spB_mainWindowWidth->text();
    appSettings[formHeight] = ui->spB_mainWindowHeight->text();

    dbSettings[hostName] = ui->le_hostname->text();
    dbSettings[dbName] = ui->le_dbName->text();
    dbSettings[port] = ui->spB_port->text();
    dbSettings[login] = ui->le_login->text();
    dbSettings[pass] = ui->le_pass->text();

    if (ui->chB_showPass->isChecked()){
        dbSettings[showPass] = "true";
    }
    else{
        dbSettings[showPass] = "false";
    }

    if (ui->chB_savePass->isChecked()){
        dbSettings[savePass] = "true";
    }
    else{
        dbSettings[savePass] = "false";
    }

    if (ui->chB_autoConnect->isChecked()){
        dbSettings[autoConnect] = "true";
    }
    else{
        dbSettings[autoConnect] = "false";
    }

    dbSettings[numOfConnectionAttempts] = ui->spB_numOfConnectionAttempts->text();
    dbSettings[timeout] = ui->spB_timeout->text();

    ui->pb_save->setEnabled(false);

    emit sig_saveSettings(appSettings, dbSettings);
}


void FormSettings::on_pb_cancel_clicked()
{
    close();
}


void FormSettings::on_pb_default_clicked()
{
    ui->chB_saveMainWindowSize->setChecked(DEFAULT_SAVE_FORM_SIZE);
    ui->spB_mainWindowWidth->setValue(DEFAULT_FORM_WIDTH);
    ui->spB_mainWindowHeight->setValue(DEFAULT_FORM_HEIGHT);
    ui->le_hostname->setText(DEFAULT_HOSTNAME);
    ui->le_dbName->setText(DEFAULT_DBNAME);
    ui->spB_port->setValue(DEFAULT_PORT);
    ui->le_login->setText(DEFAULT_LOGIN);
    ui->le_pass->setText(DEFAULT_PASS);
    ui->chB_showPass->setChecked(DEFAULT_SHOW_PASS);
    ui->chB_savePass->setChecked(DEFAULT_SAVE_PASS);

    if (ui->chB_showPass->isChecked()){
        ui->le_pass->setEchoMode(QLineEdit::Normal);
    }
    else{
        ui->le_pass->setEchoMode(QLineEdit::Password);
    }

    ui->chB_autoConnect->setChecked(DEFAULT_AUTOCONNECT);
    ui->spB_numOfConnectionAttempts->setValue(DEFAULT_NUM_OF_CONN_ATTEMPTS);
    ui->spB_timeout->setValue(DEFAULT_TIMEOUT);

    if (ui->chB_autoConnect->isChecked()){
        ui->lb_numOfConnectionAttempts->setEnabled(true);
        ui->spB_numOfConnectionAttempts->setEnabled(true);
        ui->lb_timeout->setEnabled(true);
        ui->spB_timeout->setEnabled(true);
    }
    else{
        ui->lb_numOfConnectionAttempts->setEnabled(false);
        ui->spB_numOfConnectionAttempts->setEnabled(false);
        ui->lb_timeout->setEnabled(false);
        ui->spB_timeout->setEnabled(false);
    }
}

