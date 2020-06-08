#ifndef FIELDSERVICE_H
#define FIELDSERVICE_H

#include <QVariant>
#include <QObject>

class Kernel: public QObject
{
    Q_OBJECT
public:
    Kernel() = delete;
    /*!
     * \brief XenoReaction enum - реакция ячейки на соседнюю ячейку с иными правилами ("иную")
     */
    enum class CellXenoReaction: int
    {
        AsOwn = 0,          // относиться как к своей
        Ignore,             // игнорировать
        Death,              // умирать
        Bomb                // взрываться
    };
    Q_ENUM(CellXenoReaction)

    enum class CellState: int
    {
        Dead = 0,           // мертва (пустая)
        Alive,              // жива
        Cursed              // отравлена
    };
    Q_ENUM(CellState)
};

/*!
 * \brief countKernelEnum - получить количество позиций в enum'е
 * \param enumname - имя enum'а
 */
int countKernelEnum(const QString& enumname);

/*!
 * \brief listKernelEnum - получить список позиций в enum'е
 * \param enumname - имя enum'а
 */
QStringList listKernelEnum(const QString& enumname);

/*!
 * \brief getNameKernelEnum - получить имя члена enum'а по индексу
 * \param enumname - имя enum'а
 * \param index - индекс искомого члена enum'а
 */
QString getNameKernelEnum(const QString& enumname, int index);

/*!
 * \brief getPropertiesList - получить список пользовательских QProperty объекта в виде <имя, тип>
 */
QMap<QString, QVariant::Type> getPropertiesList(QObject *object);

typedef QVector<QPair<QString, QPair<QString,int>>> CellActivity;
Q_DECLARE_METATYPE(CellActivity);
Q_DECLARE_METATYPE(Kernel::CellXenoReaction)
Q_DECLARE_METATYPE(Kernel::CellState)

#endif // FIELDSERVICE_H
