#ifndef KONFIG_CZYNNOSCI_H
#define KONFIG_CZYNNOSCI_H

#include <QDialog>

namespace Ui {
class konfig_czynnosci;
}

class konfig_czynnosci : public QDialog
{
    Q_OBJECT

public:
    explicit konfig_czynnosci(QWidget *parent = 0);
    ~konfig_czynnosci();

private slots:
    void on_pushButton_szukaj_clicked();
    void on_pushButton_dodaj_clicked();
    void on_tableView_czynnosci_activated(const QModelIndex &index);
    void on_pushButton_edytuj_clicked();
    void odswiez_tabele();
    void on_lineEdit_szukaj_textChanged(const QString &arg1);

private:
    Ui::konfig_czynnosci *ui;

};

#endif // KONFIG_CZYNNOSCI_H
