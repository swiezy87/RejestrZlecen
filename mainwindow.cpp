#include "mainwindow.h"
#include "ui_mainwindow.h"
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
#include <QShortcut>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->checkBox_szukaj_zgloszenie->setChecked(true);
    listadzialselect=listapracselect=listazlecselect=false;
    ui->pushButton->setDefault(true);
    ui->pushButton_dodaj_zgloszenie->setShortcut(tr("F1"));
    ui->pushButton_dodaj_czynnosc->setShortcut(tr("F2"));
    ui->pushButton_zakoncz_zgloszenie->setShortcut(tr("F3"));
    ui->pushButton->setShortcut(tr("Enter"));
    ui->lineEdit_punkty->setReadOnly(true);
    ui->lineEdit_dzial->setReadOnly(true);
    ui->lineEdit_imie->setReadOnly(true);
    ui->lineEdit_nazwisko->setReadOnly(true);
    ui->lineEdit_ido->setReadOnly(true);
    ui->lineEdit_idp->setReadOnly(true);
    ui->lineEdit_idz->setReadOnly(true);

    ui->actionKonfiguracja_czynno_ci_2->setEnabled(false);
    ui->actionKonfiguracja_o_rodk_w->setEnabled(false);
    ui->actionKonfiguracja_u_ytkownik_w->setEnabled(false);
    ui->actionZestawienie_miesi_czne->setEnabled(false);

    sygnalidz="";
    ui->page_listazgloszen->resize(1330,660);
    ui->dateEdit_do->setDate(QDate::currentDate());
    QFile plik("c://program1//pliczek.ini");
    if(!plik.open(QIODevice::ReadOnly))
       create_config_database();
    plik.close();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    database_connect();
    QString wpisanylogin=ui->lineEdit_Login->text();
    QString wpisanehaslo=ui->lineEdit_Haslo->text();
    QCryptographicHash hasloszyfr(QCryptographicHash::Md5);
    hasloszyfr.reset();
    hasloszyfr.addData(wpisanehaslo.toLatin1());
    wpisanehaslo=hasloszyfr.result().toHex();
    QString idp;
    QString login_baza;
    QString haslo_baza;
    QSqlQuery select;
    select.exec("SELECT idp, login, haslo, admin FROM pracownik WHERE login='"+wpisanylogin+"'and haslo='"+wpisanehaslo+"'");
       while(select.next())
       {
          idp = select.value(0).toString();
          login_baza = select.value(1).toString();
          haslo_baza = select.value(2).toString();
          czy_admin = select.value(3).toString();
       }

     if(wpisanylogin==login_baza and wpisanehaslo==haslo_baza and czy_admin=="TAK")
     {
         sygnalidp=idp;
         ui->page_Logowanie->hide();
         odswiez_tabele_zlec();
         odswiez_tabele_uzyt();
         odswiez_tabele_koszt();
         ui->page_listazgloszen->show();

         ui->lineEdit_imie->setDisabled(true);
         ui->label_imie->setDisabled(true);
         ui->lineEdit_nazwisko->setDisabled(true);
         ui->label_nazwisko->setDisabled(true);
         ui->lineEdit_dzial->setDisabled(true);
         ui->label_dzial->setDisabled(true);
         ui->lineEdit_punkty->setDisabled(true);
         ui->label_punkty->setDisabled(true);
         ui->lineEdit_ido->setDisabled(true);
         ui->label_ido->setDisabled(true);
         ui->lineEdit_idp->setDisabled(true);
         ui->label_idp->setDisabled(true);
         ui->lineEdit_idz->setDisabled(true);
         ui->label_idz->setDisabled(true);
         ui->dateEdit_data_zakonczenia->setDisabled(true);
         ui->dateEdit_data_zgloszenia->setDisabled(true);
         ui->label_opis_zakonczenia->setDisabled(true);
         ui->label_opis_zgloszenia->setDisabled(true);
         ui->label_data_zakonczenia->setDisabled(true);
         ui->label_data_zgloszenia->setDisabled(true);
         ui->textEdit_opis_zakonczenia->setDisabled(true);
         ui->textEdit_opis_zgloszenia->setDisabled(true);
         ui->toolButton_data_zak->setDisabled(true);
         ui->toolButton_data_zgl->setDisabled(true);
         ui->groupBox_zgloszenie_przyciski->setDisabled(false);
         ui->pushButton_dodaj_czynnosc->setDisabled(true);
         ui->pushButton_zakoncz_zgloszenie->setDisabled(true);
         ui->pushButton_dodaj_zgloszenie->setDisabled(true);
         ui->pushButton_bierzaca_data_zak->setDisabled(true);
         ui->pushButton_bierzaca_data_zgl->setDisabled(true);
         ui->pushButton_edytuj->setDisabled(true);
         ui->pushButton_zatwierdz->setDisabled(true);
         ui->groupBox_szukaj_dzial->setDisabled(false);
         ui->groupBox_szukaj_prac->setDisabled(false);

         ui->actionKonfiguracja_czynno_ci_2->setEnabled(true);
         ui->actionKonfiguracja_o_rodk_w->setEnabled(true);
         ui->actionKonfiguracja_u_ytkownik_w->setEnabled(true);
         ui->actionZestawienie_miesi_czne->setEnabled(true);

         ui->actionKonfiguracja_bazy->setEnabled(false);//blokuje zmianę konfiguracji bazy
         QDateTime dateTime = dateTime.currentDateTime();
         QString dateTimeString = dateTime.toString("yyyy-MM-dd hh:mm:ss");
         ui->dateEdit_data_zakonczenia->setDate(QDate::currentDate());
         ui->dateEdit_data_zgloszenia->setDate(QDate::currentDate());
         QSqlQuery selectinsert;
         selectinsert.exec("UPDATE pracownik SET ostatnie_logowanie='"+dateTimeString+"'WHERE idp='"+idp+"' ");
         //ui->actionKonfiguracja_bazy->setVisible(false);// blokuje rozwijanie
     }
     else
         if(czy_admin=="NIE")
             QMessageBox::information(this,tr(""),tr("Brak uprawnień administratora"));
             else
             QMessageBox::information(this,tr(""),tr("Błędny login lub haslo"));
}
void MainWindow::create_config_database()
{
    konfiguracjaBazy obiektkonfiguracjabazy;
    obiektkonfiguracjabazy.exec();
}

