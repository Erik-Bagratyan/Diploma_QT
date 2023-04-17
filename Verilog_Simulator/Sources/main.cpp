#include "Headers/mainwindow.h"
#include <QApplication>

#include <QDebug>
#include <QMap>

#include <memory>

void print_test_branch(QVector<QMap<QString, size_t>> primaryInputs);

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow mainWindow;

    mainWindow.show();

    return a.exec();
}
