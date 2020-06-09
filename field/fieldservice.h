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
     * \brief The CellState enum - состояние ячейки
     */
    enum class CellState: int
    {
        Dead = 0,           // мертва (пустая)
        Alive,              // жива
        Cursed              // отравлена (может принять такое значение при смерти или взрыве)
    };
    Q_ENUM(CellState)

    /*!
     * \brief The CellActivityType enum - тип активности ячейки
     */
    enum class CellActivityType: int
    {
        Birth = 0,          // рождение
        Death,              // смерть
        Bomb                // взрыв
    };
    Q_ENUM(CellActivityType)

    /*!
     * \brief The CellActivityTarget enum - цель проверок активности
     */
    enum class CellActivityTarget: int
    {
        Self = 0,           // сама ячейка
        Near,               // соседние ячейки любые
        NearAlien,          // соседние ячейки чужие
        NearAlly            // соседние ячейки свои
    };
    Q_ENUM(CellActivityTarget)
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
 * \brief CellActivity - список активностей ячейки:
 * {CellActivityType, CellActivityTarget, [оператор: <,>,=], [значение]};
 * проверяется на условие выполнения по-очереди начиная с первой, до первого успешного
 */
typedef QVector<QVector<QVariant>> CellActivity;

Q_DECLARE_METATYPE(Kernel::CellActivityTarget)
Q_DECLARE_METATYPE(Kernel::CellActivityType)
Q_DECLARE_METATYPE(Kernel::CellState)
Q_DECLARE_METATYPE(CellActivity)

#endif // FIELDSERVICE_H
