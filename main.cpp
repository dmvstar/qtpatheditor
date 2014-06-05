#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>
//#include <QLibraryInfo>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QTranslator qtTranslator;

    qtTranslator.load("qt_" + QLocale::system().name());

//    qtTranslator.load("qt_" + QLocale::system().name(),
//                 QLibraryInfo::location(QLibraryInfo::TranslationsPath));

    app.installTranslator(&qtTranslator);

    QTranslator myappTranslator;
    myappTranslator.load(":/qtpatheditor_" + QLocale::system().name());
    app.installTranslator(&myappTranslator);

    //c_out << myappTranslator;

    MainWindow w;
    w.show();

    return app.exec();
}
