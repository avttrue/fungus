#ifndef KERNEL_H
#define KERNEL_H

#include <QObject>
#include <QVariant>

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
        DEAD = 0,           // мертва (пустая)
        ALIVE,              // жива
        CURSED              // отравлена
    };
    Q_ENUM(CellState)

    /*!
     * \brief The CellActivityType enum - тип активности ячейки
     */
    enum class ActivityType: int
    {
        BIRTH = 0,          // рождение
        DEATH,              // смерть
        CURSE               // отравление
    };
    Q_ENUM(ActivityType)

    /*!
     * \brief The CellActivityTarget enum - цель проверок активности
     */
    enum class ActivityTarget: int
    {
        SELF = 0,           // сама ячейка
        NEAR                // соседние ячейки любые
    };
    Q_ENUM(ActivityTarget)

    /*!
     * \brief The CellActivityOperator enum - оператор проверок активности
     */
    enum class ActivityOperator: int
    {
        EQUAL = 0,          // равен
        MORE,               // больше
        LESS                // меньше
    };
    Q_ENUM(ActivityOperator)

    /*!
     * \brief The ActivityOperand enum - проверяемй параметр
     */
    enum class ActivityOperand: int
    {
        COUNT = 0,          // количество
        AGE,                // возраст
        GEN                 // поколение
    };
    Q_ENUM(ActivityOperand)
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

QString ActivityElementToString(const QVector<QVariant>& activity);

/*!
 * \brief Activity - список активностей ячейки:
 * {ActivityType, SelfState, ActivityTarget, TargetState, ActivityOperand, ActivityOperator, [значение]};
 *
 * Что сделать (ActivityType)
 * если
 * своё состояние (SelfState) и
 * проверяем цель (ActivityTarget)
 * состояние цели (TargetState)
 * проверяем у цели что (ActivityOperand)
 * оператор проверки (ActivityOperator)
 * значение проверки
 * выполняется по-очереди начиная с первого правила
 */
typedef QVector<QVector<QVariant>> Activity;

Q_DECLARE_METATYPE(Kernel::ActivityTarget)
Q_DECLARE_METATYPE(Kernel::ActivityType)
Q_DECLARE_METATYPE(Kernel::ActivityOperator)
Q_DECLARE_METATYPE(Kernel::ActivityOperand)
Q_DECLARE_METATYPE(Kernel::CellState)
Q_DECLARE_METATYPE(Activity)

#endif // KERNEL_H
