#include "raporty.h"
#include "ui_raporty.h"
#include <qlistwidget.h>
#include <qmessagebox.h>
#include <QDebug>
#include <konfiguracjabazy.h>
#include <konfig_uzytkownika.h>
#include <konfiguracja_kosztow.h>
#include <konfig_czynnosci.h>
#include <dod_zlecenia.h>
#include <QDateTime>
#include <qdatetime.h>
#include <QSizePolicy>
#include <QDate>
#include <raporty.h>
#include <kalendarz.h>
#include <QTextDocument>
#include <QPrintDialog>
#include <QPrinter>
#include <QFileDialog>
#include <QtWebKitWidgets/QWebView>

raporty::raporty(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::raporty)
{  
    ui->setupUi(this);
    ui->pushButton_druk2->setDisabled(true);
    ui->dateEdit_do->setDate(QDate::currentDate());
    odswiez_osrodki_punkty();
    ui->pushButton_druk->setToolTip("drukowanie");
}

raporty::~raporty()
{
    delete ui;
}
void raporty::odswiez_osrodki_punkty()
{
    QString data_od=ui->dateEdit_od->date().toString("yyyy-MM-dd");

    QString data_do=ui->dateEdit_do->date().toString("yyyy-MM-dd");

    QSqlQueryModel *model = new QSqlQueryModel;
         model->setQuery(" SELECT o.ido, o.numer, o.dzial, SUM(c.punkty * w.krotnosc) punkty FROM osrodek o JOIN zgloszenie z ON z.fk_ido = o.ido JOIN wykonanie w ON w.fk_idz = z.idz JOIN czynnosc c ON c.idc = w.fk_idc where z.data_zgloszenia >= '"+data_od+"' and z.data_zgloszenia <= '"+data_do+"' GROUP BY o.dzial ");
         model->setHeaderData(1, Qt::Horizontal, tr("numer"));
         model->setHeaderData(2, Qt::Horizontal, tr("dzial"));
         model->setHeaderData(3, Qt::Horizontal, tr("punkty"));
         ui->tableView_osrodki_punkty->setModel(model);
         ui->tableView_osrodki_punkty->setColumnWidth(0,0);
         ui->tableView_osrodki_punkty->resizeColumnsToContents();
   QSqlQuery suma;
         suma.prepare(" SELECT SUM(c.punkty * w.krotnosc) punkty FROM osrodek o JOIN zgloszenie z ON z.fk_ido = o.ido JOIN wykonanie w ON w.fk_idz = z.idz JOIN czynnosc c ON c.idc = w.fk_idc where z.data_zgloszenia >= '"+data_od+"' and z.data_zgloszenia <= '"+data_do+"' ");
         if(suma.exec())
             while(suma.next())
             {
                 ui->lineEdit_suma_punktow->setText(suma.value(0).toString());
             }
}

void raporty::on_toolButton_data_od_clicked()
{
    nowykalendarz = new kalendarz;
    nowykalendarz->setModal(true);
    nowykalendarz->show();
    connect(nowykalendarz, SIGNAL(sygnal_daty(QDate)),this, SLOT(ustaw_date_od_kalendar(QDate)));
}

void raporty::on_toolButton_data_do_clicked()
{
    nowykalendarz = new kalendarz;
    nowykalendarz->setModal(true);
    nowykalendarz->show();
    connect(nowykalendarz, SIGNAL(sygnal_daty(QDate)),this, SLOT(ustaw_date_do_kalendar(QDate)));
}
void raporty::ustaw_date_od_kalendar(QDate data_od)
{
       ui->dateEdit_od->setDate(data_od);
       odswiez_osrodki_punkty();
}
void raporty::ustaw_date_do_kalendar(QDate data_do)
{
       ui->dateEdit_do->setDate(data_do);
       odswiez_osrodki_punkty();
}
void raporty::on_tableView_osrodki_punkty_clicked(const QModelIndex &index)
{
    ido = (ui->tableView_osrodki_punkty->model()->data(ui->tableView_osrodki_punkty->model()->index(ui->tableView_osrodki_punkty->currentIndex().row(), 0))).toString();
    numer = (ui->tableView_osrodki_punkty->model()->data(ui->tableView_osrodki_punkty->model()->index(ui->tableView_osrodki_punkty->currentIndex().row(), 1))).toString();
    QString opis = (ui->tableView_osrodki_punkty->model()->data(ui->tableView_osrodki_punkty->model()->index(ui->tableView_osrodki_punkty->currentIndex().row(), 2))).toString();
    numer.append(" ");
    ui->lineEdit_wybor->setText(numer.append(opis));
    ui->pushButton_druk2->setDisabled(false);
}

