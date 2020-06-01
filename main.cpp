#include "mainwindow.h"
#include "properties.h"
#include "helper.h"

#include <QApplication>
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);
    application.setStyleSheet(getTextFromRes(":/resources/main.css"));

    QTextCodec::setCodecForLocale(QTextCodec::codecForName(TEXT_CODEC.toLatin1()));
    config = new Config(application.applicationDirPath());

    MainWindow window;
    window.show();

    return application.exec();
}
