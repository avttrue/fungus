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
    QString SceneSelectColor() const;
    void setSceneSelectColor(const QString &value);
    QString SceneObjectCurseColor() const;
    void setSceneObjectCurseColor(const QString &value);
    int CellInfoWindowHeight() const;
    void setCellInfoWindowHeight(int value);
    int CellInfoWindowWidth() const;
    void setCellInfoWindowWidth(int value);

protected:
    void load();

private:
    QSettings* m_Settings;
    QString m_DateTimeFormat;                   // формат отображения даты и времени
    QString m_PathAppConfig;                    // путь до конфига приложения
    QString m_PathAppDir;                       // путь до приложения
    QString m_LastDir;                          // путь до последнего каталога
    QString m_SceneSelectColor;                 // цвет выбранного объекта сцены
    QString m_SceneBgColor;                     // цвет задника сцены
    QString m_SceneColor;                       // цвет сцены
    QString m_SceneBorderColor;                 // цвет рамок сцены
    QString m_SceneObjectDeadColor;             // цвет мёртвой ячейки
    QString m_SceneObjectCurseColor;            // цвет отравленной ячейки
    qreal m_SceneScaleStep;                     // шаг масштабирования сцены
    int m_LogSize;                              // количество строк лога событий
    int m_ButtonSize;                           // размеры кнопок интерфейса    
    int m_SceneObjectSize;                      // размер объекта сцены
    int m_SceneBspTreeDepth;                    // глубина BspTreeIndex в сцене (0 - авто, 10 - максимум)
    int m_SceneSize;                            // размер сцены при создании
    int m_CellInfoWindowHeight;                 // размеры окна информации о ячейке
    int m_CellInfoWindowWidth;                  // размеры окна информации о ячейке
    Qt::KeyboardModifiers m_SceneZoomModifier;  // модификатор управления зумом
    Qt::KeyboardModifiers m_SceneObjectModifier;// модификатор создания/удаления объектов
    bool m_SceneObjectAgeIndicate;              // отображать возраст живых ячеек
    bool m_SI_metric;                           // использовать систему СИ в отображении размеров файлов
    bool m_SceneViewAntialiasing;               // SceneView: Antialiasing
    bool m_ScenePreFill;                        // удалить автозаполнение ячейками сцены (эксперимент)
    bool m_SceneBspTreeIndex;                   // использовать BspTreeIndex в сцене
};

#endif // CONFIG_H
