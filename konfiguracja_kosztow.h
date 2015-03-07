#ifndef KONFIGURACJA_KOSZTOW_H
#define KONFIGURACJA_KOSZTOW_H

#include <QDialog>

namespace Ui {
class Konfiguracja_Kosztow;
}

class Konfiguracja_Kosztow : public QDialog
{
    Q_OBJECT

public:
    explicit Konfiguracja_Kosztow(QWidget *parent = 0);
    ~Konfiguracja_Kosztow();

private slots:
    void on_pushButton_dodaj_clicked();
    void on_tableView_osrodki_activated(const QModelIndex &index);
    void on_pushButton_edytuj_clicked();
    void on_pushButton_szukaj_clicked();
    void odswiez_tabele();

    void on_lineEdit_szukaj_textChanged(const QString &arg1);

private:
    Ui::Konfiguracja_Kosztow *ui;

};

#endif // KONFIGURACJA_KOSZTOW_H
