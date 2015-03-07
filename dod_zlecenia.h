#ifndef DOD_ZLECENIA_H
#define DOD_ZLECENIA_H

#include <QDialog>

namespace Ui {
class dod_zlecenia;
}

class dod_zlecenia : public QDialog
{
    Q_OBJECT

public:
    explicit dod_zlecenia(QWidget *parent = 0);
    ~dod_zlecenia();

private slots:
    void on_pushButton_szukaj_clicked();
    void setidz(QString);
    void setidp(QString);
    void on_pushButton_szukaj_wykonane_clicked();
    void on_tableView_czynnosci_clicked(const QModelIndex &index);
    void on_tableView_czynnosci_wykonane_clicked(const QModelIndex &index);
    void on_lineEdit_szukaj_textChanged(const QString &arg1);
    void odswiez_tabele_czynnosci();
    void odswiez_tabele_czynnosci_wykonane();


private:
    Ui::dod_zlecenia *ui;
    QString idzzmainwindow;
    QString idpzmainwindow;

};

#endif // DOD_ZLECENIA_H
