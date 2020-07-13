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
        Dead = 0,           // мертва (пустая)
        Alive,              // жива
        Cursed              // отравлена (может принять такое значение при смерти или взрыве)
    };
    Q_ENUM(CellState)

    /*!
     * \brief The CellActivityType enum - тип активности ячейки
     */
    enum class ActivityType: int
    {
        Birth = 0,          // рождение
        Death,              // смерть
        Bomb                // взрыв
    };
    Q_ENUM(ActivityType)

    /*!
     * \brief The CellActivityTarget enum - цель проверок активности
     */
    enum class ActivityTarget: int
    {
        Self = 0,           // сама ячейка
        Near                // соседние ячейки любые
    };
    Q_ENUM(ActivityTarget)

    /*!
     * \brief The CellActivityOperator enum - оператор проверок активности
     */
    enum class ActivityOperator: int
    {
        Equal = 0,          // равен
        More,               // больше
        Less                // меньше
    };
    Q_ENUM(ActivityOperator)

    /*!
     * \brief The ActivityOperand enum - проверяемй параметр
     */
    enum class ActivityOperand: int
    {
        Count = 0,          // количество
        Age                 // возраст
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
