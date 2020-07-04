#include "config.h"
#include "properties.h"

#include <QDebug>
#include <QDir>
#include <QTextCodec>

Config::Config(const QString& in_AppDirectory):
    m_Settings(nullptr)
{
    m_PathAppDir = in_AppDirectory;
    m_PathAppConfig = m_PathAppDir + QDir::separator() + APP_CFG;

    qInfo() << "AppConfig:" << m_PathAppConfig;

    m_Settings = new QSettings(m_PathAppConfig, QSettings::IniFormat);
    m_Settings->setIniCodec(QTextCodec::codecForName(TEXT_CODEC.toLatin1()));

    load();

    QObject::connect(this, &QObject::destroyed, [=](){ qInfo() << "Config destroyed"; });
}

void Config::load()
{
    if(!m_Settings->contains("LastCatalog"))
        m_Settings->setValue("LastCatalog", m_PathAppDir);
    m_LastDir = m_Settings->value("LastCatalog").toString();

    if(!m_Settings->contains("SIMetric"))
        m_Settings->setValue("SIMetric", SI_METRIC);
    m_SI_metric = m_Settings->value("SIMetric").toBool();

    if(!m_Settings->contains("MainWindow/Height"))
        m_Settings->setValue("MainWindow/Height", WINDOW_HEIGHT);

    if(!m_Settings->contains("MainWindow/Width"))
        m_Settings->setValue("MainWindow/Width", WINDOW_WIDTH);

    if(!m_Settings->contains("MainWindow/LogSize"))
        m_Settings->setValue("MainWindow/LogSize", LOG_SIZE);
    m_LogSize = m_Settings->value("MainWindow/LogSize").toInt();

    if(!m_Settings->contains("MainWindow/ButtonSize"))
        m_Settings->setValue("MainWindow/ButtonSize", BUTTON_SIZE);
    m_ButtonSize = m_Settings->value("MainWindow/ButtonSize").toInt();

    if(!m_Settings->contains("MainWindow/DateTimeFormat"))
        m_Settings->setValue("MainWindow/DateTimeFormat", DT_FORMAT);
    m_DateTimeFormat = m_Settings->value("MainWindow/DateTimeFormat").toString();

    if(!m_Settings->contains("MainWindow/CellInfoWindowHeight"))
        m_Settings->setValue("MainWindow/CellInfoWindowHeight", CELL_INFO_WINDOW_HEIGHT);
    m_CellInfoWindowHeight = m_Settings->value("MainWindow/CellInfoWindowHeight").toInt();

    if(!m_Settings->contains("MainWindow/CellInfoWindowWidth"))
        m_Settings->setValue("MainWindow/CellInfoWindowWidth", CELL_INFO_WINDOW_WIDTH);
    m_CellInfoWindowWidth = m_Settings->value("MainWindow/CellInfoWindowWidth").toInt();

    if(!m_Settings->contains("Scene/BackgroundColor"))
        m_Settings->setValue("Scene/BackgroundColor", SCENE_BG_COLOR);
    m_SceneBgColor = m_Settings->value("Scene/BackgroundColor").toString();

    if(!m_Settings->contains("Scene/ScaleStep"))
        m_Settings->setValue("Scene/ScaleStep", SCENE_SCALE_STEP);
    m_SceneScaleStep = m_Settings->value("Scene/ScaleStep").toDouble();

    if(!m_Settings->contains("Scene/CellSize"))
        m_Settings->setValue("Scene/CellSize", SCENE_CELL_SIZE);
    m_SceneCellSize = m_Settings->value("Scene/CellSize").toInt();

    if(!m_Settings->contains("Scene/FieldSize"))
        m_Settings->setValue("Scene/FieldSize", SCENE_FIELD_SIZE);
    m_SceneFieldSize = m_Settings->value("Scene/FieldSize").toInt();

    if(!m_Settings->contains("Scene/ViewAntialiasing"))
        m_Settings->setValue("Scene/ViewAntialiasing", SCENE_VIEW_ANTIALIASING);
    m_SceneViewAntialiasing = m_Settings->value("Scene/ViewAntialiasing").toBool();

    if(!m_Settings->contains("Scene/BspTreeIndex"))
        m_Settings->setValue("Scene/BspTreeIndex", SCENE_BSP_TREE_INDEX);
    m_SceneBspTreeIndex = m_Settings->value("Scene/BspTreeIndex").toBool();

    if(!m_Settings->contains("Scene/BspTreeDepth"))
        m_Settings->setValue("Scene/BspTreeDepth", SCENE_BSP_TREE_DEPTH);
    m_SceneBspTreeDepth = m_Settings->value("Scene/BspTreeDepth").toInt();

    if(!m_Settings->contains("Scene/ZoomKeyModifier"))
        m_Settings->setValue("Scene/ZoomKeyModifier", SCENE_ZOOM_MODIFIER);
    m_SceneZoomModifier = static_cast<Qt::KeyboardModifiers>(m_Settings->value("Scene/ZoomKeyModifier").toInt());

    if(!m_Settings->contains("Scene/ObjectKeyModifier"))
        m_Settings->setValue("Scene/ObjectKeyModifier", SCENE_OBJECT_MODIFIER);
    m_SceneObjectModifier = static_cast<Qt::KeyboardModifiers>(m_Settings->value("Scene/ObjectKeyModifier").toInt());

    if(!m_Settings->contains("Scene/ObjectAgeIndicate"))
        m_Settings->setValue("Scene/ObjectAgeIndicate", SCENE_OBJECT_AGE_INDICATE);
    m_SceneObjectAgeIndicate = m_Settings->value("Scene/ObjectAgeIndicate").toBool();

    if(!m_Settings->contains("Scene/CellDeadColor"))
        m_Settings->setValue("Scene/CellDeadColor", SCENE_CELL_DEAD_COLOR);
    m_SceneCellDeadColor = m_Settings->value("Scene/CellDeadColor").toString();

    if(!m_Settings->contains("Scene/SceneSelectColor"))
        m_Settings->setValue("Scene/SceneSelectColor", SCENE_SELECT_COLOR);
    m_SceneSelectColor = m_Settings->value("Scene/SceneSelectColor").toString();

    if(!m_Settings->contains("Scene/ViewUpdateMode"))
        m_Settings->setValue("Scene/ViewUpdateMode", SCENE_VIEW_UPDATE_MODE);
    m_SceneViewUpdateMode = m_Settings->value("Scene/ViewUpdateMode").toString();

    if(!m_Settings->contains("Scene/CellCurseColor"))
        m_Settings->setValue("Scene/CellCurseColor", SCENE_CELL_CURSE_COLOR);
    m_SceneCellCurseColor = m_Settings->value("Scene/CellCurseColor").toString();

    if(!m_Settings->contains("Scene/CalculatingMinPause"))
        m_Settings->setValue("Scene/CalculatingMinPause", SCENE_CALCULATING_MIN_PAUSE);
    m_SceneCalculatingMinPause = m_Settings->value("Scene/CalculatingMinPause").toInt();
}

