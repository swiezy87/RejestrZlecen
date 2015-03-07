#ifndef KALENDARZ_H
#define KALENDARZ_H

#include <QDialog>
#include <QDate>

namespace Ui {
class kalendarz;
}

class kalendarz : public QDialog
{
    Q_OBJECT

public:
    explicit kalendarz(QWidget *parent = 0);
    ~kalendarz();

private slots:
    void on_calendarWidget_clicked(const QDate &date);

signals:
    void sygnal_daty(QDate);

private:
    Ui::kalendarz *ui;
};

#endif // KALENDARZ_H