void MainWindow::konfiguracja_uzytkownika()
{
    Konfig_uzytkownika obiektkonfiguzytkownika;
    obiektkonfiguzytkownika.exec();
}
void MainWindow::konfiguracja_kosztow()
{
    Konfiguracja_Kosztow obiektkonfigkosztow;
    obiektkonfigkosztow.exec();
}
void MainWindow::konfiguracja_czynnosci()
{
    konfig_czynnosci obiektkonfigczynnosci;
    obiektkonfigczynnosci.exec();
}
void MainWindow::dodaj_zlecenie()
{
    dod_zlecenia obiektdodajzlecenie;
    obiektdodajzlecenie.exec();
}
void MainWindow::generuj_raporty()
{
    raporty obiektraporty;
    obiektraporty.exec();
}
void MainWindow::database_connect()
{
    QFile plik("c://program1//pliczek.ini");
    plik.open(QIODevice::ReadOnly);
    QTextStream wczytaj(&plik);
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName(wczytaj.readLine());
    db.setDatabaseName(wczytaj.readLine());
    db.setUserName(wczytaj.readLine());
    db.setPassword(wczytaj.readLine());
    plik.close();
    if(!db.open())
    {
         QMessageBox::information(this,tr(""),tr("błędne parametry bazy"));
         ui->actionKonfiguracja_bazy->setEnabled(true);
    }
    else
    {
        ui->actionKonfiguracja_bazy->setEnabled(false);
    }
}
void MainWindow::odswiez_tabele_zlec()
{
    QSqlQueryModel *model = new QSqlQueryModel;
    QString comboboxwyszukajzlec=ui->comboBox_szukaj_zgloszenie->currentText();
    QString sortuj=ui->comboBox_sortuj->currentText();
    QString lineeditwyszukajzlec=ui->lineEdit_szukaj_zgloszenie->text();
    QString data_od=ui->dateEdit_od->date().toString("yyyy-MM-dd");
    QString data_do=ui->dateEdit_do->date().toString("yyyy-MM-dd");
    if(comboboxwyszukajzlec!="" and !ui->checkBox_szukaj_zgloszenie->isChecked())
        model->setQuery("SELECT z.idz, p.imie, p.nazwisko, o.dzial, o.numer, z.opis_usterki, z.data_zgloszenia, z.data_zakonczenia, z.opis_zamkniecia, z.zakonczono FROM zgloszenie z JOIN pracownik p on z.fk_idp=p.idp JOIN osrodek o on z.fk_ido=o.ido where "+comboboxwyszukajzlec+" like '%"+lineeditwyszukajzlec+"%' and z.data_zgloszenia >= '"+data_od+"' and z.data_zgloszenia <= '"+data_do+"' ORDER BY "+sortuj+" DESC ");
    if(comboboxwyszukajzlec!="" and ui->checkBox_szukaj_zgloszenie->isChecked())
        model->setQuery("SELECT z.idz, p.imie, p.nazwisko, o.dzial, o.numer, z.opis_usterki, z.data_zgloszenia, z.data_zakonczenia, z.opis_zamkniecia, z.zakonczono FROM zgloszenie z JOIN pracownik p on z.fk_idp=p.idp JOIN osrodek o on z.fk_ido=o.ido where "+comboboxwyszukajzlec+" like '%"+lineeditwyszukajzlec+"%' and z.zakonczono='NIE' and z.data_zgloszenia >= '"+data_od+"' and z.data_zgloszenia <= '"+data_do+"' ORDER BY "+sortuj+" DESC ");
    if(comboboxwyszukajzlec=="" and ui->checkBox_szukaj_zgloszenie->isChecked())
        model->setQuery("SELECT z.idz, p.imie, p.nazwisko, o.dzial, o.numer, z.opis_usterki, z.data_zgloszenia, z.data_zakonczenia, z.opis_zamkniecia, z.zakonczono FROM zgloszenie z JOIN pracownik p on z.fk_idp=p.idp JOIN osrodek o on z.fk_ido=o.ido where z.zakonczono='NIE' and z.data_zgloszenia >= '"+data_od+"' and z.data_zgloszenia <= '"+data_do+"' ORDER BY "+sortuj+" DESC ");
    if(comboboxwyszukajzlec=="" and !ui->checkBox_szukaj_zgloszenie->isChecked())
       model->setQuery("SELECT z.idz, p.imie, p.nazwisko, o.dzial, o.numer, z.opis_usterki, z.data_zgloszenia, z.data_zakonczenia, z.opis_zamkniecia, z.zakonczono FROM zgloszenie z JOIN pracownik p on z.fk_idp=p.idp JOIN osrodek o on z.fk_ido=o.ido where z.data_zgloszenia >= '"+data_od+"' and z.data_zgloszenia <= '"+data_do+"' ORDER BY "+sortuj+" DESC ");
       model->setHeaderData(0, Qt::Horizontal, tr("idz"));
       model->setHeaderData(1, Qt::Horizontal, tr("imię"));
       model->setHeaderData(2, Qt::Horizontal, tr("nazwisko"));
       model->setHeaderData(3, Qt::Horizontal, tr("dział"));
       model->setHeaderData(4, Qt::Horizontal, tr("numer"));
       model->setHeaderData(5, Qt::Horizontal, tr("opis zgłoszenia"));
       model->setHeaderData(6, Qt::Horizontal, tr("data zgłoszenia"));
       model->setHeaderData(7, Qt::Horizontal, tr("data zakończenia"));
       model->setHeaderData(8, Qt::Horizontal, tr("opis zakończenia"));
       model->setHeaderData(9, Qt::Horizontal, tr("zakończono"));

       ui->tableView_listazgloszen->setModel(model);
       ui->tableView_listazgloszen->setAlternatingRowColors(true);
       ui->tableView_listazgloszen->resizeColumnToContents(0);
       ui->tableView_listazgloszen->setColumnWidth(0,0);
       ui->tableView_listazgloszen->resizeColumnToContents(1);
       ui->tableView_listazgloszen->resizeColumnToContents(2);
       ui->tableView_listazgloszen->resizeColumnToContents(3);
       ui->tableView_listazgloszen->resizeColumnToContents(4);
       ui->tableView_listazgloszen->resizeColumnToContents(6);
       ui->tableView_listazgloszen->resizeColumnToContents(7);
       ui->tableView_listazgloszen->resizeColumnToContents(9);
       ui->tableView_listazgloszen->setColumnWidth(5,250);
       ui->tableView_listazgloszen->setColumnWidth(8,250);
}

