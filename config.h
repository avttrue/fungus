#ifndef CONFIG_H
#define CONFIG_H

#include <QObject>
#include <QString>
#include <QSettings>

/*!
 * \brief Config - настройки программы с загрузкой из файла конфига и сохранением в файл.
 * Интерфейс над QSettings.
 */
class Config : public QObject
{
    Q_OBJECT

public:
    explicit Config(const QString& in_AppDirectory);

    QString PathApp() const;
    QString PathAppConfig() const;
    int ButtonSize() const;
    void setButtonSize(int value);
    QString DateTimeFormat() const;
    void setDateTimeFormat(const QString &value);
    QString LastDir() const;
    void setLastDir(const QString &value);
    int LogSize() const;
    void setLogSize(int value);
    bool SIMetric() const;
    void setSIMetric(bool value);
    QString SceneBgColor() const;
    void setSceneBgColor(const QString &value);
    qreal SceneScaleStep() const;
    void setSceneScaleStep(qreal value);
    int SceneObjectSize() const;
    void setSceneObjectSize(int value);
    QString SceneColor() const;
    void setSceneColor(const QString &value);
    bool SceneViewAntialiasing() const;
    void setSceneViewAntialiasing(bool value);
    QString SceneBorderColor() const;
    void setSceneBorderColor(const QString &value);
    bool ScenePreFill() const;
    void setScenePreFill(bool value);
    bool SceneBspTreeIndex() const;
    void setSceneBspTreeIndex(bool value);
    int SceneBspTreeDepth() const;
    void setSceneBspTreeDepth(int value);
    Qt::KeyboardModifiers SceneZoomModifier() const;
    void setSceneZoomModifier(const Qt::KeyboardModifiers &value);
    Qt::KeyboardModifiers SceneObjectModifier() const;
    void setSceneObjectModifier(const Qt::KeyboardModifiers &value);
    int SceneSize() const;
    void setSceneSize(int value);
    QString SceneObjectDeadColor() const;
    void setSceneObjectDeadColor(const QString &value);
    bool SceneObjectAgeIndicate() const;
    void setSceneObjectAgeIndicate(bool value);
    QString SceneObjectAlive0Color() const;
    void setSceneObjectAlive0Color(const QString &value);
    QString SceneObjectAlive1Color() const;
    void setSceneObjectAlive1Color(const QString &value);
    QString SceneObjectAlive2Color() const;
    void setSceneObjectAlive2Color(const QString &value);
    QString SceneObjectAlive3Color() const;
    void setSceneObjectAlive3Color(const QString &value);
    QString SceneObjectAlive4Color() const;
    void setSceneObjectAlive4Color(const QString &value);
    QString SceneObjectAlive5Color() const;
    void setSceneObjectAlive5Color(const QString &value);
    QString SceneObjectAlive6Color() const;
    void setSceneObjectAlive6Color(const QString &value);
    QString SceneObjectAlive7Color() const;
    void setSceneObjectAlive7Color(const QString &value);
    QString SceneObjectAlive8Color() const;
    void setSceneObjectAlive8Color(const QString &value);
    QString SceneObjectAlive9Color() const;
    void setSceneObjectAlive9Color(const QString &value);

protected:
    void load();

private:
    QSettings* m_Settings;
    QString m_DateTimeFormat;                   // формат отображения даты и времени
    QString m_PathAppConfig;                    // путь до конфига приложения
    QString m_PathAppDir;                       // путь до приложения
    QString m_LastDir;                          // путь до последнего каталога
    QString m_SceneBgColor;                     // цвет задника сцены
    QString m_SceneColor;                       // цвет сцены
    QString m_SceneBorderColor;                 // цвет рамок сцены
    QString m_SceneObjectDeadColor;             // цвет мёртвой ячейки
    QString m_SceneObjectAlive0Color;           // цвет живой 0 ячейки
    QString m_SceneObjectAlive1Color;           // цвет живой 1 ячейки
    QString m_SceneObjectAlive2Color;           // цвет живой 2 ячейки
    QString m_SceneObjectAlive3Color;           // цвет живой 3 ячейки
    QString m_SceneObjectAlive4Color;           // цвет живой 4 ячейки
    QString m_SceneObjectAlive5Color;           // цвет живой 5 ячейки
    QString m_SceneObjectAlive6Color;           // цвет живой 6 ячейки
    QString m_SceneObjectAlive7Color;           // цвет живой 7 ячейки
    QString m_SceneObjectAlive8Color;           // цвет живой 8 ячейки
    QString m_SceneObjectAlive9Color;           // цвет живой 9 ячейки
    bool m_SceneObjectAgeIndicate;              // отображать возраст живых ячеек
    int m_LogSize;                              // количество строк лога событий
    int m_ButtonSize;                           // размеры кнопок интерфейса
    bool m_SI_metric;                           // использовать систему СИ в отображении размеров файлов
    qreal m_SceneScaleStep;                     // шаг масштабирования сцены
    int m_SceneObjectSize;                      // размер объекта сцены
    bool m_SceneViewAntialiasing;               // SceneView: Antialiasing
    bool m_ScenePreFill;                        // удалить автозаполнение ячейками сцены (эксперимент)
    bool m_SceneBspTreeIndex;                   // использовать BspTreeIndex в сцене
    int m_SceneBspTreeDepth;                    // глубина BspTreeIndex в сцене (0 - авто, 10 - максимум)
    Qt::KeyboardModifiers m_SceneZoomModifier;  // модификатор управления зумом
    Qt::KeyboardModifiers m_SceneObjectModifier;// модификатор создания/удаления объектов
    int m_SceneSize;                            // размер сцены при создании
};

#endif // CONFIG_H
