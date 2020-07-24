#include "mainwindow.h"
#include "properties.h"
#include "helper.h"

#include <QApplication>
#include <QTextCodec>
#include <QDebug>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);
    application.setStyleSheet(getTextFromRes(":/resources/main.css"));

    QTextCodec::setCodecForLocale(QTextCodec::codecForName(TEXT_CODEC.toLatin1()));
    config = new Config(application.applicationDirPath());

    // копируются пресеты
    bool ok = true;
    copyResources(":/resources/presets", config->PathPresetDirectory(), config->RewriteResource(), &ok);
    if(!ok)
    {
        qCritical() << "Error at presets coping to" << config->PathPresetDirectory();
        QMessageBox::critical(nullptr, QObject::tr("Error"),
                              QObject::tr("Error at presets coping to '%1'").arg(config->PathPresetDirectory()));
    }

    MainWindow window;
    window.show();

    return application.exec();
}
