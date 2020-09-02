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

    QString PathAppDir() const;
    QString PathAppConfig() const;
    QString PathPresetsDir() const;
    void setPathPresetsDir(const QString &value);
    QString PathPojectsDir() const;
    void setPathPojectsDir(const QString &value);
    QString PathRulesDir() const;
    void setPathRulesDir(const QString &value);
    int ButtonSize() const;
    void setButtonSize(int value);
    QString DateTimeFormat() const;
    void setDateTimeFormat(const QString &value);
    QString LastDir() const;
    void setLastDir(const QString &value);
    bool WriteLogsToFile() const;
    void setWriteLogsToFile(bool value);
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
    bool CellAliveAgeIndicate() const;
    void setCellAliveAgeIndicate(bool value);
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
    QString ImageFileFormat() const;
    void setImageFileFormat(const QString &value);
    bool WindowShowFieldInfo() const;
    void setWindowShowFieldInfo(bool value);
    Qt::KeyboardModifiers SceneMultiselModifier() const;
    void setSceneMultiselModifier(const Qt::KeyboardModifiers &value);
    int SceneMultiselAlfa() const;
    void setSceneMultiselAlfa(int value);
    bool JsonCompactMode() const;
    void setJsonCompactMode(bool value);
    bool CopyToClipboardExceptDead() const;
    void setCopyToClipboardExceptDead(bool value);
    bool SaveToPresetExceptDead() const;
    void setSaveToPresetExceptDead(bool value);
    bool JsonIgnoreDataVersion() const;
    void setJsonIgnoreDataVersion(bool value);
    bool RewriteResource() const;
    void setRewriteResource(bool value);
    int SceneSelAlfa() const;
    void setSceneSelAlfa(int value);
    bool SceneFirstSnapshot() const;
    void setSceneFirstSnapshot(bool value);
    QString PathLogsDir() const;
    void setPathLogsDir(const QString &value);
    int SplashTime() const;
    void setSplashTime(int value);
    int SplashSize() const;
    void setSplashSize(int value);
    QString SceneCellAliveColor() const;
    void setSceneCellAliveColor(const QString &value);
    QString CellAliveAgeIndicBright() const;
    void setCellAliveAgeIndicBright(const QString &value);
    int CellAliveAgeIndicFactor() const;
    void setCellAliveAgeIndicFactor(int value);
    int CellAliveAgeIndicDiapason() const;
    void setCellAliveAgeIndicDiapason(int value);
    bool CellCurseAgeIndicate() const;
    void setCellCurseAgeIndicate(bool value);
    QString CellCurseAgeIndicBright() const;
    void setCellCurseAgeIndicBright(const QString &value);
    int CellCurseAgeIndicFactor() const;
    void setCellCurseAgeIndicFactor(int value);
    int CellCurseAgeIndicDiapason() const;
    void setCellCurseAgeIndicDiapason(int value);
    int EditRulesWindowHeight() const;
    void setEditRulesWindowHeight(int value);
    int EditRulesWindowWidth() const;
    void setEditRulesWindowWidth(int value);
    bool SceneCreateDefaultRule() const;
    void setSceneCreateDefaultRule(bool value);
    bool ProjectFileCompression() const;
    void setProjectFileCompression(bool value);
    int ProjectFileCompressionLevel() const;
    void setProjectFileCompressionLevel(int value);
    int InfoWindowHeight() const;
    void setInfoWindowHeight(int value);
    int InfoWindowWidth() const;
    void setInfoWindowWidth(int value);
    int FieldRandomisationValue() const;
    void setFieldRandomisationValue(int value);
    int SceneGridLineWidth() const;
    void setSceneGridLineWidth(int value);

protected:
    void load();

