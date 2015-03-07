#include "konfig_czynnosci.h"
#include "ui_konfig_czynnosci.h"
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QMessageBox>

konfig_czynnosci::konfig_czynnosci(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::konfig_czynnosci)
{
    ui->setupUi(this);
    QRegExp rx("[A-ząężźółćśńĄĘŻŹÓŁĆŚŃ(),0123456789/\- ]+");
    QRegExpValidator  *valid = new QRegExpValidator(rx,this);
    ui->lineEdit_opis->setValidator(valid);
    ui->lineEdit_idc->setEnabled(false);
    ui->lineEdit_punkty->setValidator( new QIntValidator(0,1000,this));
    ui->lineEdit_punkty->setMaxLength(4);
    ui->lineEdit_opis->setMaxLength(100);
    odswiez_tabele();
}

konfig_czynnosci::~konfig_czynnosci()
{
    delete ui;
}
void konfig_czynnosci::odswiez_tabele()
{
    QSqlQueryModel *model = new QSqlQueryModel;
    QString comboboxwyszukaj=ui->comboBox_szukaj->currentText();
    QString lineeditwyszukaj=ui->lineEdit_szukaj->text();
    if(comboboxwyszukaj!="")
         model->setQuery("SELECT idc, opis, punkty, blokada  FROM czynnosc where "+comboboxwyszukaj+" like '%"+lineeditwyszukaj+"%'");
         else
         model->setQuery("SELECT idc, opis, punkty, blokada  FROM czynnosc");
         model->setHeaderData(0, Qt::Horizontal, tr("ID"));
         model->setHeaderData(1, Qt::Horizontal, tr("opis"));
         model->setHeaderData(2, Qt::Horizontal, tr("punkty"));
         model->setHeaderData(3, Qt::Horizontal, tr("blokada"));
         ui->tableView_czynnosci->setModel(model);
         ui->tableView_czynnosci->resizeColumnsToContents();
}

void konfig_czynnosci::on_pushButton_szukaj_clicked()
{
    odswiez_tabele();
}

void konfig_czynnosci::on_pushButton_dodaj_clicked()
{
    QString opis=ui->lineEdit_opis->text();
    QString punkty=ui->lineEdit_punkty->text();
    QString blokada=ui->comboBox_blokuj->currentText();
    QString opis_z_bazy;

    QSqlQuery model_select;
    model_select.prepare("SELECT opis FROM czynnosc where opis='"+opis+"' ");
    if(model_select.exec())
        while(model_select.next())
        {
            opis_z_bazy=model_select.value(0).toString();
        }
    if(opis_z_bazy==opis)
         {
             QMessageBox::information(this,tr("Błąd"),tr("taka czynność już istnieje"));
         }
    else
    {
    QSqlQuery selectinsert;
    selectinsert.exec("INSERT INTO czynnosc (opis, punkty, blokada) VALUES ('"+opis+"', '"+punkty+"','"+blokada+"')");
    }
    odswiez_tabele();
}

void konfig_czynnosci::on_tableView_czynnosci_activated(const QModelIndex &index)
{
    QString val = (ui->tableView_czynnosci->model()->data(ui->tableView_czynnosci->model()->index(ui->tableView_czynnosci->currentIndex().row(), 0))).toString();
    //QString val=ui->tableView_czynnosci->model()->data(index).toString();
    QSqlQuery model_select;
    //model_select.prepare("SELECT idc, opis, punkty, blokada FROM czynnosc where idc='"+val+"' or opis='"+val+"' or punkty='"+val+"' or blokada='"+val+"' ");
    model_select.prepare("SELECT idc, opis, punkty, blokada FROM czynnosc where idc='"+val+"' ");
    if(model_select.exec())
        while(model_select.next())
        {
            ui->lineEdit_idc->setText(model_select.value(0).toString());
            ui->lineEdit_opis->setText(model_select.value(1).toString());
            ui->lineEdit_punkty->setText(model_select.value(2).toString());
            ui->comboBox_blokuj->setCurrentText(model_select.value(3).toString());
        }
}

void konfig_czynnosci::on_pushButton_edytuj_clicked()
{
    QString idc=ui->lineEdit_idc->text();
    QString opis=ui->lineEdit_opis->text();
    QString punkty=ui->lineEdit_punkty->text();
    QString blokada=ui->comboBox_blokuj->currentText();
    if(idc=="")
        QMessageBox::information(this,tr("Błąd"),tr("Wybierz pracownika"));
    else
    {
    QSqlQuery selectinsert;
    selectinsert.exec("UPDATE czynnosc SET opis='"+opis+"', punkty='"+punkty+"', blokada='"+blokada+"' WHERE idc='"+idc+"'");
    odswiez_tabele();
    }
}

void konfig_czynnosci::on_lineEdit_szukaj_textChanged(const QString &arg1)
{
    odswiez_tabele();
}
