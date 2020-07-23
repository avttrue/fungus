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
    int SceneCellSize() const;
    void setSceneCellSize(int value);
    bool SceneViewAntialiasing() const;
    void setSceneViewAntialiasing(bool value);
    Qt::KeyboardModifiers SceneZoomModifier() const;
    void setSceneZoomModifier(const Qt::KeyboardModifiers &value);
    int SceneFieldSize() const;
    void setSceneFieldSize(int value);
    QString SceneCellDeadColor() const;
    void setSceneCellDeadColor(const QString &value);
    bool SceneCellAgeIndicate() const;
    void setSceneCellAgeIndicate(bool value);
    QString SceneSelectColor() const;
    void setSceneSelectColor(const QString &value);
    QString SceneCellCurseColor() const;
    void setSceneCellCurseColor(const QString &value);
    int CellInfoWindowHeight() const;
    void setCellInfoWindowHeight(int value);
    int CellInfoWindowWidth() const;
    void setCellInfoWindowWidth(int value);
    int SceneCalculatingMinPause() const;
    void setSceneCalculatingMinPause(int value);
    QString SceneViewUpdateMode() const;
    void setSceneViewUpdateMode(const QString &value);
    QString SceneFieldThreadPriority() const;
    void setSceneFieldThreadPriority(const QString &value);
    int SceneItemZValue() const;
    void setSceneItemZValue(int value);
    Qt::KeyboardModifiers SceneToolTipModifier() const;
    void setSceneToolTipModifier(const Qt::KeyboardModifiers &value);
    int FieldInfoWindowHeight() const;
    void setFieldInfoWindowHeight(int value);
    int FieldInfoWindowWidth() const;
    void setFieldInfoWindowWidth(int value);
    QString SceneImageFileFormat() const;
    void setSceneImageFileFormat(const QString &value);
    bool WindowShowFieldInfo() const;
    void setWindowShowFieldInfo(bool value);
    Qt::KeyboardModifiers SceneMultiselModifier() const;
    void setSceneMultiselModifier(const Qt::KeyboardModifiers &value);
    int SceneMultiselAlfa() const;
    void setSceneMultiselAlfa(int value);
    bool JsonCompactMode() const;
    void setJsonCompactMode(bool value);
    QString PresetFileExtension() const;
    void setPresetFileExtension(const QString &value);
    bool CopyToClipboardExceptDead() const;
    void setCopyToClipboardExceptDead(bool value);
    bool SaveToPresetExceptDead() const;
    void setSaveToPresetExceptDead(bool value);
    bool IgnoreJsonDataVersion() const;
    void setIgnoreJsonDataVersion(bool value);

protected:
    void load();

private:
    QSettings* m_Settings;
    QString m_DateTimeFormat;                       // формат отображения даты и времени
    QString m_PathAppConfig;                        // путь до конфига приложения
    QString m_PathAppDir;                           // путь до приложения
    QString m_LastDir;                              // путь до последнего каталога
    QString m_SceneSelectColor;                     // цвет выбранного объекта сцены
    QString m_SceneBgColor;                         // цвет задника сцены
    QString m_SceneCellDeadColor;                   // цвет мёртвой ячейки
    QString m_SceneCellCurseColor;                  // цвет отравленной ячейки
    QString m_SceneViewUpdateMode;                  // метод отрисовки SceneView
    QString m_SceneFieldThreadPriority;             // приоритет потока Field
    QString m_SceneImageFileFormat;                 // формат файлов для сохранения картинки сцены
    QString m_PresetFileExtension;                  // формат файлов для сохранения пресетов
    qreal m_SceneScaleStep;                         // шаг масштабирования сцены
    int m_SceneMultiselAlfa;                        // прозрачность рамки выделения 0-254
    int m_SceneItemZValue;                          // Z-координата SceneItem
    int m_SceneCalculatingMinPause;                 // минимальная допустимая пауза при обсчёте Field
    int m_LogSize;                                  // количество строк лога событий
    int m_ButtonSize;                               // размеры кнопок интерфейса
    int m_SceneCellSize;                            // размер объекта сцены
    int m_SceneFieldSize;                           // размер сцены при создании
    int m_CellInfoWindowHeight;                     // размеры окна информации о Cell
    int m_CellInfoWindowWidth;                      // размеры окна информации о Cell
    int m_FieldInfoWindowHeight;                    // размеры окна информации о Field
    int m_FieldInfoWindowWidth;                     // размеры окна информации о Field
    Qt::KeyboardModifiers m_SceneZoomModifier;      // модификатор управления зумом
    Qt::KeyboardModifiers m_SceneToolTipModifier;   // модификатор отображения координат Field
    Qt::KeyboardModifiers m_SceneMultiselModifier;  // модификатор выбора группы Cell
    bool m_SceneCellAgeIndicate;                    // отображать возраст живых ячеек
    bool m_SI_metric;                               // использовать систему СИ в отображении размеров файлов
    bool m_SceneViewAntialiasing;                   // SceneView: Antialiasing
    bool m_WindowShowFieldInfo;                     // отображать окно информации о Field при создании Field
    bool m_JsonCompactMode;                         // формат записи json
    bool m_IgnoreJsonDataVersion;                   // игнорировать версию в данных json
    bool m_CopyToClipboardExceptDead;               // при копировании в буфер обмена исключать мёртвые
    bool m_SaveToPresetExceptDead;                  // при сохранении в пресет исключать мёртвые
};

#endif // CONFIG_H
