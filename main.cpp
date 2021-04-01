#include "mainwindow.h"
#include "properties.h"
#include "helpers/helper.h"
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

    if(!CreateDir(config->PathPresetsDir()))
        config->setPathPresetsDir(config->PathAppDir());

    // каталог проектов
    config->setPathPojectsDir(config->PathAppDir() + QDir::separator() + PROJECTS_DIRECTORY);
    if(!CreateDir(config->PathPojectsDir()))
        config->setPathPojectsDir(config->PathAppDir());

    // каталог правил
    config->setPathRulesDir(config->PathAppDir() + QDir::separator() + RULES_DIRECTORY);
    if(!CreateDir(config->PathRulesDir()))
        config->setPathRulesDir(config->PathAppDir());

    // каталог логов
    config->setPathLogsDir(config->PathAppDir() + QDir::separator() + LOG_DIRECTORY);
    if(!CreateDir(config->PathLogsDir()))
        config->setPathLogsDir(config->PathAppDir());

    // каталог отчётов
    config->setPathReportsDir(config->PathAppDir() + QDir::separator() + REPORTS_DIRECTORY);
    if(!CreateDir(config->PathReportsDir()))
        config->setPathReportsDir(config->PathAppDir());
}

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
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

    qInfo() << "App version:" << APP_VERSION;
    qInfo() << "Format version:" << FORMAT_VERSION;
    qInfo() << "Git, build date:" << GIT_VERS << BUILD_DATE;
    qInfo() << "System info:" << getSystemInfo();
    qInfo() << "Qt version:" << QT_VERSION_STR;

    qInfo() << "PathAppDirectory:" << config->PathAppDir();
    qInfo() << "PathAppConfig:" << config->PathAppConfig();
    qInfo() << "PathPresetDirectory:" << config->PathPresetsDir();
    qInfo() << "PathLogsDirectory:" << config->PathLogsDir();

    // копируются пресеты
    {
        bool ok = true;
        copyResources(":/resources/presets", config->PathPresetsDir(), config->RewriteResource(), &ok);
        if(!ok) qCritical() << "Error at presets coping to" << config->PathPresetsDir();
    }
    // копируются правила
    {
        bool ok = true;
        copyResources(":/resources/rules", config->PathRulesDir(), config->RewriteResource(), &ok);
        if(!ok) qCritical() << "Error at rules coping to" << config->PathRulesDir();
    }

    SplashScreen splash;
    splash.show();

    MainWindow window;

    QEventLoop loop;

    auto close_splash = [&application, &window, &splash, &loop]()
    {
        splash.finish(&window);
        window.show();
        application.setActiveWindow(&window);
        loop.quit();
    };

    QTimer::singleShot(config->SplashTime(), &application, close_splash);
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