void MainWindow::odswiez_tabele_uzyt()
{
    QSqlQueryModel *model = new QSqlQueryModel;
    QString comboboxwyszukajuser=ui->comboBox_szukaj_prac->currentText();
    QString lineeditwyszukajuser=ui->lineEdit_szukaj_prac->text();
    if(comboboxwyszukajuser!="")
         model->setQuery("SELECT idp, imie, nazwisko, login FROM pracownik where "+comboboxwyszukajuser+" like '%"+lineeditwyszukajuser+"%' and blokada='NIE'");
         else
         model->setQuery("SELECT idp, imie, nazwisko, login FROM pracownik where blokada='NIE'");
         model->setHeaderData(0, Qt::Horizontal, tr("idp"));
         model->setHeaderData(1, Qt::Horizontal, tr("imię"));
         model->setHeaderData(2, Qt::Horizontal, tr("nazwisko"));
         model->setHeaderData(3, Qt::Horizontal, tr("login"));
         ui->tableView_uzytkownicy->setModel(model);
         ui->tableView_uzytkownicy->setAlternatingRowColors(true);
         ui->tableView_uzytkownicy->resizeColumnsToContents();
         ui->tableView_uzytkownicy->setColumnWidth(0,0);
}

void MainWindow::odswiez_tabele_koszt()
{
    QSqlQueryModel *model = new QSqlQueryModel;
    QString comboboxwyszukaj=ui->comboBox_szukaj_dzial->currentText();
    QString lineeditwyszukaj=ui->lineEdit_szukaj_dzial->text();
    QString sortuj=ui->comboBox_sortuj_osrodki->currentText();
    if(comboboxwyszukaj!="")
         model->setQuery("SELECT ido, dzial, numer FROM osrodek where "+comboboxwyszukaj+" like '%"+lineeditwyszukaj+"%' and blokada='NIE' ORDER BY "+sortuj+" ");
         else
         model->setQuery("SELECT ido, dzial, numer FROM osrodek where blokada='NIE' ORDER BY "+sortuj+" ");
         model->setHeaderData(0, Qt::Horizontal, tr("ido"));
         model->setHeaderData(1, Qt::Horizontal, tr("dział"));
         model->setHeaderData(2, Qt::Horizontal, tr("numer"));
         ui->tableView_osrodki->setModel(model);
         ui->tableView_osrodki->setAlternatingRowColors(true);
         ui->tableView_osrodki->resizeColumnsToContents();
         ui->tableView_osrodki->setColumnWidth(0,0);
}


