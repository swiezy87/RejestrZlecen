#include "konfiguracjabazy.h"
#include "ui_konfiguracjabazy.h"
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QCryptographicHash>

konfiguracjaBazy::konfiguracjaBazy(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::konfiguracjaBazy)
{
    ui->setupUi(this);
}

konfiguracjaBazy::~konfiguracjaBazy()
{
    delete ui;
}

void konfiguracjaBazy::on_pushButton_Testuj_clicked()
{

    QString Nazwa_serweraK=ui->lineEdit_Nazwa_serweraK->text();
    QString Nazwa_bazyK=ui->lineEdit_Nazwa_bazyK->text();
    QString LoginK=ui->lineEdit_LoginK->text();
    QString HasloK=ui->lineEdit_HasloK->text();
    QFile plik("c://program1//pliczek.ini");
    plik.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream wpisz(&plik);
                wpisz << Nazwa_serweraK;
                wpisz << "\n";
                wpisz << Nazwa_bazyK;
                wpisz << "\n";
                wpisz << LoginK;
                wpisz << "\n";
                wpisz << HasloK;
    plik.close();
    ui->label_polaczonoK->setText("Zapisano konfigurację bazy");
}