void Config::setSceneViewUpdateMode(const QString &value)
{
    if(m_SceneViewUpdateMode == value) return;

    m_SceneViewUpdateMode = value;
    m_Settings->setValue("Scene/ViewUpdateMode", m_SceneViewUpdateMode);
}

void Config::setSceneCalculatingMinPause(int value)
{
    if(m_SceneCalculatingMinPause == value) return;

    m_SceneCalculatingMinPause = value;
    m_Settings->setValue("Scene/CalculatingMinPause", m_SceneCalculatingMinPause);
}

void Config::setCellInfoWindowWidth(int value)
{
    if(m_CellInfoWindowWidth == value) return;

    m_CellInfoWindowWidth = value;
    m_Settings->setValue("MainWindow/CellInfoWindowWidth", m_CellInfoWindowWidth);
}

void Config::setCellInfoWindowHeight(int value)
{
    if(m_CellInfoWindowHeight == value) return;

    m_CellInfoWindowHeight = value;
    m_Settings->setValue("MainWindow/CellInfoWindowHeight", m_CellInfoWindowHeight);
}

void Config::setSceneCellCurseColor(const QString &value)
{
    if(m_SceneCellCurseColor == value) return;
    
    m_SceneCellCurseColor = value;
    m_Settings->setValue("Scene/CellCurseColor", m_SceneCellCurseColor);
}

void Config::setSceneSelectColor(const QString &value)
{
    if(m_SceneSelectColor == value) return;
    
    m_SceneSelectColor = value;
    m_Settings->setValue("Scene/SceneSelectColor", m_SceneSelectColor);
}

void Config::setSceneObjectAgeIndicate(bool value)
{
    if(m_SceneObjectAgeIndicate == value) return;

    m_SceneObjectAgeIndicate = value;
    m_Settings->setValue("Scene/ObjectAgeIndicate", m_SceneObjectAgeIndicate);
}

void Config::setSceneCellDeadColor(const QString &value)
{
    if(m_SceneCellDeadColor == value) return;

    m_SceneCellDeadColor = value;
    m_Settings->setValue("Scene/CellDeadColor", m_SceneCellDeadColor);
}

void Config::setSceneFieldSize(int value)
{
    if(m_SceneFieldSize == value) return;
    
    m_SceneFieldSize = value;
    m_Settings->setValue("Scene/FieldSize", m_SceneFieldSize);
}

void Config::setSceneObjectModifier(const Qt::KeyboardModifiers &value)
{
    if(m_SceneObjectModifier == value) return;
    
    m_SceneObjectModifier = value;
    m_Settings->setValue("Scene/ObjectKeyModifier", static_cast<int>(m_SceneObjectModifier));
}

