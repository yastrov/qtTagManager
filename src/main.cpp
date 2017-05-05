#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // Set for QSettings
    a.setApplicationDisplayName(QMAKE_TARGET_PRODUCT);
    a.setApplicationName(QMAKE_TARGET_PRODUCT);
    a.setApplicationVersion(APP_VERSION);
    a.setOrganizationName(QMAKE_TARGET_COMPANY);
    // End
    MainWindow w;
    w.show();

    return a.exec();
}