void MainWindow::on_actionKonfiguracja_bazy_triggered()
{
     create_config_database();
}

void MainWindow::on_actionKonfiguracja_u_ytkownik_w_triggered()
{
     konfiguracja_uzytkownika();
}

void MainWindow::on_actionKonfiguracja_o_rodk_w_triggered()
{
    konfiguracja_kosztow();
}

void MainWindow::on_actionKonfiguracja_czynno_ci_2_triggered()
{
    konfiguracja_czynnosci();
}

void MainWindow::on_actionZestawienie_miesi_czne_triggered()
{
    generuj_raporty();
}

void MainWindow::on_pushButton_szukaj_dzial_clicked()
{
    odswiez_tabele_koszt();
}

void MainWindow::on_pushButton_szukaj_prac_clicked()
{
    odswiez_tabele_uzyt();
}

void MainWindow::on_pushButton_szukaj_zgloszenie_clicked()
{
    odswiez_tabele_zlec();
}

void MainWindow::on_comboBox_sortuj_activated(const QString &arg1)
{
    odswiez_tabele_zlec();
}

void MainWindow::on_lineEdit_Login_returnPressed()
{
    on_pushButton_clicked();
}

void MainWindow::on_lineEdit_Haslo_returnPressed()
{
    on_pushButton_clicked();
}

void MainWindow::on_pushButton_dodaj_zgloszenie_clicked()
{
    if(ui->lineEdit_idp->text()=="" or ui->lineEdit_ido->text()=="")
        QMessageBox::information(this,tr("Błąd"),tr("Wybierz ośrodek oraz osobę"));
    else
    {
        if (ui->textEdit_opis_zgloszenia->toPlainText().length() > 3)
        {
            QString idp=ui->lineEdit_idp->text();
            QString ido=ui->lineEdit_ido->text();
            QString datazgloszenia=ui->dateEdit_data_zgloszenia->text();
            QString opiszgloszenia=ui->textEdit_opis_zgloszenia->toPlainText();
            QSqlQuery insert;
            insert.exec("INSERT INTO `zgloszenie`(data_zgloszenia, fk_idp, fk_ido, opis_usterki) VALUES ('"+datazgloszenia+"','"+idp+"','"+ido+"', '"+opiszgloszenia+"')");
            odswiez_tabele_zlec();
            //czysc_pola();
            ustaw_widok_default();
        }
        else
            QMessageBox::information(this,tr(""),tr(" brak lub za krótki opis zgłoszenia"));

    }
}

void MainWindow::czysc_pola()
{
    ui->lineEdit_dzial->setText("");
    ui->lineEdit_ido->setText("");
    ui->lineEdit_idp->setText("");
    ui->lineEdit_idz->setText("");
    ui->lineEdit_imie->setText("");
    ui->lineEdit_punkty->setText("");
    ui->lineEdit_nazwisko->setText("");
}

void MainWindow::on_pushButton_zakoncz_zgloszenie_clicked()
{
    sygnalidz=ui->lineEdit_idz->text();
    if(sygnalidz=="")
        QMessageBox::information(this,tr("Błąd"),tr("Wybierz zgłoszenie"));
    else
    {
        QString idz=ui->lineEdit_idz->text();
        QString datazakonczenia=ui->dateEdit_data_zakonczenia->text();
        QString opiszakonczenia=ui->textEdit_opis_zakonczenia->toPlainText();
        QSqlQuery selectinsert;
        selectinsert.exec("UPDATE zgloszenie SET data_zakonczenia='"+datazakonczenia+"', opis_zamkniecia='"+opiszakonczenia+"', zakonczono='TAK' WHERE idz='"+idz+"' ");
        odswiez_tabele_zlec();
        czysc_pola();
        ustaw_widok_default();
    }
}

void MainWindow::on_pushButton_bierzaca_data_zgl_clicked()
{
    ui->dateEdit_data_zgloszenia->setDate(QDate::currentDate());
}

void MainWindow::on_pushButton_bierzaca_data_zak_clicked()
{
    ui->dateEdit_data_zakonczenia->setDate(QDate::currentDate());
}

void MainWindow::on_pushButton_dodaj_czynnosc_clicked()
{
    sygnalidz=ui->lineEdit_idz->text();
    if(sygnalidz=="")
        QMessageBox::information(this,tr("Błąd"),tr("Wybierz zgłoszenie"));
    else
    {
    nowydodaj_zlecenia = new dod_zlecenia;
    nowydodaj_zlecenia->setModal(true); //blokuje pozostałe okna
    nowydodaj_zlecenia->show();
    connect(this, SIGNAL(dodajczynnoscidz(QString)),nowydodaj_zlecenia, SLOT(setidz(QString)));
    connect(this, SIGNAL(dodajczynnoscidp(QString)),nowydodaj_zlecenia, SLOT(setidp(QString)));
    emit dodajczynnoscidz(sygnalidz);
    emit dodajczynnoscidp(sygnalidp);
    //czysc_pola();
    }
}

void MainWindow::on_checkBox_szukaj_zgloszenie_clicked()
{
    odswiez_tabele_zlec();
}


