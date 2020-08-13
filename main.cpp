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
    config->setPathPresetsDir(config->PathAppDir() + QDir::separator() + PRESET_DIRECTORY);
    if(!QDir().exists(config->PathPresetsDir()) && !QDir().mkpath(config->PathPresetsDir()))
    {
        qCritical() << "Directory not exist and cannot be created:" << config->PathPresetsDir();
        config->setPathPresetsDir(config->PathAppDir());
    }
    else
    {
        auto p = QFile(config->PathPresetsDir()).permissions();
        if(!QFile::setPermissions(config->PathPresetsDir(), p |
                                  QFileDevice::ReadOwner |
                                  QFileDevice::WriteOwner))
            qCritical() << "Cannot set permissions to directory:" << config->PathPresetsDir();
        else qInfo() << "Directory" << config->PathPresetsDir() << "ready";
    }

    // каталог проектов
    config->setPathPojectsDir(config->PathAppDir() + QDir::separator() + PROJECTS_DIRECTORY);
    if(!QDir().exists(config->PathPojectsDir()) && !QDir().mkpath(config->PathPojectsDir()))
    {
        qCritical() << "Directory not exist and cannot be created:" << config->PathPojectsDir();
        config->setPathPojectsDir(config->PathAppDir());
    }
    else
    {
        auto p = QFile(config->PathPojectsDir()).permissions();
        if(!QFile::setPermissions(config->PathPojectsDir(), p |
                                  QFileDevice::ReadOwner |
                                  QFileDevice::WriteOwner))
            qCritical() << "Cannot set permissions to directory:" << config->PathPojectsDir();
        else qInfo() << "Directory" << config->PathPojectsDir() << "ready";
    }

    // каталог правил
    config->setPathRulesDir(config->PathAppDir() + QDir::separator() + RULES_DIRECTORY);
    if(!QDir().exists(config->PathRulesDir()) && !QDir().mkpath(config->PathRulesDir()))
    {
        qCritical() << "Directory not exist and cannot be created:" << config->PathRulesDir();
        config->setPathRulesDir(config->PathAppDir());
    }
    else
    {
        auto p = QFile(config->PathRulesDir()).permissions();
        if(!QFile::setPermissions(config->PathRulesDir(), p |
                                  QFileDevice::ReadOwner |
                                  QFileDevice::WriteOwner))
            qCritical() << "Cannot set permissions to directory:" << config->PathRulesDir();
        else qInfo() << "Directory" << config->PathRulesDir() << "ready";
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
    qInfo() << "PathPresetDirectory:" << config->PathPresetsDir();
    qInfo() << "PathLogsDirectory:" << config->PathLogsDir();

    // копируются пресеты
    bool ok = true;
    copyResources(":/resources/presets", config->PathPresetsDir(), config->RewriteResource(), &ok);
    if(!ok)
        qCritical() << "Error at presets coping to" << config->PathPresetsDir();

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
    //log_out.flush();
}
