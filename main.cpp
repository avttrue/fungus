#include "mainwindow.h"
#include "properties.h"
#include "helper.h"

#include <QApplication>
#include <QTextCodec>
#include <QDebug>
#include <QMessageBox>
#include <QDir>
#include <QDateTime>

// подготовка обработчика консоли
static QScopedPointer<QFile> m_logFile;
void consoleOut(QtMsgType type, const QMessageLogContext &context, const QString &msg);

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);
    application.setStyleSheet(getTextFromRes(":/resources/main.css"));

    QTextCodec::setCodecForLocale(QTextCodec::codecForName(TEXT_CODEC.toLatin1()));
    config = new Config(application.applicationDirPath());

    if(config->WriteLogsToFile())
    {
        auto name = QDateTime::currentDateTime().toString(LOG_FILE_NAME_FORMAT).append('.');
        auto file = config->PathLogsDirectory() + QDir::separator() + name + LOG_FILE_EXTENSION.toLower();
        m_logFile.reset(new QFile(file));
        m_logFile.data()->open(QFile::Append | QFile::Text);
        qInstallMessageHandler(consoleOut);
    }

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

// обработчик консоли
void consoleOut(QtMsgType msgtype, const QMessageLogContext &context, const QString &msg)
{
    QTextStream log_out(m_logFile.data());
    log_out.setCodec(QTextCodec::codecForName(TEXT_CODEC.toLatin1()));
    auto time = QDateTime::currentDateTime().toString(config->DateTimeFormat().append("\t"));
    QString type;
    switch (msgtype)
    {
    case QtDebugMsg:
    { type = "DBG:\t"; break; }
    case QtInfoMsg:
    { type = "INF:\t"; break; }
    case QtWarningMsg:
    { type = "WRN:\t"; break; }
    case QtCriticalMsg:
    { type = "CRT:\t"; break; }
    case QtFatalMsg:
    { type = "FTL:\t"; break; }
    }
    log_out << time << type << context.file << "\t" << context.line << "\t" << msg << Qt::endl;
    log_out.flush();
}