void Config::setSceneZoomModifier(const Qt::KeyboardModifiers &value)
{
    if(m_SceneZoomModifier == value) return;

    m_SceneZoomModifier = value;
    m_Settings->setValue("Scene/ZoomKeyModifier", static_cast<int>(m_SceneZoomModifier));
}

void Config::setSceneBspTreeDepth(int value)
{
    if(m_SceneBspTreeDepth == value) return;
    
    m_SceneBspTreeDepth = value;
    m_Settings->setValue("Scene/BspTreeDepth", m_SceneBspTreeDepth);
}

void Config::setSceneBspTreeIndex(bool value)
{
    if(m_SceneBspTreeIndex == value) return;

    m_SceneBspTreeIndex = value;
    m_Settings->setValue("Scene/BspTreeIndex", m_SceneBspTreeIndex);
}

void Config::setSceneViewAntialiasing(bool value)
{
    if(m_SceneViewAntialiasing == value) return;

    m_SceneViewAntialiasing = value;
    m_Settings->setValue("Scene/ViewAntialiasing", m_SceneViewAntialiasing);
}

void Config::setSceneCellSize(int value)
{
    if(m_SceneCellSize == value) return;
    
    m_SceneCellSize = value;
    m_Settings->setValue("Scene/CellSize", m_SceneCellSize);
}

void Config::setSceneScaleStep(qreal value)
{
    if(value <= 0) m_SceneScaleStep = 2.0;
    else m_SceneScaleStep = value;
    m_Settings->setValue("Scene/ScaleStep", m_SceneScaleStep);
}

void Config::setSceneBgColor(const QString &value)
{
    if(m_SceneBgColor == value) return;
    
    m_SceneBgColor = value;
    m_Settings->setValue("Scene/BackgroundColor", m_SceneBgColor);
}

void Config::setSIMetric(bool value)
{
    if(m_SI_metric == value) return;
    
    m_SI_metric = value;
    m_Settings->setValue("SIMetric", m_SI_metric);
}

void Config::setLogSize(int value)
{
    if(m_LogSize == value) return;
    
    m_LogSize = value >= 0 ? value : 0;
    m_Settings->setValue("MainWindow/LogSize", m_LogSize);
}

void Config::setLastDir(const QString &value)
{
    if(m_LastDir == value) return;

    m_LastDir = value;
    m_Settings->setValue("LastCatalog", m_LastDir);
}

void Config::setDateTimeFormat(const QString &value)
{
    if(m_DateTimeFormat == value) return;
    
    m_DateTimeFormat = value;
    m_Settings->setValue("MainWindow/DateTimeFormat", m_DateTimeFormat);
}

void Config::setButtonSize(int value)
{
    if(m_ButtonSize == value) return;

    m_ButtonSize = value;
    m_Settings->setValue("MainWindow/ButtonSize", m_ButtonSize);
}

QString Config::SceneViewUpdateMode() const { return m_SceneViewUpdateMode; }
int Config::CellInfoWindowWidth() const { return m_CellInfoWindowWidth; }
int Config::CellInfoWindowHeight() const { return m_CellInfoWindowHeight; }
QString Config::SceneCellCurseColor() const { return m_SceneCellCurseColor; }
QString Config::SceneSelectColor() const { return m_SceneSelectColor; }
bool Config::SceneObjectAgeIndicate() const { return m_SceneObjectAgeIndicate; }
QString Config::SceneCellDeadColor() const { return m_SceneCellDeadColor; }
int Config::SceneFieldSize() const { return m_SceneFieldSize; }
Qt::KeyboardModifiers Config::SceneObjectModifier() const { return m_SceneObjectModifier; }
Qt::KeyboardModifiers Config::SceneZoomModifier() const { return m_SceneZoomModifier; }
int Config::SceneBspTreeDepth() const { return m_SceneBspTreeDepth; }
bool Config::SceneBspTreeIndex() const { return m_SceneBspTreeIndex; }
bool Config::SceneViewAntialiasing() const { return m_SceneViewAntialiasing; }
int Config::SceneCellSize() const { return m_SceneCellSize; }
qreal Config::SceneScaleStep() const { return m_SceneScaleStep; }
QString Config::SceneBgColor() const { return m_SceneBgColor; }
bool Config::SIMetric() const { return m_SI_metric; }
int Config::LogSize() const { return m_LogSize; }
QString Config::LastDir() const { return m_LastDir; }
QString Config::DateTimeFormat() const { return m_DateTimeFormat; }
int Config::ButtonSize() const { return m_ButtonSize; }
QString Config::PathApp() const { return m_PathAppDir; }
QString Config::PathAppConfig() const { return m_PathAppConfig; }
int Config::SceneCalculatingMinPause() const { return m_SceneCalculatingMinPause; }
