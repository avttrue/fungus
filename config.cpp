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

    if(!m_Settings->contains("Scene/BackgroundColor"))
        m_Settings->setValue("Scene/BackgroundColor", SCENE_BG_COLOR);
    m_SceneBgColor = m_Settings->value("Scene/BackgroundColor").toString();

    if(!m_Settings->contains("Scene/Color"))
        m_Settings->setValue("Scene/Color", SCENE_COLOR);
    m_SceneColor = m_Settings->value("Scene/Color").toString();

    if(!m_Settings->contains("Scene/BorderColor"))
        m_Settings->setValue("Scene/BorderColor", SCENE_BORDER_COLOR);
    m_SceneBorderColor = m_Settings->value("Scene/BorderColor").toString();

    if(!m_Settings->contains("Scene/ScaleStep"))
        m_Settings->setValue("Scene/ScaleStep", SCENE_SCALE_STEP);
    m_SceneScaleStep = m_Settings->value("Scene/ScaleStep").toDouble();

    if(!m_Settings->contains("Scene/ObjectSize"))
        m_Settings->setValue("Scene/ObjectSize", SCENE_OBJECT_SIZE);
    m_SceneObjectSize = m_Settings->value("Scene/ObjectSize").toInt();

    if(!m_Settings->contains("Scene/Size"))
        m_Settings->setValue("Scene/Size", SCENE_SIZE);
    m_SceneSize = m_Settings->value("Scene/Size").toInt();

    if(!m_Settings->contains("Scene/ViewAntialiasing"))
        m_Settings->setValue("Scene/ViewAntialiasing", SCENE_VIEW_ANTIALIASING);
    m_SceneViewAntialiasing = m_Settings->value("Scene/ViewAntialiasing").toBool();

    if(!m_Settings->contains("Scene/PreFill"))
        m_Settings->setValue("Scene/PreFill", SCENE_PREFILL);
    m_ScenePreFill = m_Settings->value("Scene/PreFill").toBool();

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

    if(!m_Settings->contains("Scene/ObjectDeadColor"))
        m_Settings->setValue("Scene/ObjectDeadColor", SCENE_OBJECT_DEAD_COLOR);
    m_SceneObjectDeadColor = m_Settings->value("Scene/ObjectDeadColor").toString();

    if(!m_Settings->contains("Scene/ObjectAlive0Color"))
        m_Settings->setValue("Scene/ObjectAlive0Color", SCENE_OBJECT_ALIVE0_COLOR);
    m_SceneObjectAlive0Color = m_Settings->value("Scene/ObjectAlive0Color").toString();

    if(!m_Settings->contains("Scene/ObjectAlive1Color"))
        m_Settings->setValue("Scene/ObjectAlive1Color", SCENE_OBJECT_ALIVE1_COLOR);
    m_SceneObjectAlive1Color = m_Settings->value("Scene/ObjectAlive1Color").toString();

    if(!m_Settings->contains("Scene/ObjectAlive2Color"))
        m_Settings->setValue("Scene/ObjectAlive2Color", SCENE_OBJECT_ALIVE2_COLOR);
    m_SceneObjectAlive2Color = m_Settings->value("Scene/ObjectAlive2Color").toString();

    if(!m_Settings->contains("Scene/ObjectAlive3Color"))
        m_Settings->setValue("Scene/ObjectAlive3Color", SCENE_OBJECT_ALIVE3_COLOR);
    m_SceneObjectAlive3Color = m_Settings->value("Scene/ObjectAlive3Color").toString();

    if(!m_Settings->contains("Scene/ObjectAlive4Color"))
        m_Settings->setValue("Scene/ObjectAlive4Color", SCENE_OBJECT_ALIVE4_COLOR);
    m_SceneObjectAlive4Color = m_Settings->value("Scene/ObjectAlive4Color").toString();

    if(!m_Settings->contains("Scene/ObjectAlive5Color"))
        m_Settings->setValue("Scene/ObjectAlive5Color", SCENE_OBJECT_ALIVE5_COLOR);
    m_SceneObjectAlive5Color = m_Settings->value("Scene/ObjectAlive5Color").toString();

    if(!m_Settings->contains("Scene/ObjectAlive6Color"))
        m_Settings->setValue("Scene/ObjectAlive6Color", SCENE_OBJECT_ALIVE6_COLOR);
    m_SceneObjectAlive6Color = m_Settings->value("Scene/ObjectAlive6Color").toString();

    if(!m_Settings->contains("Scene/ObjectAlive7Color"))
        m_Settings->setValue("Scene/ObjectAlive7Color", SCENE_OBJECT_ALIVE7_COLOR);
    m_SceneObjectAlive7Color = m_Settings->value("Scene/ObjectAlive7Color").toString();

    if(!m_Settings->contains("Scene/ObjectAlive8Color"))
        m_Settings->setValue("Scene/ObjectAlive8Color", SCENE_OBJECT_ALIVE8_COLOR);
    m_SceneObjectAlive8Color = m_Settings->value("Scene/ObjectAlive8Color").toString();

    if(!m_Settings->contains("Scene/ObjectAlive9Color"))
        m_Settings->setValue("Scene/ObjectAlive9Color", SCENE_OBJECT_ALIVE9_COLOR);
    m_SceneObjectAlive9Color = m_Settings->value("Scene/ObjectAlive9Color").toString();

    if(!m_Settings->contains("Scene/SceneSelectColor"))
        m_Settings->setValue("Scene/SceneSelectColor", SCENE_SELECT_COLOR);
    m_SceneSelectColor = m_Settings->value("Scene/SceneSelectColor").toString();

    if(!m_Settings->contains("Scene/ObjectCurseColor"))
        m_Settings->setValue("Scene/ObjectCurseColor", SCENE_OBJECT_CURSE_COLOR);
    m_SceneObjectCurseColor = m_Settings->value("Scene/ObjectCurseColor").toString();
}