void MainWindow::on_tableView_listazgloszen_clicked(const QModelIndex &index)
{
    listazlecselect=true;
    listapracselect=false;
    listadzialselect=false;

    QString val = (ui->tableView_listazgloszen->model()->data(ui->tableView_listazgloszen->model()->index(ui->tableView_listazgloszen->currentIndex().row(), 0))).toString();
    QSqlQuery model_select;
    model_select.prepare("SELECT z.idz, p.imie, p.nazwisko, o.dzial, z.opis_usterki, z.data_zgloszenia, z.data_zakonczenia, z.opis_zamkniecia, p.idp, o.ido, z.zakonczono FROM zgloszenie z JOIN pracownik p on z.fk_idp=p.idp JOIN osrodek o on z.fk_ido=o.ido where z.idz='"+val+"'");
    if(model_select.exec())
        while(model_select.next())
        {
            ui->lineEdit_idz->setText(model_select.value(0).toString());
            sygnalidz=model_select.value(0).toString();
            ui->lineEdit_imie->setText(model_select.value(1).toString());
            ui->lineEdit_nazwisko->setText(model_select.value(2).toString());
            ui->lineEdit_dzial->setText(model_select.value(3).toString());
            ui->textEdit_opis_zgloszenia->setPlainText(model_select.value(4).toString());
            ui->dateEdit_data_zgloszenia->setDate(model_select.value(5).toDate());
            ui->dateEdit_data_zakonczenia->setDate(model_select.value(6).toDate());
            ui->textEdit_opis_zakonczenia->setPlainText(model_select.value(7).toString());
            ui->lineEdit_idp->setText(model_select.value(8).toString());
            ui->lineEdit_ido->setText(model_select.value(9).toString());
            zakonczono=model_select.value(10).toString();
        }
    ui->dateEdit_data_zakonczenia->setMinimumDate(ui->dateEdit_data_zgloszenia->date());
    ui->dateEdit_data_zakonczenia->setDate(ui->dateEdit_data_zgloszenia->date());
    QSqlQuery liczpunkty;
    liczpunkty.prepare("select sum(w.krotnosc * c.punkty) punkty from wykonanie w join czynnosc c on w.fk_idc=c.idc where w.fk_idz='"+sygnalidz+"' ");
    if(liczpunkty.exec())
        while(liczpunkty.next())
        {
            ui->lineEdit_punkty->setText(liczpunkty.value(0).toString());
        }
    ustaw_widok();
}

void MainWindow::on_tableView_uzytkownicy_clicked(const QModelIndex &index)
{
    listapracselect=true;
    listazlecselect=false;
    ustaw_widok();
    QString val = (ui->tableView_uzytkownicy->model()->data(ui->tableView_uzytkownicy->model()->index(ui->tableView_uzytkownicy->currentIndex().row(), 0))).toString();  // o takich wspolrzednych
    QSqlQuery model_select;
    model_select.prepare("SELECT idp, imie, nazwisko, login FROM pracownik where idp='"+val+"' ");
    if(model_select.exec())
        while(model_select.next())
        {
            ui->lineEdit_idp->setText(model_select.value(0).toString());
            ui->lineEdit_imie->setText(model_select.value(1).toString());
            ui->lineEdit_nazwisko->setText(model_select.value(2).toString());
        }
}

void MainWindow::on_tableView_osrodki_clicked(const QModelIndex &index)
{
    listadzialselect=true;

    listazlecselect=false;
    ustaw_widok();
    QString val = (ui->tableView_osrodki->model()->data(ui->tableView_osrodki->model()->index(ui->tableView_osrodki->currentIndex().row(), 0))).toString();
    QSqlQuery model_select;
    model_select.prepare("SELECT ido, dzial FROM osrodek where ido='"+val+"' ");
    if(model_select.exec())
        while(model_select.next())
        {
            ui->lineEdit_ido->setText(model_select.value(0).toString());
            ui->lineEdit_dzial->setText(model_select.value(1).toString());
        }
}
void MainWindow::ustaw_widok_default()
{
    ui->lineEdit_imie->setDisabled(true);
    ui->label_imie->setDisabled(true);
    ui->lineEdit_nazwisko->setDisabled(true);
    ui->label_nazwisko->setDisabled(true);
    ui->lineEdit_dzial->setDisabled(true);
    ui->label_dzial->setDisabled(true);
    ui->lineEdit_punkty->setDisabled(true);
    ui->label_punkty->setDisabled(true);
    ui->lineEdit_ido->setDisabled(true);
    ui->label_ido->setDisabled(true);
    ui->lineEdit_idp->setDisabled(true);
    ui->label_idp->setDisabled(true);
    ui->lineEdit_idz->setDisabled(true);
    ui->label_idz->setDisabled(true);
    ui->dateEdit_data_zakonczenia->setDisabled(true);
    ui->dateEdit_data_zgloszenia->setDisabled(true);
    ui->label_opis_zakonczenia->setDisabled(true);
    ui->label_opis_zgloszenia->setDisabled(true);
    ui->label_data_zakonczenia->setDisabled(true);
    ui->label_data_zgloszenia->setDisabled(true);
    ui->textEdit_opis_zakonczenia->setDisabled(true);
    ui->textEdit_opis_zgloszenia->setDisabled(true);
    ui->groupBox_zgloszenie_przyciski->setDisabled(false);
    ui->pushButton_dodaj_czynnosc->setDisabled(true);
    ui->pushButton_zakoncz_zgloszenie->setDisabled(true);
    ui->pushButton_dodaj_zgloszenie->setDisabled(true);
    ui->pushButton_bierzaca_data_zak->setDisabled(true);
    ui->pushButton_bierzaca_data_zgl->setDisabled(true);
    ui->pushButton_edytuj->setDisabled(true);
    ui->pushButton_zatwierdz->setDisabled(true);
    ui->toolButton_data_zak->setDisabled(true);
    ui->groupBox_szukaj_dzial->setDisabled(false);
    ui->groupBox_szukaj_prac->setDisabled(false);
}

