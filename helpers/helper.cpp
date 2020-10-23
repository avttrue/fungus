#include "helper.h"

#include <sys/utsname.h>
#include <QFile>
#include <QDebug>
#include <QDateTime>
#include <QtMath>
#include <QMetaProperty>
#include <QDir>

QString getTextFromRes(const QString& path)
{
    QFile file(path);
    if(file.open(QFile::ReadOnly))
    {
        QString text(file.readAll());
        return text;
    }
    qCritical()<< __func__ << ": Resurce not found:" << path;
    return "";
}

bool textToFile(const QString& text, const QString& path)
{
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qCritical()<< __func__ << ": File not open for writing:" << path;
        return false;
    }

    QTextStream stream(&file);
    stream << text;
    file.close();

    if (stream.status() != QTextStream::Ok)
    {
        qCritical()<< __func__ << ": Error at file saving:" << path;
        return false;
    }
    return true;
}

QString longTimeToString(long datetime, const QString& format)
{
    QDateTime dt;
    dt.setMSecsSinceEpoch(datetime);
    return dt.toString(format);
}

QString humanReadableByteCount(long long bytes, bool si)
{
    int unit = si ? 1000 : 1024;
    if (bytes < unit)
        return QString("%1 B").arg(QString::number(bytes));

    int exp = static_cast<int>(qLn(bytes) / qLn(unit));
    QString pre = si ? "kMGTPE" : "KMGTPE";

    return QString("%1 %2%3B").
            arg(bytes / qPow(unit, exp), 0, 'f', 1, '0').
            arg(pre[exp - 1]).
            arg(si ? "" : "i");
}

QString fileToText(const QString& path, bool* ok)
{
    QString text;
    QFile file(path);

    if (!file.exists())
    {
        qCritical()<< __func__ << ": File not found:" << path;
        if(ok) *ok = false;
        return "";
    }

    if (!file.open(QIODevice::ReadOnly | QFile::Text))
    {
        qCritical()<< __func__ << ": File not open for reading:" << path;
        if(ok) *ok = false;
        return "";
    }

    QTextStream stream(&file);
    text = stream.readAll();
    file.close();
    if(ok) *ok = true;
    return text;
}

QString getSystemInfo()
{
    QString result;

    struct utsname buf{};
    if (uname(&buf) == 0)
    {
        result.append(buf.sysname).append(" ").
                append(buf.version).append(" ").
                append(buf.release).append(" ").
                append(buf.machine);
        return result;
    }
    return "unknown";
}

qreal calcAverage(qreal oldAverage, uint iteration, uint value)
{
    auto iteration_1 = static_cast<qreal>(iteration - 1);
    return iteration < 2
            ? value
            : (oldAverage + static_cast<qreal>(value) / iteration_1) /
              (static_cast<qreal>(iteration) / iteration_1);
}

void copyProperty(QObject *source, QObject *target)
{
    for(int i = source->metaObject()->propertyOffset(); i < source->metaObject()->propertyCount(); ++i)
    {
        auto sp  = source->metaObject()->property(i);
        sp.write(target, source->property(sp.name()));
    }
}

void copyResources(const QString& outPath, const QString& inPath, bool rewrite, bool* ok)
{
    if(!QDir().exists(inPath) && !QDir().mkpath(inPath))
    {
        qCritical() << "Directory not exist and cannot be created:" << inPath;
        *ok = false; return;
    }

    QDir outdir(outPath);
    if(!outdir.exists())
    {
        qCritical() << "Path not exist:" << outPath;
        *ok = false; return;
    }

    for(auto filename: outdir.entryList())
    {
        auto newfilename = inPath + QDir::separator() + filename;
        QFile file(outdir.path() + "/" + filename);
        QFileInfo fi(file);

        if(fi.isFile())
        {
            if(rewrite && QFile::exists(newfilename) && !QFile::remove(newfilename))
            {
                qCritical() << "Unable to remove file" << newfilename;
                *ok = false; return;
            }

            if(!QFile::exists(newfilename) && !file.copy(newfilename))
            {
                qCritical() << "Unable to write file" << newfilename << "from" << file.fileName();
                *ok = false; return;
            }

            auto p = QFile(newfilename).permissions();
            if(!QFile::setPermissions(newfilename, p |
                                      QFileDevice::ReadOwner |
                                      QFileDevice::WriteOwner))
                qCritical() << "Cannot set permissions to file:" << newfilename;
            else qInfo() << "Resource" << newfilename << "ready";
        }
        else if(fi.isDir())
        {
            if(!QDir().exists(newfilename) && !QDir().mkpath(newfilename))
            {
                qCritical() << "Directory not exist and cannot be created:" << newfilename;
                *ok = false; return;
            }
            auto p = QFile(newfilename).permissions();
            if(!QFile::setPermissions(newfilename, p |
                                      QFileDevice::ReadOwner |
                                      QFileDevice::WriteOwner))
                qCritical() << "Cannot set permissions to directory:" << newfilename;
            else qInfo() << "Directory" << newfilename << "ready";

            auto newOutPath = fi.filePath();
            copyResources(newOutPath, newfilename, rewrite, ok);
            if(!(*ok)) return;
        }
    }
}

QString BoolToString(bool value) { return value ? "[+]" : "[-]"; }

bool CreateDir(const QString &path)
{
    if(!QDir().exists(path) && !QDir().mkpath(path))
    {
        qCritical() << "Directory not exist and cannot be created:" << path;
        return false;
    }

    auto p = QFile(path).permissions();
    if(!QFile::setPermissions(path, p |
                              QFileDevice::ReadOwner |
                              QFileDevice::WriteOwner))
    {
        qCritical() << "Cannot set permissions to directory:" << path;
        return false;
    }

    qInfo() << "Directory" << path << "ready";
    return true;
}