void Config::setSceneObjectCurseColor(const QString &value)
{
    if(m_SceneObjectCurseColor == value) return;

    m_SceneObjectCurseColor = value;
    m_Settings->setValue("Scene/ObjectCurseColor", m_SceneObjectCurseColor);
}

void Config::setSceneSelectColor(const QString &value)
{
    if(m_SceneSelectColor == value) return;
    
    m_SceneSelectColor = value;
    m_Settings->setValue("Scene/SceneSelectColor", m_SceneSelectColor);
}

void Config::setSceneObjectAlive0Color(const QString &value)
{
    if(m_SceneObjectAlive0Color == value) return;
    
    m_SceneObjectAlive0Color = value;
    m_Settings->setValue("Scene/ObjectAlive0Color", m_SceneObjectAlive0Color);
}

void Config::setSceneObjectAlive1Color(const QString &value)
{
    if(m_SceneObjectAlive1Color == value) return;

    m_SceneObjectAlive1Color = value;
    m_Settings->setValue("Scene/ObjectAlive1Color", m_SceneObjectAlive1Color);
}

void Config::setSceneObjectAlive2Color(const QString &value)
{
    if(m_SceneObjectAlive2Color == value) return;

    m_SceneObjectAlive2Color = value;
    m_Settings->setValue("Scene/ObjectAlive2Color", m_SceneObjectAlive2Color);
}

void Config::setSceneObjectAlive3Color(const QString &value)
{
    if(m_SceneObjectAlive3Color == value) return;

    m_SceneObjectAlive3Color = value;
    m_Settings->setValue("Scene/ObjectAlive3Color", m_SceneObjectAlive3Color);
}

void Config::setSceneObjectAlive4Color(const QString &value)
{
    if(m_SceneObjectAlive4Color == value) return;

    m_SceneObjectAlive4Color = value;
    m_Settings->setValue("Scene/ObjectAlive4Color", m_SceneObjectAlive4Color);
}

void Config::setSceneObjectAlive5Color(const QString &value)
{
    if(m_SceneObjectAlive5Color == value) return;

    m_SceneObjectAlive5Color = value;
    m_Settings->setValue("Scene/ObjectAlive5Color", m_SceneObjectAlive5Color);
}

void Config::setSceneObjectAlive6Color(const QString &value)
{
    if(m_SceneObjectAlive6Color == value) return;

    m_SceneObjectAlive6Color = value;
    m_Settings->setValue("Scene/ObjectAlive6Color", m_SceneObjectAlive6Color);
}

void Config::setSceneObjectAlive7Color(const QString &value)
{
    if(m_SceneObjectAlive7Color == value) return;

    m_SceneObjectAlive7Color = value;
    m_Settings->setValue("Scene/ObjectAlive7Color", m_SceneObjectAlive7Color);
}

void Config::setSceneObjectAlive8Color(const QString &value)
{
    if(m_SceneObjectAlive8Color == value) return;

    m_SceneObjectAlive8Color = value;
    m_Settings->setValue("Scene/ObjectAlive8Color", m_SceneObjectAlive8Color);
}

void Config::setSceneObjectAlive9Color(const QString &value)
{
    if(m_SceneObjectAlive9Color == value) return;

    m_SceneObjectAlive9Color = value;
    m_Settings->setValue("Scene/ObjectAlive9Color", m_SceneObjectAlive9Color);
}

