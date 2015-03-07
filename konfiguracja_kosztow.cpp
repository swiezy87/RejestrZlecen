#include "konfiguracja_kosztow.h"
#include "ui_konfiguracja_kosztow.h"
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QMessageBox>

Konfiguracja_Kosztow::Konfiguracja_Kosztow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Konfiguracja_Kosztow)
{
    ui->setupUi(this);
    odswiez_tabele();
    QRegExp rx("[A-ząężźółćśńĄĘŻŹÓŁĆŚŃ(), ]+");
    QRegExpValidator  *valid = new QRegExpValidator(rx,this);
    ui->lineEdit_opis->setValidator( valid);
    ui->lineEdit_opis->setMaxLength(120);
    ui->lineEdit_ido->setEnabled(false);
    ui->lineEdit_o1->setValidator( new QIntValidator(0,6000000,this));
}

Konfiguracja_Kosztow::~Konfiguracja_Kosztow()
{
    delete ui;
}

void Konfiguracja_Kosztow::odswiez_tabele()
{
    QSqlQueryModel *model = new QSqlQueryModel;
    QString comboboxwyszukaj=ui->comboBox_szukaj->currentText();
    QString lineeditwyszukaj=ui->lineEdit_szukaj->text();
    if(comboboxwyszukaj!="")
         model->setQuery("SELECT ido, dzial, numer, blokada FROM osrodek where "+comboboxwyszukaj+" like '%"+lineeditwyszukaj+"%'");
         else
         model->setQuery("SELECT ido, dzial, numer, blokada FROM osrodek");
         model->setHeaderData(0, Qt::Horizontal, tr("IDO"));
         model->setHeaderData(1, Qt::Horizontal, tr("dział"));
         model->setHeaderData(2, Qt::Horizontal, tr("numer"));
         model->setHeaderData(3, Qt::Horizontal, tr("blokada"));
         ui->tableView_osrodki->setModel(model);
         ui->tableView_osrodki->setAlternatingRowColors(true);
         ui->tableView_osrodki->resizeColumnsToContents();
}



void Konfiguracja_Kosztow::on_pushButton_dodaj_clicked()
{
    QString dzial=ui->lineEdit_opis->text();
    QString numer=ui->lineEdit_o1->text();
    QString blokada=ui->comboBox_blokada->currentText();
    QString numer_z_bazy;

    QSqlQuery model_select;
    model_select.prepare("SELECT numer FROM osrodek where numer='"+numer+"' ");
    if(model_select.exec())
        while(model_select.next())
        {
            numer_z_bazy=model_select.value(0).toString();
        }
    if(numer_z_bazy==numer)
         {
             QMessageBox::information(this,tr("Błąd"),tr("taki numer ośrodka już istnieje"));
         }
    else
    {
    QSqlQuery selectinsert;
    selectinsert.exec("INSERT INTO osrodek (dzial, numer, blokada) VALUES ('"+dzial+"', '"+numer+"','"+blokada+"')");
    }
    odswiez_tabele();

}

void Konfiguracja_Kosztow::on_tableView_osrodki_activated(const QModelIndex &index)
{
   QString val = (ui->tableView_osrodki->model()->data(ui->tableView_osrodki->model()->index(ui->tableView_osrodki->currentIndex().row(), 0))).toString();
   QSqlQuery model_select;
   model_select.prepare("SELECT ido, dzial, numer, blokada FROM osrodek where ido='"+val+"' ");
   if(model_select.exec())
       while(model_select.next())
       {
           ui->lineEdit_ido->setText(model_select.value(0).toString());
           ui->lineEdit_opis->setText(model_select.value(1).toString());
           ui->lineEdit_o1->setText(model_select.value(2).toString());
           ui->comboBox_blokada->setCurrentText(model_select.value(3).toString());
       }
}

void Konfiguracja_Kosztow::on_pushButton_edytuj_clicked()
{
    QString ido=ui->lineEdit_ido->text();
    QString dzial=ui->lineEdit_opis->text();
    QString numer=ui->lineEdit_o1->text();
    QString blokada=ui->comboBox_blokada->currentText();

    QSqlQuery selectinsert;
    selectinsert.exec("UPDATE osrodek SET dzial='"+dzial+"', numer='"+numer+"', blokada='"+blokada+"' WHERE ido='"+ido+"' ");
    odswiez_tabele();
}

void Konfiguracja_Kosztow::on_pushButton_szukaj_clicked()
{
    odswiez_tabele();
}

void Konfiguracja_Kosztow::on_lineEdit_szukaj_textChanged(const QString &arg1)
{
    odswiez_tabele();
}
