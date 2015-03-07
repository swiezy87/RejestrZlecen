#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qmessagebox.h>
#include <QtSql>
#include <QtDebug>
#include <QFileInfo>
#include <dod_zlecenia.h>
#include <kalendarz.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void create_config_database();
    void database_connect();
    void konfiguracja_uzytkownika();
    void konfiguracja_kosztow();
    void dodaj_zlecenie();
    void generuj_raporty();
    void odswiez_tabele_zlec();
    void odswiez_tabele_uzyt();
    void odswiez_tabele_koszt();
    void czysc_pola();
    void ustaw_date_od_kalendar(QDate);
    void ustaw_date_do_kalendar(QDate);
    void ustaw_date_zak_kalendar(QDate);
    void ustaw_date_zgl_kalendar(QDate);
    void on_actionKonfiguracja_bazy_triggered();
    void on_actionKonfiguracja_o_rodk_w_triggered();
    void konfiguracja_czynnosci();
    void ustaw_widok();
    void ustaw_widok_default();
    void on_pushButton_szukaj_dzial_clicked();
    void on_pushButton_szukaj_prac_clicked();
    void on_pushButton_szukaj_zgloszenie_clicked();
    void on_pushButton_dodaj_zgloszenie_clicked();
    void on_pushButton_zakoncz_zgloszenie_clicked();
    void on_pushButton_bierzaca_data_zgl_clicked();
    void on_pushButton_bierzaca_data_zak_clicked();
    void on_pushButton_dodaj_czynnosc_clicked();
    void on_checkBox_szukaj_zgloszenie_clicked();
    void on_tableView_listazgloszen_clicked(const QModelIndex &index);
    void on_tableView_uzytkownicy_clicked(const QModelIndex &index);
    void on_tableView_osrodki_clicked(const QModelIndex &index);
    void on_toolButton_data_od_clicked();
    void on_toolButton_data_do_clicked();
    void on_toolButton_data_zgl_clicked();
    void on_toolButton_data_zak_clicked();
    void on_textEdit_opis_zgloszenia_textChanged();
    void on_textEdit_opis_zakonczenia_textChanged();
    void on_comboBox_sortuj_activated(const QString &arg1);
    void on_actionZestawienie_miesi_czne_triggered();
    void on_lineEdit_szukaj_dzial_textChanged(const QString &arg1);
    void on_lineEdit_szukaj_prac_textChanged(const QString &arg1);
    void on_lineEdit_szukaj_zgloszenie_textChanged(const QString &arg1);
    void on_comboBox_sortuj_osrodki_activated(const QString &arg1);
    void on_pushButton_edytuj_clicked();
    void on_pushButton_zatwierdz_clicked();
    void on_dateEdit_data_zgloszenia_dateChanged(const QDate &date);
    void on_actionKonfiguracja_czynno_ci_2_triggered();
    void on_actionKonfiguracja_u_ytkownik_w_triggered();

    void on_lineEdit_Login_returnPressed();

    void on_lineEdit_Haslo_returnPressed();

signals:
    void dodajczynnoscidz(QString);
    void dodajczynnoscidp(QString);

private:
    Ui::MainWindow *ui;
    dod_zlecenia * nowydodaj_zlecenia;
    kalendarz * nowykalendarz;
    QString sygnalidz;
    QString sygnalidp;
    QString zakonczono;
    QString czy_admin;
    bool listapracselect;
    bool listadzialselect;
    bool listazlecselect;
};

#endif // MAINWINDOW_H
