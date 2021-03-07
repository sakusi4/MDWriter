#include "mainwindow.h"

#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{    

    QApplication a(argc, argv);
    MainWindow w;

    QFile file("./VisualScript.qss");
    file.open(QFile::ReadOnly);

    QString styleSheet = QLatin1String(file.readAll());

    //setup stylesheet
    a.setStyleSheet(styleSheet);

    w.show();
    return a.exec();
}
