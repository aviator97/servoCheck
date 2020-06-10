#include <QApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(mdi);

    QApplication app(argc, argv);
    QCoreApplication::setApplicationName("servo_check");
    QCoreApplication::setOrganizationName("Albatros");
    QCoreApplication::setApplicationVersion(QT_VERSION_STR);
    QCommandLineParser parser;
    parser.setApplicationDescription("Qt MDI Example");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("file", "The file to open.");
    parser.process(app);

    MainWindow mainWin;
    foreach (const QString &fileName, parser.positionalArguments())
        mainWin.openFile(fileName);
    mainWin.show();
    return app.exec();
}