void MainWindow::ustaw_widok()
{
    if(listapracselect and !listadzialselect)
    {
        ui->lineEdit_punkty->setText("");
        ui->lineEdit_dzial->setText("");
        ui->lineEdit_ido->setText("");
        ui->lineEdit_idz->setText("");
    }
    if(!listapracselect and listadzialselect)
    {
        ui->lineEdit_imie->setText("");
        ui->lineEdit_nazwisko->setText("");
        ui->lineEdit_idp->setText("");
        ui->lineEdit_idz->setText("");
    }
    if(listazlecselect)
    {
        ui->lineEdit_imie->setDisabled(false);
        ui->label_imie->setDisabled(false);
        ui->lineEdit_nazwisko->setDisabled(false);
        ui->label_nazwisko->setDisabled(false);
        ui->lineEdit_dzial->setDisabled(false);
        ui->label_dzial->setDisabled(false);
        ui->lineEdit_punkty->setDisabled(false);
        ui->label_punkty->setDisabled(false);
        ui->lineEdit_ido->setDisabled(true);
        ui->label_ido->setDisabled(true);
        ui->lineEdit_idp->setDisabled(true);
        ui->label_idp->setDisabled(true);
        ui->lineEdit_idz->setDisabled(true);
        ui->label_idz->setDisabled(true);
        ui->dateEdit_data_zakonczenia->setDisabled(true);
        ui->dateEdit_data_zgloszenia->setDisabled(true);
        ui->label_opis_zakonczenia->setDisabled(false);
        ui->label_opis_zgloszenia->setDisabled(true);
        ui->label_data_zakonczenia->setDisabled(true);
        ui->label_data_zgloszenia->setDisabled(true);
        ui->textEdit_opis_zgloszenia->setDisabled(true);
        ui->groupBox_zgloszenie_przyciski->setDisabled(false);
        ui->pushButton_dodaj_zgloszenie->setDisabled(true);
        ui->pushButton_bierzaca_data_zak->setDisabled(true);
        ui->pushButton_bierzaca_data_zgl->setDisabled(true);
        ui->toolButton_data_zak->setDisabled(true);
        ui->toolButton_data_zgl->setDisabled(true);
        ui->pushButton_edytuj->setDisabled(true);
        ui->pushButton_zatwierdz->setDisabled(true);
        ui->groupBox_szukaj_dzial->setDisabled(false);
        ui->groupBox_szukaj_prac->setDisabled(false);

        if(zakonczono=="TAK")
        {
            ui->pushButton_zakoncz_zgloszenie->setDisabled(true);
            ui->pushButton_dodaj_czynnosc->setDisabled(true);
            ui->textEdit_opis_zakonczenia->setDisabled(true);
            ui->label_opis_zakonczenia->setDisabled(true);
            ui->pushButton_edytuj->setDisabled(false);
            ui->pushButton_edytuj->setDisabled(false);
            ui->pushButton_zatwierdz->setDisabled(true);
        }
        else
        {
            ui->pushButton_zakoncz_zgloszenie->setDisabled(false);
            ui->pushButton_dodaj_czynnosc->setDisabled(false);
            ui->textEdit_opis_zakonczenia->setDisabled(false);
            ui->label_opis_zakonczenia->setDisabled(false);
            ui->label_data_zakonczenia->setDisabled(false);
            ui->dateEdit_data_zakonczenia->setDisabled(false);
            ui->toolButton_data_zak->setDisabled(false);
            ui->pushButton_bierzaca_data_zak->setDisabled(false);
            ui->pushButton_edytuj->setDisabled(true);
            ui->pushButton_zatwierdz->setDisabled(true);
        }
    }
    if(listadzialselect or listapracselect)
    {
        ui->lineEdit_imie->setDisabled(false);
        ui->label_imie->setDisabled(false);
        ui->lineEdit_nazwisko->setDisabled(false);
        ui->label_nazwisko->setDisabled(false);
        ui->lineEdit_dzial->setDisabled(false);
        ui->label_dzial->setDisabled(false);
        ui->lineEdit_punkty->setDisabled(true);
        ui->label_punkty->setDisabled(true);
        ui->lineEdit_ido->setDisabled(true);
        ui->label_ido->setDisabled(true);
        ui->lineEdit_idp->setDisabled(true);
        ui->label_idp->setDisabled(true);
        ui->lineEdit_idz->setDisabled(true);
        ui->label_idz->setDisabled(true);
        ui->dateEdit_data_zakonczenia->setDisabled(true);
        ui->dateEdit_data_zgloszenia->setDisabled(true);
        ui->label_opis_zakonczenia->setDisabled(true);
        ui->label_opis_zgloszenia->setDisabled(true);
        ui->label_data_zakonczenia->setDisabled(true);
        ui->label_data_zgloszenia->setDisabled(false);
        ui->textEdit_opis_zakonczenia->setDisabled(true);
        ui->textEdit_opis_zgloszenia->setDisabled(true);
        ui->groupBox_zgloszenie_przyciski->setDisabled(false);
        ui->pushButton_dodaj_czynnosc->setDisabled(true);
        ui->pushButton_zakoncz_zgloszenie->setDisabled(true);
        ui->pushButton_dodaj_zgloszenie->setDisabled(true);
        ui->pushButton_bierzaca_data_zak->setDisabled(true);
        ui->pushButton_bierzaca_data_zgl->setDisabled(false);
        ui->label_data_zgloszenia->setDisabled(true);
        ui->pushButton_bierzaca_data_zgl->setDisabled(true);
        ui->toolButton_data_zak->setDisabled(true);
        ui->pushButton_edytuj->setDisabled(true);
        ui->pushButton_zatwierdz->setDisabled(true);
        ui->textEdit_opis_zakonczenia->setPlainText("");
        ui->textEdit_opis_zgloszenia->setPlainText("");

        ui->dateEdit_data_zgloszenia->setDate(QDate::currentDate());
        ui->dateEdit_data_zakonczenia->setDate(QDate::currentDate());

        if(listadzialselect and listapracselect)
        {
            ui->lineEdit_imie->setDisabled(false);
            ui->label_imie->setDisabled(false);
            ui->lineEdit_nazwisko->setDisabled(false);
            ui->label_nazwisko->setDisabled(false);
            ui->lineEdit_dzial->setDisabled(false);
            ui->label_dzial->setDisabled(false);
            ui->lineEdit_punkty->setDisabled(true);
            ui->label_punkty->setDisabled(true);
            ui->lineEdit_ido->setDisabled(true);
            ui->label_ido->setDisabled(true);
            ui->lineEdit_idp->setDisabled(true);
            ui->label_idp->setDisabled(true);
            ui->lineEdit_idz->setDisabled(true);
            ui->label_idz->setDisabled(true);
            ui->dateEdit_data_zakonczenia->setDisabled(true);
            ui->dateEdit_data_zgloszenia->setDisabled(false);
            ui->label_opis_zakonczenia->setDisabled(true);
            ui->label_opis_zgloszenia->setDisabled(false);
            ui->label_data_zakonczenia->setDisabled(true);
            ui->label_data_zgloszenia->setDisabled(false);
            ui->textEdit_opis_zakonczenia->setDisabled(true);
            ui->textEdit_opis_zgloszenia->setDisabled(false);
            ui->groupBox_zgloszenie_przyciski->setDisabled(false);
            ui->pushButton_dodaj_czynnosc->setDisabled(true);
            ui->pushButton_zakoncz_zgloszenie->setDisabled(true);
            ui->pushButton_dodaj_zgloszenie->setDisabled(false);
            ui->pushButton_bierzaca_data_zak->setDisabled(true);
            ui->pushButton_bierzaca_data_zgl->setDisabled(false);
            ui->toolButton_data_zgl->setDisabled(false);
            ui->pushButton_edytuj->setDisabled(true);
            ui->pushButton_zatwierdz->setDisabled(true);
        }
    }
}

