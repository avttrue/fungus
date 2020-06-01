#ifndef HELPER_H
#define HELPER_H

#include <QColor>
#include <QString>

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

QString getSystemInfo();

#endif // HELPER_H
