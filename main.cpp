#include "mainwindow.h"
#include "properties.h"
#include "helper.h"
#include "splashscreen.h"

#include <QApplication>
#include <QTextCodec>
#include <QDebug>
#include <QDir>
#include <QDateTime>
#include <QTimer>

// подготовка обработчика консоли
static QScopedPointer<QFile> m_logFile;
void consoleOut(QtMsgType type, const QMessageLogContext &context, const QString &msg);

// создание каталогов
void createDirectories()
{
    // каталог пресетов
    config->setPathPresetDir(config->PathAppDir() + QDir::separator() + PRESET_DIRECTORY);
    if(!QDir().exists(config->PathPresetDir()) && !QDir().mkpath(config->PathPresetDir()))
    {
        qCritical() << "Directory not exist and cannot be created:" << config->PathPresetDir();
        config->setPathPresetDir(config->PathAppDir());
    }
    else
    {
        auto p = QFile(config->PathPresetDir()).permissions();
        if(!QFile::setPermissions(config->PathPresetDir(), p |
                                  QFileDevice::ReadOwner |
                                  QFileDevice::WriteOwner))
            qCritical() << "Cannot set permissions to directory:" << config->PathPresetDir();
        else qInfo() << "Directory" << config->PathPresetDir() << "ready";
    }

    // каталог логов
    config->setPathLogsDir(config->PathAppDir() + QDir::separator() + LOG_DIRECTORY);
    if(!QDir().exists(config->PathLogsDir()) && !QDir().mkpath(config->PathLogsDir()))
    {
        qCritical() << "Directory not exist and cannot be created:" << config->PathLogsDir();
        config->setPathLogsDir(config->PathAppDir());
    }
    else
    {
        auto p = QFile(config->PathLogsDir()).permissions();
        if(!QFile::setPermissions(config->PathLogsDir(), p |
                                  QFileDevice::ReadOwner |
                                  QFileDevice::WriteOwner))
            qCritical() << "Cannot set permissions to directory:" << config->PathLogsDir();
        else qInfo() << "Directory" << config->PathLogsDir() << "ready";
    }
}

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);
    application.setStyleSheet(getTextFromRes(":/resources/main.css"));

    QTextCodec::setCodecForLocale(QTextCodec::codecForName(TEXT_CODEC.toLatin1()));
    config = new Config(application.applicationDirPath());
    createDirectories();

    if(config->WriteLogsToFile())
    {
        auto name = QDateTime::currentDateTime().toString(LOG_FILE_NAME_FORMAT).append('.');
        auto file = config->PathLogsDir() + QDir::separator() + name + LOG_FILE_EXTENSION.toLower();
        m_logFile.reset(new QFile(file));
        m_logFile.data()->open(QFile::Append | QFile::Text);
        qInstallMessageHandler(consoleOut);
    }
    qInfo() << "PathAppDirectory:" << config->PathAppDir();
    qInfo() << "PathAppConfig:" << config->PathAppConfig();
    qInfo() << "PathPresetDirectory:" << config->PathPresetDir();
    qInfo() << "PathLogsDirectory:" << config->PathLogsDir();

    // копируются пресеты
    bool ok = true;
    copyResources(":/resources/presets", config->PathPresetDir(), config->RewriteResource(), &ok);
    if(!ok)
        qCritical() << "Error at presets coping to" << config->PathPresetDir();

    SplashScreen splash;
    splash.show();

    MainWindow window;

    QEventLoop loop;
    QTimer::singleShot(config->SplashTime(), [&application, &window, &splash, &loop]()
    {
        splash.finish(&window);
        window.show();
        application.setActiveWindow(&window);
        loop.quit();
    });
    loop.exec(QEventLoop::ExcludeUserInputEvents);

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
