#include "kalendarz.h"
#include "ui_kalendarz.h"
#include <QDate>

kalendarz::kalendarz(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::kalendarz)
{
    ui->setupUi(this);
}

kalendarz::~kalendarz()
{
    delete ui;
}

void kalendarz::on_calendarWidget_clicked(const QDate &date)
{
    emit sygnal_daty(date);
}
