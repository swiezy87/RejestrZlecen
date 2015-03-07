#include "mainwindow.h"
#include <QApplication>
#include <qmessagebox.h>
#include <QtSql>
#include <QtDebug>
#include <QFileInfo>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.showMaximized();// Pokazuje okno zmaksymalizowane

    return a.exec();
}