void Config::setSceneObjectAgeIndicate(bool value)
{
    if(m_SceneObjectAgeIndicate == value) return;

    m_SceneObjectAgeIndicate = value;
    m_Settings->setValue("Scene/ObjectAgeIndicate", m_SceneObjectAgeIndicate);
}

void Config::setSceneObjectDeadColor(const QString &value)
{
    if(m_SceneObjectDeadColor == value) return;

    m_SceneObjectDeadColor = value;
    m_Settings->setValue("Scene/ObjectDeadColor", m_SceneObjectDeadColor);
}

void Config::setSceneSize(int value)
{
    if(m_SceneSize == value) return;
    
    m_SceneSize = value;
    m_Settings->setValue("Scene/Size", m_SceneSize);
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

void Config::setScenePreFill(bool value)
{
    if(m_ScenePreFill == value) return;

    m_ScenePreFill = value;
    m_Settings->setValue("Scene/PreFill", m_ScenePreFill);
}

void Config::setSceneBorderColor(const QString &value)
{
    if(m_SceneBorderColor == value) return;

    m_SceneBorderColor = value;
    m_Settings->setValue("Scene/BorderColor", m_SceneBorderColor);
}

void Config::setSceneViewAntialiasing(bool value)
{
    if(m_SceneViewAntialiasing == value) return;

    m_SceneViewAntialiasing = value;
    m_Settings->setValue("Scene/ViewAntialiasing", m_SceneViewAntialiasing);
}

void Config::setSceneColor(const QString &value)
{
    if(m_SceneColor == value) return;
    
    m_SceneColor = value;
    m_Settings->setValue("Scene/Color", m_SceneColor);
}

void Config::setSceneObjectSize(int value)
{
    if(m_SceneObjectSize == value) return;
    
    m_SceneObjectSize = value;
    m_Settings->setValue("Scene/ObjectSize", m_SceneObjectSize);
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

QString Config::SceneObjectCurseColor() const { return m_SceneObjectCurseColor; }
QString Config::SceneSelectColor() const { return m_SceneSelectColor; }
QString Config::SceneObjectAlive0Color() const { return m_SceneObjectAlive0Color; }
QString Config::SceneObjectAlive1Color() const { return m_SceneObjectAlive1Color; }
QString Config::SceneObjectAlive2Color() const { return m_SceneObjectAlive2Color; }
QString Config::SceneObjectAlive3Color() const { return m_SceneObjectAlive3Color; }
QString Config::SceneObjectAlive4Color() const { return m_SceneObjectAlive4Color; }
QString Config::SceneObjectAlive5Color() const { return m_SceneObjectAlive5Color; }
QString Config::SceneObjectAlive6Color() const { return m_SceneObjectAlive6Color; }
QString Config::SceneObjectAlive7Color() const { return m_SceneObjectAlive7Color; }
QString Config::SceneObjectAlive8Color() const { return m_SceneObjectAlive8Color; }
QString Config::SceneObjectAlive9Color() const { return m_SceneObjectAlive9Color; }
bool Config::SceneObjectAgeIndicate() const { return m_SceneObjectAgeIndicate; }
QString Config::SceneObjectDeadColor() const { return m_SceneObjectDeadColor; }
int Config::SceneSize() const { return m_SceneSize; }
Qt::KeyboardModifiers Config::SceneObjectModifier() const { return m_SceneObjectModifier; }
Qt::KeyboardModifiers Config::SceneZoomModifier() const { return m_SceneZoomModifier; }
int Config::SceneBspTreeDepth() const { return m_SceneBspTreeDepth; }
bool Config::SceneBspTreeIndex() const { return m_SceneBspTreeIndex; }
bool Config::ScenePreFill() const { return m_ScenePreFill; }
QString Config::SceneBorderColor() const { return m_SceneBorderColor; }
bool Config::SceneViewAntialiasing() const { return m_SceneViewAntialiasing; }
QString Config::SceneColor() const { return m_SceneColor; }
int Config::SceneObjectSize() const { return m_SceneObjectSize; }
qreal Config::SceneScaleStep() const { return m_SceneScaleStep; }
QString Config::SceneBgColor() const { return m_SceneBgColor; }
bool Config::SIMetric() const { return m_SI_metric; }
int Config::LogSize() const { return m_LogSize; }
QString Config::LastDir() const { return m_LastDir; }
QString Config::DateTimeFormat() const { return m_DateTimeFormat; }
int Config::ButtonSize() const { return m_ButtonSize; }
QString Config::PathApp() const { return m_PathAppDir; }
QString Config::PathAppConfig() const { return m_PathAppConfig; }
