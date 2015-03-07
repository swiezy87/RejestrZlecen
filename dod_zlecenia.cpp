#include "dod_zlecenia.h"
#include "ui_dod_zlecenia.h"
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QMessageBox>

dod_zlecenia::dod_zlecenia(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dod_zlecenia)
{
    ui->setupUi(this);
}

dod_zlecenia::~dod_zlecenia()
{
    delete ui;

}


void dod_zlecenia::odswiez_tabele_czynnosci()
{
    QSqlQueryModel *model = new QSqlQueryModel;
    QString comboboxwyszukaj=ui->comboBox_szukaj->currentText();
    QString lineeditwyszukaj=ui->lineEdit_szukaj->text();
    if(comboboxwyszukaj!="")
         model->setQuery("SELECT idc, opis, punkty FROM czynnosc where "+comboboxwyszukaj+" like '%"+lineeditwyszukaj+"%' ");
         else
         model->setQuery("SELECT idc, opis, punkty FROM czynnosc");
         model->setHeaderData(0, Qt::Horizontal, tr("ID"));
         model->setHeaderData(1, Qt::Horizontal, tr("opis"));
         model->setHeaderData(2, Qt::Horizontal, tr("punkty"));
         ui->tableView_czynnosci->setModel(model);
         ui->tableView_czynnosci->resizeColumnsToContents();
         odswiez_tabele_czynnosci_wykonane();
}
void dod_zlecenia::odswiez_tabele_czynnosci_wykonane()
{
         QSqlQueryModel *model = new QSqlQueryModel;
         QString comboboxwyszukaj=ui->comboBox_szukaj_wykonane->currentText();
         QString lineeditwyszukaj=ui->lineEdit_szukaj_wykonane->text();
         if(comboboxwyszukaj!="")
              model->setQuery("SELECT w.idw, c.opis, c.punkty, w.krotnosc, p.imie, p.nazwisko from czynnosc c JOIN wykonanie w on w.fk_idc=c.idc JOIN pracownik p on w.fk_idp=p.idp where "+comboboxwyszukaj+" like '%"+lineeditwyszukaj+"%' and fk_idz='"+idzzmainwindow+"'");
         else
         model->setQuery("SELECT w.idw, c.opis, c.punkty, w.krotnosc, p.imie, p.nazwisko from czynnosc c JOIN wykonanie w on w.fk_idc=c.idc JOIN pracownik p on w.fk_idp=p.idp where fk_idz='"+idzzmainwindow+"'");
         model->setHeaderData(0, Qt::Horizontal, tr("idw"));
         model->setHeaderData(1, Qt::Horizontal, tr("opis"));
         model->setHeaderData(2, Qt::Horizontal, tr("punkty"));
         model->setHeaderData(3, Qt::Horizontal, tr("krotnosc"));
         ui->tableView_czynnosci_wykonane->setModel(model);
         ui->tableView_czynnosci_wykonane->resizeColumnsToContents();
         //ui->tableView_czynnosci_wykonane->resizeColumnToContents(1);
}

void dod_zlecenia::on_tableView_czynnosci_clicked(const QModelIndex &index)
{
    QString idw;
    QString oldidc;
    QString krotnosc;
    QString idp;
    int intkrotnosc;
    //pobieram idc
    QString val = (ui->tableView_czynnosci->model()->data(ui->tableView_czynnosci->model()->index(ui->tableView_czynnosci->currentIndex().row(), 0))).toString();
    QSqlQuery select("SELECT idw, fk_idc, krotnosc, fk_idp FROM wykonanie where fk_idc='"+val+"' and fk_idz='"+idzzmainwindow+"' and fk_idp='"+idpzmainwindow+"'");
    while(select.next())
    {
        idw=(select.value(0).toString());
        oldidc=(select.value(1).toString());
        intkrotnosc=(select.value(2).toInt());
        idp=(select.value(3).toString());
    }
    if(oldidc==val and idp==idpzmainwindow)
    {
       // ui->lineEdit_szukaj->setText();
        intkrotnosc++;
        krotnosc.append(QString("%1").arg(intkrotnosc));
        QSqlQuery update;
        update.exec("UPDATE wykonanie SET krotnosc='"+krotnosc+"' WHERE idw='"+idw+"' ");
    }
    else
    {
    QSqlQuery insert;
    insert.exec("INSERT INTO wykonanie (fk_idz, fk_idc, fk_idp, krotnosc) VALUES ('"+idzzmainwindow+"', '"+val+"','"+idpzmainwindow+"', '1')");
    }
    odswiez_tabele_czynnosci_wykonane();
}
void dod_zlecenia::on_tableView_czynnosci_wykonane_clicked(const QModelIndex &index)
{
    QString idw;
    QString opis;
    QString krotnosc;
    QString idp;
    int intkrotnosc;
    QString val = (ui->tableView_czynnosci_wykonane->model()->data(ui->tableView_czynnosci_wykonane->model()->index(ui->tableView_czynnosci_wykonane->currentIndex().row(), 0))).toString();
    QSqlQuery select("SELECT idw, krotnosc, fk_idp FROM wykonanie where idw='"+val+"' and fk_idz='"+idzzmainwindow+"' ");
    while(select.next())
    {
        idw=(select.value(0).toString());
        intkrotnosc=(select.value(1).toInt());
        idp=(select.value(2).toString());

    }
    if(intkrotnosc>1 and idp==idpzmainwindow)
    {
        intkrotnosc--;
        krotnosc.append(QString("%1").arg(intkrotnosc));
        QSqlQuery update;
        update.exec("UPDATE wykonanie SET krotnosc='"+krotnosc+"' WHERE idw='"+idw+"' ");
    }
    else
        if(idp==idpzmainwindow)
         {
            QSqlQuery del;
            del.exec("DELETE FROM `wykonanie` WHERE idw='"+idw+"'");
         }
    odswiez_tabele_czynnosci_wykonane();
}
void dod_zlecenia::on_pushButton_szukaj_clicked()
{
    odswiez_tabele_czynnosci();
}
void dod_zlecenia::on_pushButton_szukaj_wykonane_clicked()
{
    odswiez_tabele_czynnosci_wykonane();
}
void dod_zlecenia::setidz(QString idz)
{
  idzzmainwindow=idz;
  odswiez_tabele_czynnosci();
  odswiez_tabele_czynnosci_wykonane();
}
void dod_zlecenia::setidp(QString idp)
{
   idpzmainwindow=idp;
}


void dod_zlecenia::on_lineEdit_szukaj_textChanged(const QString &arg1)
{
    odswiez_tabele_czynnosci();
}