void MainWindow::on_toolButton_data_od_clicked()
{
    nowykalendarz = new kalendarz;
    nowykalendarz->setModal(true);
    nowykalendarz->show();
    connect(nowykalendarz, SIGNAL(sygnal_daty(QDate)),this, SLOT(ustaw_date_od_kalendar(QDate)));
}

void MainWindow::on_toolButton_data_do_clicked()
{
    nowykalendarz = new kalendarz;
    nowykalendarz->setModal(true);
    nowykalendarz->show();
    connect(nowykalendarz, SIGNAL(sygnal_daty(QDate)),this, SLOT(ustaw_date_do_kalendar(QDate)));
}

void MainWindow::on_toolButton_data_zgl_clicked()
{
    nowykalendarz = new kalendarz;
    nowykalendarz->setModal(true);
    nowykalendarz->show();
    connect(nowykalendarz, SIGNAL(sygnal_daty(QDate)),this, SLOT(ustaw_date_zgl_kalendar(QDate)));
}

void MainWindow::on_toolButton_data_zak_clicked()
{
    nowykalendarz = new kalendarz;
    nowykalendarz->setModal(true);
    nowykalendarz->show();
    connect(nowykalendarz, SIGNAL(sygnal_daty(QDate)),this, SLOT(ustaw_date_zak_kalendar(QDate)));
}
void MainWindow::ustaw_date_od_kalendar(QDate datazk)
{
       ui->dateEdit_od->setDate(datazk);
       odswiez_tabele_zlec();
}
void MainWindow::ustaw_date_do_kalendar(QDate datazk)
{
       ui->dateEdit_do->setDate(datazk);
       odswiez_tabele_zlec();
}
void MainWindow::ustaw_date_zgl_kalendar(QDate datazk)
{
       ui->dateEdit_data_zgloszenia->setDate(datazk);
}
void MainWindow::ustaw_date_zak_kalendar(QDate datazk)
{
       ui->dateEdit_data_zakonczenia->setDate(datazk);
}