private:
    QSettings* m_Settings;
    QString m_DateTimeFormat;                       // формат отображения даты и времени
    QString m_PathAppConfig;                        // путь до конфига приложения
    QString m_PathPresetsDir;                       // путь до каталога с пресетами
    QString m_PathPojectsDir;                       // путь до каталога с проектами
    QString m_PathRulesDir;                         // путь до каталога с правилами
    QString m_PathLogsDir;                          // путь до каталога с логами
    QString m_PathAppDir;                           // путь до каталога приложения
    QString m_LastDir;                              // путь до последнего каталога
    QString m_SceneSelectColor;                     // цвет выбранного объекта сцены
    QString m_SceneBgColor;                         // цвет задника сцены
    QString m_SceneCellDeadColor;                   // цвет мёртвой ячейки
    QString m_SceneCellAliveColor;                  // цвет живой ячейки
    QString m_SceneCellCurseColor;                  // цвет отравленной ячейки
    QString m_SceneViewUpdateMode;                  // метод отрисовки SceneView
    QString m_SceneFieldThreadPriority;             // приоритет потока Field
    QString m_ImageFileFormat;                      // формат файлов для сохранения картинки сцены
    QString m_CellAliveAgeIndicBright;              // индикация возраста живой ячейки: ярче/темнее
    QString m_CellCurseAgeIndicBright;              // индикация возраста отравленной ячейки: ярче/темнее
    qreal m_SceneScaleStep;                         // шаг масштабирования сцены
    int m_FieldRandomisationValue;                  // параметр рандомизации 0-100
    int m_CellAliveAgeIndicFactor;                  // индикация возраста живой ячейки: максимальный фактор яркости/тёмности
    int m_CellAliveAgeIndicDiapason;                // индикация возраста живой ячейки: диапазон по возрасту
    int m_CellCurseAgeIndicFactor;                  // индикация возраста отравленной ячейки: максимальный фактор яркости/тёмности
    int m_CellCurseAgeIndicDiapason;                // индикация возраста живой отравленной: диапазон по возрасту
    int m_SceneMultiselAlfa;                        // прозрачность рамки выделения 0-254
    int m_SceneSelAlfa;                             // прозрачность выделения 0-254
    int m_SceneItemZValue;                          // Z-координата SceneItem
    int m_SceneCalculatingMinPause;                 // минимальная допустимая пауза при обсчёте Field
    int m_ButtonSize;                               // размеры кнопок интерфейса
    int m_SceneCellSize;                            // размер объекта сцены
    int m_SceneFieldSize;                           // размер сцены при создании
    int m_CellInfoWindowHeight;                     // размеры окна информации о Cell
    int m_CellInfoWindowWidth;                      // размеры окна информации о Cell
    int m_FieldInfoWindowHeight;                    // размеры окна информации о Field
    int m_FieldInfoWindowWidth;                     // размеры окна информации о Field
    int m_InfoWindowHeight;                         // размеры справочного окна
    int m_InfoWindowWidth;                          // размеры справочного окна
    int m_EditRulesWindowHeight;                    // размеры окна редактирования правил
    int m_EditRulesWindowWidth;                     // размеры окна редактирования правил
    int m_SplashTime;                               // время отображения сплеш-заставки
    int m_SplashSize;                               // размеры сплеш-заставки
    int m_ProjectFileCompressionLevel;              // уровень сжатия данных при сохранении проекта: -1 - поумолчанию, 0-9
    int m_SceneGridLineWidth;                       // ширина линий в сетки
    Qt::KeyboardModifiers m_SceneZoomKeyModifier;   // модификатор управления зумом
    Qt::KeyboardModifiers m_SceneTooltipKeyModifier;// модификатор отображения координат Field
    Qt::KeyboardModifiers m_SceneMultiselKeyModifier;// модификатор выбора группы Cell
    bool m_CellAliveAgeIndicate;                    // индикация возраста живой ячейки
    bool m_CellCurseAgeIndicate;                    // индикация возраста отравленной ячейки
    bool m_WriteLogsToFile;                         // писать логи в файл
    bool m_SceneViewAntialiasing;                   // SceneView: Antialiasing
    bool m_WindowShowFieldInfo;                     // отображать окно информации о Field при создании Field
    bool m_JsonCompactMode;                         // формат записи json
    bool m_JsonIgnoreDataVersion;                   // игнорировать версию в данных json
    bool m_CopyToClipboardExceptDead;               // при копировании в буфер обмена исключать мёртвые
    bool m_SaveToPresetExceptDead;                  // при сохранении в пресет исключать мёртвые
    bool m_RewriteResource;                         // переписывать ресурсы при старте
    bool m_SceneFirstSnapshot;                      // создавать снапшот при первом запуске поля
    bool m_SceneCreateDefaultRule;                  // создавать правило по-умолчанию
    bool m_ProjectFileCompression;                  // сжимать данные при сохранении проекта
};

#endif // CONFIG_H