void raporty::print()
{
    QString data_od=ui->dateEdit_od->date().toString("yyyy-MM-dd");
    QString data_do=ui->dateEdit_do->date().toString("yyyy-MM-dd");

    QSqlQueryModel *model = new QSqlQueryModel;
      model->setQuery(" SELECT o.numer, o.dzial, SUM(c.punkty * w.krotnosc) punkty FROM osrodek o JOIN zgloszenie z ON z.fk_ido = o.ido JOIN wykonanie w ON w.fk_idz = z.idz JOIN czynnosc c ON c.idc = w.fk_idc where z.data_zgloszenia >= '"+data_od+"' and z.data_zgloszenia <= '"+data_do+"' GROUP BY o.dzial ");
         QString html;
     html = "<html><body><table border=\"1\">";
     html += " <div align=\"center\">Raport punktowy w podziale na ośrodki za okres od "+ data_od +" do "+ data_do +"";
     html += "<td>Numer ośrodka</td>";
     html += "<td>Nazwa ośrodka</td>";
     html += "<td>Suma punktów</td>";
     html += "<td>Podpis</td>";
        for(int row = 0; row < model->rowCount(); row++)
        {
            html += "<tr>";
            for(int column = 0; column < model->columnCount(); column++)
            {
                 QString data = model->data(model->index(row, column), Qt::DisplayRole).toString();
                 html += "<td>" + data + "</td>";
            }
            html += "<td width=\"300\" height=\"50\"></td>";
            html += "</tr>";
        }
        html += "<td> </td>";
        html += "<td>Suma punktów</td>";
        QString suma=ui->lineEdit_suma_punktow->text();
        html += "<td>" + suma + "</td>";
        html += "</table></body></html>";
QTextDocument *document = new QTextDocument();
document->setHtml(html);

QWebView *podgladpliku = new QWebView();
podgladpliku->setHtml(html);

QPrinter printer(QPrinter::HighResolution);
    printer.setPageSize(QPrinter::A4);
    printer.setOutputFormat(QPrinter::PdfFormat);
    QFileDialog *fileDialog = new QFileDialog;
    fileDialog->setDefaultSuffix("pdf");
    printer.setOutputFileName(fileDialog->getSaveFileName(this,tr("Zapisz raport"), "Raport punktowy w podziale na ośrodki za okres od "+ data_od +" do "+ data_do +"", tr("(*.pdf)")));
        podgladpliku->print(&printer);
        //document->print(&printer);
        delete document;
}
void raporty::print2()
{
    QString data_od=ui->dateEdit_od->date().toString("yyyy-MM-dd");
    QString data_do=ui->dateEdit_do->date().toString("yyyy-MM-dd");

    QSqlQueryModel *model = new QSqlQueryModel;
    QSqlQueryModel *modelczynnosci = new QSqlQueryModel;
    model->setQuery(" SELECT z.idz, z.data_zgloszenia, z.data_zakonczenia, z.opis_usterki, z.opis_zamkniecia FROM zgloszenie z JOIN osrodek o on z.fk_ido=o.ido where z.data_zgloszenia >= '"+data_od+"' and z.data_zgloszenia <= '"+data_do+"' and o.ido='"+ido+"' ");

     QString html;
     QString idzraportowane;
     html = "<html><body><table border=\"1\">";
     html += "<div align=\"center\">Raport z ośrodka "+ numer +" za okres od "+ data_od +" do "+ data_do +"";
     html += "<td>id zgłoszenia</td>";
     html += "<td>data zgłoszenia</td>";
     html += "<td>data zakończenia</td>";
     html += "<td>opis zgłoszenia</td>";
     html += "<td>opis zakończenia</td>";
        for(int row = 0; row < model->rowCount(); row++)
        {
            html += "<tr>";
            for(int column = 0; column < model->columnCount(); column++)
            {
                 QString data = model->data(model->index(row, column), Qt::DisplayRole).toString();
                 if(column==0){
                     idzraportowane=data;
                 }
                 html += "<td>" + data + "</td>";
            }
            html += "</tr>";
            modelczynnosci->setQuery("SELECT c.opis, w.krotnosc from czynnosc c JOIN wykonanie w on w.fk_idc=c.idc where w.fk_idz='"+idzraportowane+"'");
            for(int row = 0; row < modelczynnosci->rowCount(); row++)
            {
                html += "<tr>";
                html += "<td> </td>";
                for(int column = 0; column < modelczynnosci->columnCount(); column++)
                {
                     QString dataczynnosci = modelczynnosci->data(modelczynnosci->index(row, column), Qt::DisplayRole).toString();
                     html += "<td>" + dataczynnosci + "</td>";
                }
               html += "</tr>";
            }
        }
        html += "</table></body></html>";

/*QTextDocument *document = new QTextDocument();
document->setHtml(html);*/

QWebView *podgladpliku = new QWebView();
podgladpliku->setHtml(html);

QPrinter printer(QPrinter::HighResolution);
    printer.setPageSize(QPrinter::A4);
    printer.setOutputFormat(QPrinter::PdfFormat);
    QFileDialog *fileDialog = new QFileDialog;
    fileDialog->setDefaultSuffix("pdf");
    printer.setOutputFileName(fileDialog->getSaveFileName(this,tr("Zapisz raport"), "Raport punktowy dla "+ numer +" za okres od "+ data_od +" do "+ data_do +"", tr("(*.pdf)")));
        podgladpliku->print(&printer);
        //document->print(&printer);
        //delete document;

}

void raporty::on_pushButton_druk_clicked()
{
    print();
}

void raporty::on_pushButton_druk2_clicked()
{
    print2();
}