void MainWindow::on_textEdit_opis_zgloszenia_textChanged()
{
    if (ui->textEdit_opis_zgloszenia->toPlainText().length() > 299)
    {
        ui->textEdit_opis_zgloszenia->textCursor().deletePreviousChar();
    }
}

void MainWindow::on_textEdit_opis_zakonczenia_textChanged()
{
    if (ui->textEdit_opis_zakonczenia->toPlainText().length() > 299)
    {
        ui->textEdit_opis_zakonczenia->textCursor().deletePreviousChar();
    }

}

void MainWindow::on_lineEdit_szukaj_dzial_textChanged(const QString &arg1)
{
    odswiez_tabele_koszt();
}

void MainWindow::on_lineEdit_szukaj_prac_textChanged(const QString &arg1)
{
    odswiez_tabele_uzyt();
}

void MainWindow::on_lineEdit_szukaj_zgloszenie_textChanged(const QString &arg1)
{
    odswiez_tabele_zlec();
}

void MainWindow::on_comboBox_sortuj_osrodki_activated(const QString &arg1)
{
    odswiez_tabele_koszt();
}

void MainWindow::on_pushButton_edytuj_clicked()
{
    ui->lineEdit_imie->setDisabled(false);
    ui->label_imie->setDisabled(false);
    ui->lineEdit_nazwisko->setDisabled(false);
    ui->label_nazwisko->setDisabled(false);
    ui->lineEdit_dzial->setDisabled(false);
    ui->label_dzial->setDisabled(false);
    ui->lineEdit_punkty->setDisabled(false);
    ui->label_punkty->setDisabled(false);
    ui->lineEdit_ido->setDisabled(true);
    ui->label_ido->setDisabled(true);
    ui->lineEdit_idp->setDisabled(true);
    ui->label_idp->setDisabled(true);
    ui->lineEdit_idz->setDisabled(true);
    ui->label_idz->setDisabled(true);
    ui->dateEdit_data_zakonczenia->setDisabled(false);
    ui->dateEdit_data_zgloszenia->setDisabled(false);
    ui->label_opis_zakonczenia->setDisabled(false);
    ui->label_opis_zgloszenia->setDisabled(false);
    ui->label_data_zakonczenia->setDisabled(false);
    ui->label_data_zgloszenia->setDisabled(false);
    ui->textEdit_opis_zakonczenia->setDisabled(false);
    ui->textEdit_opis_zgloszenia->setDisabled(false);
    ui->groupBox_zgloszenie_przyciski->setDisabled(false);
    ui->pushButton_dodaj_czynnosc->setDisabled(false);
    ui->pushButton_zakoncz_zgloszenie->setDisabled(true);
    ui->pushButton_dodaj_zgloszenie->setDisabled(true);
    ui->pushButton_bierzaca_data_zak->setDisabled(false);
    ui->pushButton_bierzaca_data_zgl->setDisabled(false);
    ui->toolButton_data_zgl->setDisabled(false);
    ui->toolButton_data_zak->setDisabled(false);
    ui->pushButton_edytuj->setDisabled(true);
    ui->pushButton_zatwierdz->setDisabled(false);
    ui->groupBox_szukaj_dzial->setDisabled(true);
    ui->groupBox_szukaj_prac->setDisabled(true);
}

void MainWindow::on_pushButton_zatwierdz_clicked()
{
    sygnalidz=ui->lineEdit_idz->text();
    if(sygnalidz=="")
        QMessageBox::information(this,tr("Błąd"),tr("Wybierz zgłoszenie"));
    else
    {
        QString idz=ui->lineEdit_idz->text();
        QString datazgloszenia=ui->dateEdit_data_zgloszenia->text();
        QString opiszgloszenia=ui->textEdit_opis_zgloszenia->toPlainText();
        QString datazakonczenia=ui->dateEdit_data_zakonczenia->text();
        QString opiszakonczenia=ui->textEdit_opis_zakonczenia->toPlainText();
        QSqlQuery update;
        update.exec("UPDATE zgloszenie SET data_zakonczenia='"+datazakonczenia+"', data_zgloszenia='"+datazgloszenia+"', opis_zamkniecia='"+opiszakonczenia+"', opis_usterki='"+opiszgloszenia+"' WHERE idz='"+idz+"' ");
        odswiez_tabele_zlec();
        czysc_pola();
        ustaw_widok_default();
    }
}

void MainWindow::on_dateEdit_data_zgloszenia_dateChanged(const QDate &date)
{
    ui->dateEdit_data_zakonczenia->setMinimumDate(ui->dateEdit_data_zgloszenia->date());
}


