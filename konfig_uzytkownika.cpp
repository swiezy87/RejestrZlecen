#include "konfig_uzytkownika.h"
#include "ui_konfig_uzytkownika.h"
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QMessageBox>
#include <QCryptographicHash>
#include <QDateEdit>
#include <QBrush>
//#include <

Konfig_uzytkownika::Konfig_uzytkownika(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Konfig_uzytkownika)
{
    ui->setupUi(this);
    odswiez_tabeleuser();
    ui->lineEdit_idp->setEnabled(false);
    ui->groupBox_haslo->setEnabled(false);
    QRegExp rx("[A-ząężźółćśńĄĘŻŹÓŁĆŚŃ(), ]+");
    QRegExpValidator  *valid = new QRegExpValidator(rx,this);
    ui->lineEdit_imie->setValidator( valid);
    ui->lineEdit_nazwisko->setValidator( valid);
    ui->lineEdit_login->setValidator( valid);
    ui->lineEdit_imie->setMaxLength(20);
    ui->lineEdit_nazwisko->setMaxLength(40);
    ui->lineEdit_login->setMaxLength(22);
    ui->lineEdit_nowe_haslo->setMaxLength(12);
    ui->lineEdit_potw_haslo->setMaxLength(12);
}

Konfig_uzytkownika::~Konfig_uzytkownika()
{
    delete ui;
}

void Konfig_uzytkownika::odswiez_tabeleuser()
{
    QSqlQueryModel *model = new QSqlQueryModel;
    QString comboboxwyszukajuser=ui->comboBox_szukaj->currentText();
    QString lineeditwyszukajuser=ui->lineEdit_szukaj->text();
    if(comboboxwyszukajuser!="")
         model->setQuery("SELECT idp, imie, nazwisko , login, blokada, admin, ostatnie_logowanie, ostatnia_zmiana_hasla FROM pracownik where "+comboboxwyszukajuser+" like '%"+lineeditwyszukajuser+"%'");
         else
         model->setQuery("SELECT idp, imie, nazwisko , login, blokada, admin, ostatnie_logowanie, ostatnia_zmiana_hasla FROM pracownik");
         model->setHeaderData(0, Qt::Horizontal, tr("id"));
         model->setHeaderData(1, Qt::Horizontal, tr("imię"));
         model->setHeaderData(2, Qt::Horizontal, tr("nazwisko"));
         model->setHeaderData(3, Qt::Horizontal, tr("login"));
         model->setHeaderData(4, Qt::Horizontal, tr("blokada"));
         model->setHeaderData(5, Qt::Horizontal, tr("admin"));
         model->setHeaderData(6, Qt::Horizontal, tr("logowanie"));
         model->setHeaderData(7, Qt::Horizontal, tr("zmiana hasła"));
         ui->tableView_uzytkownicy->setModel(model);
         ui->tableView_uzytkownicy->resizeColumnsToContents();

}

void Konfig_uzytkownika::on_pushButton_Dodaj_clicked()
{
    QString imie=ui->lineEdit_imie->text();
    QString nazwisko=ui->lineEdit_nazwisko->text();
    QString admin=ui->comboBox_admin->currentText();
    QString login=ui->lineEdit_login->text();
    QString blokada=ui->comboBox_blokada->currentText();
    QString login_baza;

    QSqlQuery model_select;
    model_select.prepare("SELECT login FROM pracownik where login='"+login+"' ");
    if(model_select.exec())
        while(model_select.next())
        {
            login_baza=model_select.value(0).toString();
        }
    if(login_baza==login)
         {
             QMessageBox::information(this,tr("Błąd"),tr("Pracownik o takim loginie już istnieje"));
         }
    else
        {
            QSqlQuery selectinsert;
            selectinsert.exec("INSERT INTO pracownik (imie, nazwisko, admin, login, blokada) VALUES ('"+imie+"', '"+nazwisko+"','"+admin+"', '"+login+"','"+blokada+"')");
         }
    odswiez_tabeleuser();
}

