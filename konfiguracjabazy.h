#ifndef KONFIGURACJABAZY_H
#define KONFIGURACJABAZY_H

#include <QDialog>
#include <QMainWindow>
#include <qmessagebox.h>
#include <QtSql>
#include <QtDebug>
#include <QFileInfo>

namespace Ui {
class konfiguracjaBazy;
}

class konfiguracjaBazy : public QDialog
{
    Q_OBJECT

public:
    explicit konfiguracjaBazy(QWidget *parent = 0);
    ~konfiguracjaBazy();

private slots:
    void on_pushButton_Testuj_clicked();

private:
    Ui::konfiguracjaBazy *ui;
};

#endif // KONFIGURACJABAZY_H
