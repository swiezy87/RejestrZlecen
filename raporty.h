#ifndef RAPORTY_H
#define RAPORTY_H

#include <QDialog>
#include <qlistwidget.h>
#include <QSql>
#include <kalendarz.h>
#include <QDate>
//#include <QtPrintSupport/QPrinter>
//#include <QtPrintSupport/QPrintDialog>
#include <QPrintDialog>
#include <QPrinter>

namespace Ui {
class raporty;
}

class raporty : public QDialog
{
    Q_OBJECT

public:
    explicit raporty(QWidget *parent = 0);
    ~raporty();
    void odswiez_osrodki_punkty();
    void print();
    void print2();

private slots:
    void on_toolButton_data_od_clicked();
    void on_toolButton_data_do_clicked();
    void ustaw_date_od_kalendar(QDate);
    void ustaw_date_do_kalendar(QDate);

    void on_pushButton_druk_clicked();

    void on_pushButton_druk2_clicked();

    void on_tableView_osrodki_punkty_clicked(const QModelIndex &index);

private:
    Ui::raporty *ui;
    QListWidget * item;
    kalendarz * nowykalendarz;
    QString ido;
    QString numer;
};

#endif // RAPORTY_H
