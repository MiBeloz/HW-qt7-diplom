#include "about.h"
#include "ui_about.h"

About::About(QWidget *parent) :
    QDialog(parent, Qt::WindowTitleHint),
    ui(new Ui::About)
{
    ui->setupUi(this);

    QPixmap pixmap(":/img/airplane.png");
    pixmap = pixmap.scaled(128, 128);
    ui->lb_pixmap->setPixmap(pixmap);

    ui->lb_text->setText("\tИнспектор Аэропортов\n"
                          "\tv" + QString::number(PROG_VER_MAJOR) + '.' + QString::number(PROG_VER_MINOR) + '.' + QString::number(PROG_VER_MICRO) + "\n"
                                                                                                      "\n"
                                                                                                      "\tКурсовая работа"
                                                                                                      "\n"
                                                                                                      "\n"
                                                                                                      "\tБелозеров Михаил");
}

About::~About()
{
    delete ui;
}

void About::on_pb_ok_clicked()
{
    close();
}