void Konfig_uzytkownika::on_pushButton_Wyszukaj_clicked()
{
    odswiez_tabeleuser();
}

void Konfig_uzytkownika::on_tableView_uzytkownicy_activated(const QModelIndex &index)
{
    QString val = (ui->tableView_uzytkownicy->model()->data(ui->tableView_uzytkownicy->model()->index(ui->tableView_uzytkownicy->currentIndex().row(), 0))).toString();  // o takich wspolrzednych
    QSqlQuery model_select;
    model_select.prepare("SELECT idp, imie, nazwisko, admin, login, blokada, ostatnie_logowanie, ostatnia_zmiana_hasla FROM pracownik where idp='"+val+"' ");
    if(model_select.exec())
        while(model_select.next())
        {
            ui->lineEdit_idp->setText(model_select.value(0).toString());
            ui->lineEdit_imie->setText(model_select.value(1).toString());
            ui->lineEdit_nazwisko->setText(model_select.value(2).toString());
            ui->comboBox_admin->setCurrentText(model_select.value(3).toString());
            ui->lineEdit_login->setText(model_select.value(4).toString());
            ui->comboBox_blokada->setCurrentText(model_select.value(5).toString());
        }
}

void Konfig_uzytkownika::on_pushButton_Edytuj_clicked()
{
    QString idp=ui->lineEdit_idp->text();
    QString imie=ui->lineEdit_imie->text();
    QString nazwisko=ui->lineEdit_nazwisko->text();
    QString admin=ui->comboBox_admin->currentText();
    QString login=ui->lineEdit_login->text();
    QString blokada=ui->comboBox_blokada->currentText();
    if(idp=="")
        QMessageBox::information(this,tr("Błąd"),tr("Wybierz pracownika"));
    else
    {
    QSqlQuery update;
    update.exec("UPDATE pracownik SET imie='"+imie+"', nazwisko='"+nazwisko+"', admin='"+admin+"', login='"+login+"', blokada='"+blokada+"' WHERE idp='"+idp+"' ");
    odswiez_tabeleuser();
    }
}


void Konfig_uzytkownika::on_pushButton_zmienhaslo_clicked()
{

    QString idp=ui->lineEdit_idp->text();
    if(idp=="")
        QMessageBox::information(this,tr("Błąd"),tr("Wybierz pracownika"));
    else
    {
        ui->groupBox_haslo->setEnabled(true);
    }
}

void Konfig_uzytkownika::on_pushButton_OK_clicked()
{
    QString nowe_haslo=ui->lineEdit_nowe_haslo->text();
    QString powt_haslo=ui->lineEdit_potw_haslo->text();
    QString idp=ui->lineEdit_idp->text();
    if(nowe_haslo==powt_haslo)
    {
        QCryptographicHash hasloszyfr(QCryptographicHash::Md5);
        hasloszyfr.reset();
        hasloszyfr.addData(nowe_haslo.toLatin1());
        QString zaszyfrowanehaslo=hasloszyfr.result().toHex();
        QDateTime dateTime = dateTime.currentDateTime();
        QString dateTimeString = dateTime.toString("yyyy-MM-dd hh:mm:ss");
        QSqlQuery update;
        update.exec("UPDATE pracownik SET haslo='"+zaszyfrowanehaslo+"', ostatnia_zmiana_hasla='"+dateTimeString+"' WHERE idp='"+idp+"' ");
        ui->groupBox_user->setEnabled(true);        
        ui->groupBox_haslo->setEnabled(false);       
        odswiez_tabeleuser();
        QMessageBox::information(this,tr(""),tr("Udana zmiana hasla"));
    }
    else
        QMessageBox::information(this,tr("Błąd"),tr("Podane hasła są różne"));

}

void Konfig_uzytkownika::on_lineEdit_szukaj_textChanged(const QString &arg1)
{
    odswiez_tabeleuser();
}
