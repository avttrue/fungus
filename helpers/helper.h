#ifndef HELPER_H
#define HELPER_H

#include <QString>
#include <QVariant>

/*!
 * \brief getTextFromRes - получить текстовый файл из ресурсов приложения
 */
QString getTextFromRes(const QString& path);

/*!
 * \brief textToFile - записать текст в файл
 */
bool textToFile(const QString& text, const QString& path);

/*!
 * \brief fileToText - считать файл как текст.
 */
QString fileToText(const QString& path, bool* ok = nullptr);

/*!
 * \brief longTimeToString - перевести время в формате long (MSecsSinceEpoch) в строку в формате format.
 */
QString longTimeToString(long datetime, const QString& format);

/*!
 * \brief humanReadableByteCount - перевести количество байт в удобный формат.
 * \param bytes - количество байт
 * \param si - SI или BINARY формат
 * \return количество байт в виде строки
 */
QString humanReadableByteCount(long long bytes, bool si = true);

void copyProperty(QObject *source, QObject *target);

qreal calcAverage(qreal oldAverage, uint iteration, uint value);

QString getSystemInfo();

void copyResources(const QString& outPath, const QString& inPath, bool rewrite, bool* ok);

QString BoolToString(bool value);

bool CreateDir(const QString& path);

bool OpenUrl(const QString& path);
bool OpenUrl(const QUrl& url);

QString GenerateFilePath(const QString &ext, const QString &dir, const QString &name, const QString &rg, const QString &add = "");

#endif // HELPER_H
