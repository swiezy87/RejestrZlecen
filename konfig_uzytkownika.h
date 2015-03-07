#ifndef KONFIG_UZYTKOWNIKA_H
#define KONFIG_UZYTKOWNIKA_H

#include <QDialog>

namespace Ui {
class Konfig_uzytkownika;
}

class Konfig_uzytkownika : public QDialog
{
    Q_OBJECT

public:
    explicit Konfig_uzytkownika(QWidget *parent = 0);
    ~Konfig_uzytkownika();

private slots:
    void on_pushButton_Dodaj_clicked();
    void on_pushButton_Wyszukaj_clicked();
    void on_tableView_uzytkownicy_activated(const QModelIndex &index);
    void on_pushButton_Edytuj_clicked();
    void on_pushButton_zmienhaslo_clicked();
    void on_pushButton_OK_clicked();
    void odswiez_tabeleuser();

    void on_lineEdit_szukaj_textChanged(const QString &arg1);

private:
    Ui::Konfig_uzytkownika *ui;

};

#endif // KONFIG_UZYTKOWNIKA_H
