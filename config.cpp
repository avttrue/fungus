#include "config.h"
#include "properties.h"

#include <QDebug>
#include <QDir>
#include <QTextCodec>

Config::Config(const QString& in_AppDirectory):
    m_Settings(nullptr),
    m_UnsavedTasksEnabled(false)
{
    setObjectName("Config");

    m_PathAppDir = in_AppDirectory;
    m_PathAppConfig = m_PathAppDir + QDir::separator() + APP_CFG;

    m_Settings = new QSettings(m_PathAppConfig, QSettings::IniFormat);
    m_Settings->setIniCodec(QTextCodec::codecForName(TEXT_CODEC.toLatin1()));

    load();

    QObject::connect(this, &QObject::destroyed, [=](){ qDebug() << objectName() << "destroyed"; });
    qDebug() << objectName() << "created";
}

void Config::load()
{
    if(!m_Settings->contains("LastCatalog"))
        m_Settings->setValue("LastCatalog", m_PathAppDir);
    m_LastDir = m_Settings->value("LastCatalog").toString();

    if(!m_Settings->contains("WriteLogsToFile"))
        m_Settings->setValue("WriteLogsToFile", WRITE_LOGS_TO_FILE);
    m_WriteLogsToFile = m_Settings->value("WriteLogsToFile").toBool();

    if(!m_Settings->contains("JsonCompactMode"))
        m_Settings->setValue("JsonCompactMode", JSON_COMPACT_MODE);
    m_JsonCompactMode = m_Settings->value("JsonCompactMode").toBool();

    if(!m_Settings->contains("CopyToClipboardExceptDead"))
        m_Settings->setValue("CopyToClipboardExceptDead", COPY_TO_CLIPBOARD_EXCEPT_DEAD);
    m_CopyToClipboardExceptDead = m_Settings->value("CopyToClipboardExceptDead").toBool();

    if(!m_Settings->contains("SaveToPresetExceptDead"))
        m_Settings->setValue("SaveToPresetExceptDead", SAVE_TO_PRESET_EXCEPT_DEAD);
    m_SaveToPresetExceptDead = m_Settings->value("SaveToPresetExceptDead").toBool();

    if(!m_Settings->contains("JsonIgnoreDataVersion"))
        m_Settings->setValue("JsonIgnoreDataVersion", JSON_IGNORE_DATA_VERSION);
    m_JsonIgnoreDataVersion = m_Settings->value("JsonIgnoreDataVersion").toBool();

    if(!m_Settings->contains("DateTimeFormat"))
        m_Settings->setValue("DateTimeFormat", FORMAT_DT);
    m_DateTimeFormat = m_Settings->value("DateTimeFormat").toString();

    if(!m_Settings->contains("ImageAutoopen"))
        m_Settings->setValue("ImageAutoopen", IMAGE_AUTOOPEN);
    m_ImageAutoopen = m_Settings->value("ImageAutoopen").toBool();

    if(!m_Settings->contains("ReportAutoopen"))
        m_Settings->setValue("ReportAutoopen", REPORT_AUTOOPEN);
    m_ReportAutoopen = m_Settings->value("ReportAutoopen").toBool();

    if(!m_Settings->contains("RewriteResource"))
        m_Settings->setValue("RewriteResource", REWITE_RESOURCES);
    m_RewriteResource = m_Settings->value("RewriteResource").toBool();

    if(!m_Settings->contains("ImageFileFormat"))
        m_Settings->setValue("ImageFileFormat", IMAGE_FILE_FORMAT);
    m_ImageFileFormat = m_Settings->value("ImageFileFormat").toString();

    if(!m_Settings->contains("ReportFileFormat"))
        m_Settings->setValue("ReportFileFormat", REPORT_FILE_FORMAT);
    m_ReportFileFormat = m_Settings->value("ReportFileFormat").toString();

    if(!m_Settings->contains("ProjectFileCompression"))
        m_Settings->setValue("ProjectFileCompression", PROJECT_FILE_COMPRESSION);
    m_ProjectFileCompression = m_Settings->value("ProjectFileCompression").toBool();

    if(!m_Settings->contains("ProjectFileCompressionLevel"))
        m_Settings->setValue("ProjectFileCompressionLevel", PROJECT_FILE_COMPRESSION_LEVEL);
    m_ProjectFileCompressionLevel = m_Settings->value("ProjectFileCompressionLevel").toInt();

    if(!m_Settings->contains("MainWindow/Height"))
        m_Settings->setValue("MainWindow/Height", WINDOW_HEIGHT);

    if(!m_Settings->contains("MainWindow/Width"))
        m_Settings->setValue("MainWindow/Width", WINDOW_WIDTH);

    if(!m_Settings->contains("MainWindow/ButtonSize"))
        m_Settings->setValue("MainWindow/ButtonSize", BUTTON_SIZE);
    m_ButtonSize = m_Settings->value("MainWindow/ButtonSize").toInt();

    if(!m_Settings->contains("MainWindow/CellInfoWindowHeight"))
        m_Settings->setValue("MainWindow/CellInfoWindowHeight", CELL_INFO_WINDOW_HEIGHT);
    m_CellInfoWindowHeight = m_Settings->value("MainWindow/CellInfoWindowHeight").toInt();

    if(!m_Settings->contains("MainWindow/CellInfoWindowWidth"))
        m_Settings->setValue("MainWindow/CellInfoWindowWidth", CELL_INFO_WINDOW_WIDTH);
    m_CellInfoWindowWidth = m_Settings->value("MainWindow/CellInfoWindowWidth").toInt();

    if(!m_Settings->contains("MainWindow/CellMonitorWindowWidth"))
        m_Settings->setValue("MainWindow/CellMonitorWindowWidth", CELL_MONITOR_WINDOW_WIDTH);
    m_CellMonitorWindowWidth = m_Settings->value("MainWindow/CellMonitorWindowWidth").toInt();

    if(!m_Settings->contains("MainWindow/CellMonitorWindowHeight"))
        m_Settings->setValue("MainWindow/CellMonitorWindowHeight", CELL_MONITOR_WINDOW_HEIGHT);
    m_CellMonitorWindowHeight = m_Settings->value("MainWindow/CellMonitorWindowHeight").toInt();

    if(!m_Settings->contains("MainWindow/FieldInfoWindowWidth"))
        m_Settings->setValue("MainWindow/FieldInfoWindowWidth", FIELD_INFO_WINDOW_WIDTH);
    m_FieldInfoWindowWidth = m_Settings->value("MainWindow/FieldInfoWindowWidth").toInt();

    if(!m_Settings->contains("MainWindow/FieldInfoWindowHeight"))
        m_Settings->setValue("MainWindow/FieldInfoWindowHeight", FIELD_INFO_WINDOW_HEIGHT);
    m_FieldInfoWindowHeight = m_Settings->value("MainWindow/FieldInfoWindowHeight").toInt();

    if(!m_Settings->contains("MainWindow/InfoWindowWidth"))
        m_Settings->setValue("MainWindow/InfoWindowWidth", INFO_WINDOW_WIDTH);
    m_InfoWindowWidth = m_Settings->value("MainWindow/InfoWindowWidth").toInt();

    if(!m_Settings->contains("MainWindow/InfoWindowHeight"))
        m_Settings->setValue("MainWindow/InfoWindowHeight", INFO_WINDOW_HEIGHT);
    m_InfoWindowHeight = m_Settings->value("MainWindow/InfoWindowHeight").toInt();

    if(!m_Settings->contains("MainWindow/InfoRuleWindowWidth"))
        m_Settings->setValue("MainWindow/InfoRuleWindowWidth", INFORULE_WINDOW_WIDTH);
    m_InfoRuleWindowWidth = m_Settings->value("MainWindow/InfoRuleWindowWidth").toInt();

    if(!m_Settings->contains("MainWindow/InfoRuleWindowHeight"))
        m_Settings->setValue("MainWindow/InfoRuleWindowHeight", INFORULE_WINDOW_HEIGHT);
    m_InfoRuleWindowHeight = m_Settings->value("MainWindow/InfoRuleWindowHeight").toInt();

    if(!m_Settings->contains("MainWindow/ReportWindowWidth"))
        m_Settings->setValue("MainWindow/ReportWindowWidth", REPORT_WINDOW_WIDTH);
    m_ReportWindowWidth = m_Settings->value("MainWindow/ReportWindowWidth").toInt();

    if(!m_Settings->contains("MainWindow/ReportWindowHeight"))
        m_Settings->setValue("MainWindow/ReportWindowHeight", REPORT_WINDOW_HEIGHT);
    m_ReportWindowHeight = m_Settings->value("MainWindow/ReportWindowHeight").toInt();

    if(!m_Settings->contains("MainWindow/HelpWindowWidth"))
        m_Settings->setValue("MainWindow/HelpWindowWidth", HELP_WINDOW_WIDTH);
    m_HelpWindowWidth = m_Settings->value("MainWindow/HelpWindowWidth").toInt();

    if(!m_Settings->contains("MainWindow/HelpWindowHeight"))
        m_Settings->setValue("MainWindow/HelpWindowHeight", HELP_WINDOW_HEIGHT);
    m_HelpWindowHeight = m_Settings->value("MainWindow/HelpWindowHeight").toInt();

    if(!m_Settings->contains("MainWindow/EditRulesWindowWidth"))
        m_Settings->setValue("MainWindow/EditRulesWindowWidth", EDIT_RULES_WINDOW_WIDTH);
    m_EditRulesWindowWidth = m_Settings->value("MainWindow/EditRulesWindowWidth").toInt();

    if(!m_Settings->contains("MainWindow/EditRulesWindowHeight"))
        m_Settings->setValue("MainWindow/EditRulesWindowHeight", EDIT_RULES_WINDOW_HEIGHT);
    m_EditRulesWindowHeight = m_Settings->value("MainWindow/EditRulesWindowHeight").toInt();

    if(!m_Settings->contains("MainWindow/SetupWindowWidth"))
        m_Settings->setValue("MainWindow/SetupWindowWidth", SETUP_WINDOW_WIDTH);
    m_SetupWindowWidth = m_Settings->value("MainWindow/SetupWindowWidth").toInt();

    if(!m_Settings->contains("MainWindow/SetupWindowHeight"))
        m_Settings->setValue("MainWindow/SetupWindowHeight", SETUP_WINDOW_HEIGHT);
    m_SetupWindowHeight = m_Settings->value("MainWindow/SetupWindowHeight").toInt();

    if(!m_Settings->contains("MainWindow/RandomFillWindowWidth"))
        m_Settings->setValue("MainWindow/RandomFillWindowWidth", RANDOMFILL_WINDOW_WIDTH);
    m_RandomFillWindowWidth = m_Settings->value("MainWindow/RandomFillWindowWidth").toInt();

    if(!m_Settings->contains("MainWindow/RandomFillWindowHeight"))
        m_Settings->setValue("MainWindow/RandomFillWindowHeight", RANDOMFILL_WINDOW_HEIGHT);
    m_RandomFillWindowHeight = m_Settings->value("MainWindow/RandomFillWindowHeight").toInt();

    if(!m_Settings->contains("MainWindow/ProjectWindowWidth"))
        m_Settings->setValue("MainWindow/ProjectWindowWidth", PROJECT_WINDOW_WIDTH);
    m_ProjectWindowWidth = m_Settings->value("MainWindow/ProjectWindowWidth").toInt();

    if(!m_Settings->contains("MainWindow/ProjectWindowHeight"))
        m_Settings->setValue("MainWindow/ProjectWindowHeight", PROJECT_WINDOW_HEIGHT);
    m_ProjectWindowHeight = m_Settings->value("MainWindow/ProjectWindowHeight").toInt();

    if(!m_Settings->contains("MainWindow/SnapshotWindowWidth"))
        m_Settings->setValue("MainWindow/SnapshotWindowWidth", SNAPSHOT_WINDOW_WIDTH);
    m_SnapshotWindowWidth = m_Settings->value("MainWindow/SnapshotWindowWidth").toInt();

    if(!m_Settings->contains("MainWindow/SnapshotWindowHeight"))
        m_Settings->setValue("MainWindow/SnapshotWindowHeight", SNAPSHOT_WINDOW_HEIGHT);
    m_SnapshotWindowHeight = m_Settings->value("MainWindow/SnapshotWindowHeight").toInt();

    if(!m_Settings->contains("MainWindow/EditCellWindowWidth"))
        m_Settings->setValue("MainWindow/EditCellWindowWidth", EDITCELL_WINDOW_WIDTH);
    m_EditCellWindowWidth = m_Settings->value("MainWindow/EditCellWindowWidth").toInt();

    if(!m_Settings->contains("MainWindow/EditCellWindowHeight"))
        m_Settings->setValue("MainWindow/EditCellWindowHeight", EDITCELL_WINDOW_HEIGHT);
    m_EditCellWindowHeight = m_Settings->value("MainWindow/EditCellWindowHeight").toInt();

    if(!m_Settings->contains("MainWindow/EditActivityWindowWidth"))
        m_Settings->setValue("MainWindow/EditActivityWindowWidth", EDITACTIVITY_WINDOW_WIDTH);
    m_EditActivityWindowWidth = m_Settings->value("MainWindow/EditActivityWindowWidth").toInt();

    if(!m_Settings->contains("MainWindow/EditActivityWindowHeight"))
        m_Settings->setValue("MainWindow/EditActivityWindowHeight", EDITACTIVITY_WINDOW_HEIGHT);
    m_EditActivityWindowHeight = m_Settings->value("MainWindow/EditActivityWindowHeight").toInt();

    if(!m_Settings->contains("MainWindow/RulePropertyWindowWidth"))
        m_Settings->setValue("MainWindow/RulePropertyWindowWidth", RULEPROPERTY_WINDOW_WIDTH);
    m_RulePropertyWindowWidth = m_Settings->value("MainWindow/RulePropertyWindowWidth").toInt();

    if(!m_Settings->contains("MainWindow/RulePropertyWindowHeight"))
        m_Settings->setValue("MainWindow/RulePropertyWindowHeight", RULEPROPERTY_WINDOW_HEIGHT);
    m_RulePropertyWindowHeight = m_Settings->value("MainWindow/RulePropertyWindowHeight").toInt();

    if(!m_Settings->contains("MainWindow/TasksWindowWidth"))
        m_Settings->setValue("MainWindow/TasksWindowWidth", TASKS_WINDOW_WIDTH);
    m_TasksWindowWidth = m_Settings->value("MainWindow/TasksWindowWidth").toInt();

    if(!m_Settings->contains("MainWindow/TasksWindowHeight"))
        m_Settings->setValue("MainWindow/TasksWindowHeight", TASKS_WINDOW_HEIGHT);
    m_TasksWindowHeight = m_Settings->value("MainWindow/TasksWindowHeight").toInt();

    if(!m_Settings->contains("MainWindow/ShowFieldInformation"))
        m_Settings->setValue("MainWindow/ShowFieldInformation", WINDOW_SHOW_FIELD_INFO);
    m_WindowShowFieldInfo = m_Settings->value("MainWindow/ShowFieldInformation").toBool();

    if(!m_Settings->contains("MainWindow/SplashSize"))
        m_Settings->setValue("MainWindow/SplashSize", SPLASH_SIZE);
    m_SplashSize = m_Settings->value("MainWindow/SplashSize").toInt();

    if(!m_Settings->contains("MainWindow/SplashTime"))
        m_Settings->setValue("MainWindow/SplashTime", SPLASH_TIME);
    m_SplashTime = m_Settings->value("MainWindow/SplashTime").toInt();

    if(!m_Settings->contains("MainWindow/HelpPage"))
        m_Settings->setValue("MainWindow/HelpPage", HELP_PAGE);
    m_HelpPage = m_Settings->value("MainWindow/HelpPage").toString();

    if(!m_Settings->contains("Scene/ZoomKeyModifier"))
        m_Settings->setValue("Scene/ZoomKeyModifier", SCENE_ZOOM_KEY_MODIFIER);
    m_SceneZoomKeyModifier = static_cast<Qt::KeyboardModifiers>(m_Settings->value("Scene/ZoomKeyModifier").toInt());

    if(!m_Settings->contains("Scene/TooltipKeyModifier"))
        m_Settings->setValue("Scene/TooltipKeyModifier", SCENE_TOOLTIP_KEY_MODIFIER);
    m_SceneTooltipKeyModifier = static_cast<Qt::KeyboardModifiers>(m_Settings->value("Scene/TooltipKeyModifier").toInt());

    if(!m_Settings->contains("Scene/MultiselectKeyModifier"))
        m_Settings->setValue("Scene/MultiselectKeyModifier", SCENE_MULTISELECT_KEY_MODIFIER);
    m_SceneMultiselKeyModifier = static_cast<Qt::KeyboardModifiers>(m_Settings->value("Scene/MultiselectKeyModifier").toInt());

    if(!m_Settings->contains("Scene/FieldFasteditKeyModifier"))
        m_Settings->setValue("Scene/FieldFasteditKeyModifier", FIELD_FASTEDIT_KEY_MODIFIER);
    m_FieldFasteditKeyModifier = static_cast<Qt::KeyboardModifiers>(m_Settings->value("Scene/FieldFasteditKeyModifier").toInt());

    if(!m_Settings->contains("Scene/BackgroundColor"))
        m_Settings->setValue("Scene/BackgroundColor", SCENE_BG_COLOR);
    m_SceneBgColor = m_Settings->value("Scene/BackgroundColor").toString();

    if(!m_Settings->contains("Scene/ScaleStep"))
        m_Settings->setValue("Scene/ScaleStep", SCENE_SCALE_STEP);
    m_SceneScaleStep = m_Settings->value("Scene/ScaleStep").toDouble();

    if(!m_Settings->contains("Scene/CellSize"))
        m_Settings->setValue("Scene/CellSize", SCENE_CELL_SIZE);
    m_SceneCellSize = m_Settings->value("Scene/CellSize").toInt();

    if(!m_Settings->contains("Scene/ViewAntialiasing"))
        m_Settings->setValue("Scene/ViewAntialiasing", SCENE_VIEW_ANTIALIASING);
    m_SceneViewAntialiasing = m_Settings->value("Scene/ViewAntialiasing").toBool();

    if(!m_Settings->contains("Scene/CellAliveAgeIndicate"))
        m_Settings->setValue("Scene/CellAliveAgeIndicate", SCENE_ALIVE_CELL_AGE_INDICATE);
    m_CellAliveAgeIndicate = m_Settings->value("Scene/CellAliveAgeIndicate").toBool();

    if(!m_Settings->contains("Scene/CellCurseAgeIndicate"))
        m_Settings->setValue("Scene/CellCurseAgeIndicate", SCENE_CURSE_CELL_AGE_INDICATE);
    m_CellCurseAgeIndicate = m_Settings->value("Scene/CellCurseAgeIndicate").toBool();

    if(!m_Settings->contains("Scene/CellDeadColor"))
        m_Settings->setValue("Scene/CellDeadColor", SCENE_CELL_DEAD_COLOR);
    m_SceneCellDeadColor = m_Settings->value("Scene/CellDeadColor").toString();

    if(!m_Settings->contains("Scene/CellAliveColor"))
        m_Settings->setValue("Scene/CellAliveColor", SCENE_CELL_ALIVE_COLOR);
    m_SceneCellAliveColor = m_Settings->value("Scene/CellAliveColor").toString();

    if(!m_Settings->contains("Scene/CellTraitColor"))
        m_Settings->setValue("Scene/CellTraitColor", SCENE_CELL_TRAIT_COLOR);
    m_SceneCellTraitColor = m_Settings->value("Scene/CellTraitColor").toString();

    if(!m_Settings->contains("Scene/SelectColor"))
        m_Settings->setValue("Scene/SelectColor", SCENE_SELECT_COLOR);
    m_SceneSelectColor = m_Settings->value("Scene/SelectColor").toString();

    if(!m_Settings->contains("Scene/ViewUpdateMode"))
        m_Settings->setValue("Scene/ViewUpdateMode", SCENE_VIEW_UPDATE_MODE);
    m_SceneViewUpdateMode = m_Settings->value("Scene/ViewUpdateMode").toString();

    if(!m_Settings->contains("Scene/FieldThreadPriority"))
        m_Settings->setValue("Scene/FieldThreadPriority", SCENE_FIELD_THREAD_PRIORITY);
    m_SceneFieldThreadPriority = m_Settings->value("Scene/FieldThreadPriority").toString();

    if(!m_Settings->contains("Scene/CellCurseColor"))
        m_Settings->setValue("Scene/CellCurseColor", SCENE_CELL_CURSE_COLOR);
    m_SceneCellCurseColor = m_Settings->value("Scene/CellCurseColor").toString();

    if(!m_Settings->contains("Scene/CellAliveAgeIndicationBrightness"))
        m_Settings->setValue("Scene/CellAliveAgeIndicationBrightness", CELL_ALIVE_AGE_INDIC_BRIGHTNESS);
    m_CellAliveAgeIndicBright = m_Settings->value("Scene/CellAliveAgeIndicationBrightness").toString();

    if(!m_Settings->contains("Scene/CellCurseAgeIndicationBrightness"))
        m_Settings->setValue("Scene/CellCurseAgeIndicationBrightness", CELL_CURSE_AGE_INDIC_BRIGHTNESS);
    m_CellCurseAgeIndicBright = m_Settings->value("Scene/CellCurseAgeIndicationBrightness").toString();

    if(!m_Settings->contains("Scene/CalculatingMinPause"))
        m_Settings->setValue("Scene/CalculatingMinPause", SCENE_CALCULATING_MIN_PAUSE);
    m_SceneCalculatingMinPause = m_Settings->value("Scene/CalculatingMinPause").toInt();

    if(!m_Settings->contains("Scene/ItemZValue"))
        m_Settings->setValue("Scene/ItemZValue", SCENE_ITEM_ZVALUE);
    m_SceneItemZValue = m_Settings->value("Scene/ItemZValue").toInt();

    if(!m_Settings->contains("Scene/MultiselectAlfa"))
        m_Settings->setValue("Scene/MultiselectAlfa", SCENE_MULTISELECT_ALFA);
    m_SceneMultiselAlfa = m_Settings->value("Scene/MultiselectAlfa").toInt();

    if(!m_Settings->contains("Scene/SelectAlfa"))
        m_Settings->setValue("Scene/SelectAlfa", SCENE_SELECT_ALFA);
    m_SceneSelAlfa = m_Settings->value("Scene/SelectAlfa").toInt();

    if(!m_Settings->contains("Scene/CellAliveAgeIndicationFactor"))
        m_Settings->setValue("Scene/CellAliveAgeIndicationFactor", CELL_ALIVE_AGE_INDIC_FACTOR);
    m_CellAliveAgeIndicFactor = m_Settings->value("Scene/CellAliveAgeIndicationFactor").toInt();

    if(!m_Settings->contains("Scene/CellCurseAgeIndicationFactor"))
        m_Settings->setValue("Scene/CellCurseAgeIndicationFactor", CELL_CURSE_AGE_INDIC_FACTOR);
    m_CellCurseAgeIndicFactor = m_Settings->value("Scene/CellCurseAgeIndicationFactor").toInt();

    if(!m_Settings->contains("Scene/CellAliveAgeIndicationDiapason"))
        m_Settings->setValue("Scene/CellAliveAgeIndicationDiapason", CELL_ALIVE_AGE_INDIC_DIAPASON);
    m_CellAliveAgeIndicDiapason = m_Settings->value("Scene/CellAliveAgeIndicationDiapason").toInt();

    if(!m_Settings->contains("Scene/CellCurseAgeIndicationDiapason"))
        m_Settings->setValue("Scene/CellCurseAgeIndicationDiapason", CELL_CURSE_AGE_INDIC_DIAPASON);
    m_CellCurseAgeIndicDiapason = m_Settings->value("Scene/CellCurseAgeIndicationDiapason").toInt();

    if(!m_Settings->contains("Scene/CreateDefaultRule"))
        m_Settings->setValue("Scene/CreateDefaultRule", SCENE_CREATE_DEFAULT_RULE);
    m_SceneCreateDefaultRule = m_Settings->value("Scene/CreateDefaultRule").toBool();

    if(!m_Settings->contains("Scene/LastRule"))
        m_Settings->setValue("Scene/LastRule", "");
    m_SceneLastRule = m_Settings->value("Scene/LastRule").toString();

    if(!m_Settings->contains("Scene/GridLineWidth"))
        m_Settings->setValue("Scene/GridLineWidth", SCENE_GRID_LINE_WIDTH);
    m_SceneGridLineWidth = m_Settings->value("Scene/GridLineWidth").toInt();

    if(!m_Settings->contains("Field/Size"))
        m_Settings->setValue("Field/Size", SCENE_FIELD_SIZE);
    m_SceneFieldSize = m_Settings->value("Field/Size").toInt();

    if(!m_Settings->contains("Field/PauseAtAge"))
        m_Settings->setValue("Field/PauseAtAge", FIELD_PAUSE_AT_AGE);
    m_FieldPauseAtAge = m_Settings->value("Field/PauseAtAge").toUInt();

    if(!m_Settings->contains("Field/RandomisationValue"))
        m_Settings->setValue("Field/RandomisationValue", FIELD_RANDOMISATION_VALUE);
    m_FieldRandomisationValue = m_Settings->value("Field/RandomisationValue").toInt();

    if(!m_Settings->contains("Field/FirstSnapshot"))
        m_Settings->setValue("Field/FirstSnapshot", FIELD_FIRST_SNAPSHOT);
    m_FieldFirstSnapshot = m_Settings->value("Field/FirstSnapshot").toBool();

    if(!m_Settings->contains("Field/FirstSnapshotClearList"))
        m_Settings->setValue("Field/FirstSnapshotClearList", FIELD_FIRST_SNAPSHOT_CLEAR_LIST);
    m_FieldFirstSnapshotClearList = m_Settings->value("Field/FirstSnapshotClearList").toBool();

    if(!m_Settings->contains("Field/SnapshotAtEveryTime"))
        m_Settings->setValue("Field/SnapshotAtEveryTime", FIELD_SNAPSHOT_AT_EVERY_TIME);
    m_FieldSnapshotAtEveryTime = m_Settings->value("Field/SnapshotAtEveryTime").toUInt();
}

void Config::setFieldSnapshotAtEveryTime(uint value)
{
    if(m_FieldSnapshotAtEveryTime == value) return;

    m_FieldSnapshotAtEveryTime = value;
    m_Settings->setValue("Field/SnapshotAtEveryTime", m_FieldSnapshotAtEveryTime);
}

void Config::setTasksWindowWidth(int value)
{
    if(m_TasksWindowWidth == value) return;
    
    m_TasksWindowWidth = value;
    m_Settings->setValue("MainWindow/TasksWindowWidth", m_TasksWindowWidth);
}

void Config::setTasksWindowHeight(int value)
{
    if(m_TasksWindowHeight == value) return;

    m_TasksWindowHeight = value;
    m_Settings->setValue("MainWindow/TasksWindowHeight", m_TasksWindowHeight);
}

void Config::setFieldPauseAtAge(uint value)
{
    if(m_FieldPauseAtAge == value) return;

    m_FieldPauseAtAge = value;
    m_Settings->setValue("Field/PauseAtAge", m_FieldPauseAtAge);
}

void Config::setImageAutoopen(bool value)
{
    if(m_ImageAutoopen == value) return;
    
    m_ImageAutoopen = value;
    m_Settings->setValue("ImageAutoopen", m_ImageAutoopen);
}

void Config::setReportAutoopen(bool value)
{
    if(m_ReportAutoopen == value) return;

    m_ReportAutoopen = value;
    m_Settings->setValue("ReportAutoopen", m_ReportAutoopen);
}

void Config::setReportWindowWidth(int value)
{
    if(m_ReportWindowWidth == value) return;
    
    m_ReportWindowWidth = value;
    m_Settings->setValue("MainWindow/ReportWindowWidth", m_ReportWindowWidth);
}

void Config::setReportWindowHeight(int value)
{
    if(m_ReportWindowHeight == value) return;

    m_ReportWindowHeight = value;
    m_Settings->setValue("MainWindow/ReportWindowHeight", m_ReportWindowHeight);
}

void Config::setReportFileFormat(const QString &value)
{
    if(m_ReportFileFormat == value) return;

    m_ReportFileFormat = value;
    m_Settings->setValue("ReportFileFormat", m_ReportFileFormat);
}

void Config::setInfoRuleWindowWidth(int value)
{
    if(m_InfoRuleWindowWidth == value) return;

    m_InfoRuleWindowWidth = value;
    m_Settings->setValue("MainWindow/InfoRuleWindowWidth", m_InfoRuleWindowWidth);
}

void Config::setInfoRuleWindowHeight(int value)
{
    if(m_InfoRuleWindowHeight == value) return;

    m_InfoRuleWindowHeight = value;
    m_Settings->setValue("MainWindow/InfoRuleWindowHeight", m_InfoRuleWindowHeight);
}

void Config::setHelpWindowWidth(int value)
{
    if(m_HelpWindowWidth == value) return;

    m_HelpWindowWidth = value;
    m_Settings->setValue("MainWindow/HelpWindowWidth", m_HelpWindowWidth);
}

void Config::setHelpWindowHeight(int value)
{
    if(m_HelpWindowHeight == value) return;

    m_HelpWindowHeight = value;
    m_Settings->setValue("MainWindow/HelpWindowHeight", m_HelpWindowHeight);
}

void Config::setRulePropertyWindowWidth(int value)
{
    if(m_RulePropertyWindowWidth == value) return;

    m_RulePropertyWindowWidth = value;
    m_Settings->setValue("MainWindow/RulePropertyWindowWidth", m_RulePropertyWindowWidth);
}

void Config::setRulePropertyWindowHeight(int value)
{
    if(m_RulePropertyWindowHeight == value) return;

    m_RulePropertyWindowHeight = value;
    m_Settings->setValue("MainWindow/RulePropertyWindowHeight", m_RulePropertyWindowHeight);
}

void Config::setEditActivityWindowWidth(int value)
{
    if(m_EditActivityWindowWidth == value) return;

    m_EditActivityWindowWidth = value;
    m_Settings->setValue("MainWindow/EditActivityWindowWidth", m_EditActivityWindowWidth);
}

void Config::setEditActivityWindowHeight(int value)
{
    if(m_EditActivityWindowHeight == value) return;

    m_EditActivityWindowHeight = value;
    m_Settings->setValue("MainWindow/EditActivityWindowHeight", m_EditActivityWindowHeight);
}

void Config::setEditCellWindowWidth(int value)
{
    if(m_EditCellWindowWidth == value) return;

    m_EditCellWindowWidth = value;
    m_Settings->setValue("MainWindow/EditCellWindowWidth", m_EditCellWindowWidth);
}

void Config::setEditCellWindowHeight(int value)
{
    if(m_EditCellWindowHeight == value) return;

    m_EditCellWindowHeight = value;
    m_Settings->setValue("MainWindow/EditCellWindowHeight", m_EditCellWindowHeight);
}

void Config::setProjectWindowWidth(int value)
{
    if(m_ProjectWindowWidth == value) return;

    m_ProjectWindowWidth = value;
    m_Settings->setValue("MainWindow/ProjectWindowWidth", m_ProjectWindowWidth);
}

void Config::setProjectWindowHeight(int value)
{
    if(m_ProjectWindowHeight == value) return;

    m_ProjectWindowHeight = value;
    m_Settings->setValue("MainWindow/ProjectWindowHeight", m_ProjectWindowHeight);
}

void Config::setRandomFillWindowWidth(int value)
{
    if(m_RandomFillWindowWidth == value) return;

    m_RandomFillWindowWidth = value;
    m_Settings->setValue("MainWindow/RandomFillWindowWidth", m_RandomFillWindowWidth);
}

void Config::setRandomFillWindowHeight(int value)
{
    if(m_RandomFillWindowHeight == value) return;

    m_RandomFillWindowHeight = value;
    m_Settings->setValue("MainWindow/RandomFillWindowHeight", m_RandomFillWindowHeight);
}

void Config::setSnapshotWindowWidth(int value)
{
    if(m_SnapshotWindowWidth == value) return;

    m_SnapshotWindowWidth = value;
    m_Settings->setValue("MainWindow/SnapshotWindowWidth", m_SnapshotWindowWidth);
}

void Config::setSnapshotWindowHeight(int value)
{
    if(m_SnapshotWindowHeight == value) return;

    m_SnapshotWindowHeight = value;
    m_Settings->setValue("MainWindow/SnapshotWindowHeight", m_SnapshotWindowHeight);
}

void Config::setSetupWindowWidth(int value)
{
    if(m_SetupWindowWidth == value) return;

    m_SetupWindowWidth = value;
    m_Settings->setValue("MainWindow/SetupWindowWidth", m_SetupWindowWidth);
}

void Config::setSetupWindowHeight(int value)
{
    if(m_SetupWindowHeight == value) return;

    m_SetupWindowHeight = value;
    m_Settings->setValue("MainWindow/SetupWindowHeight", m_SetupWindowHeight);
}

void Config::setHelpPage(const QString &value)
{
    if(m_HelpPage == value) return;
    
    m_HelpPage = value;
    m_Settings->setValue("MainWindow/HelpPage", m_HelpPage);
}

void Config::setFieldFirstSnapshotClearList(bool value)
{
    if(m_FieldFirstSnapshotClearList == value) return;
    
    m_FieldFirstSnapshotClearList = value;
    m_Settings->setValue("Field/FirstSnapshotClearList", m_FieldFirstSnapshotClearList);
}

void Config::setSceneLastRule(const QString &value)
{
    if(m_SceneLastRule == value) return;

    m_SceneLastRule = value;
    m_Settings->setValue("Scene/LastRule", m_SceneLastRule);
}


void Config::setFieldFasteditKeyModifier(const Qt::KeyboardModifiers &value)
{
    if(m_FieldFasteditKeyModifier == value) return;

    m_FieldFasteditKeyModifier = value;
    m_Settings->setValue("Scene/FieldFasteditKeyModifier", static_cast<int>(m_FieldFasteditKeyModifier));
}

void Config::setCellMonitorWindowWidth(int value)
{
    if(m_CellMonitorWindowWidth == value) return;

    m_CellMonitorWindowWidth = value;
    m_Settings->setValue("MainWindow/CellMonitorWindowWidth", m_CellMonitorWindowWidth);
}

void Config::setCellMonitorWindowHeight(int value)
{
    if(m_CellMonitorWindowHeight == value) return;

    m_CellMonitorWindowHeight = value;
    m_Settings->setValue("MainWindow/CellMonitorWindowHeight", m_CellMonitorWindowHeight);
}

void Config::setSceneGridLineWidth(int value)
{
    if(m_SceneGridLineWidth == value) return;

    m_SceneGridLineWidth = value;
    m_Settings->setValue("Scene/GridLineWidth", m_SceneGridLineWidth);
}

void Config::setFieldRandomisationValue(int value)
{
    if(m_FieldRandomisationValue == value) return;

    m_FieldRandomisationValue = value;
    m_Settings->setValue("Field/RandomisationValue", m_FieldRandomisationValue);
}

void Config::setInfoWindowWidth(int value)
{
    if(m_InfoWindowWidth == value) return;

    m_InfoWindowWidth = value;
    m_Settings->setValue("MainWindow/InfoWindowWidth", m_InfoWindowWidth);
}

void Config::setInfoWindowHeight(int value)
{
    if(m_InfoWindowHeight == value) return;

    m_InfoWindowHeight = value;
    m_Settings->setValue("MainWindow/InfoWindowHeight", m_InfoWindowHeight);
}

void Config::setProjectFileCompressionLevel(int value)
{
    if(m_ProjectFileCompressionLevel == value) return;

    m_ProjectFileCompressionLevel = value;
    m_Settings->setValue("ProjectFileCompressionLevel", m_ProjectFileCompressionLevel);
}

void Config::setProjectFileCompression(bool value)
{
    if(m_ProjectFileCompression == value) return;

    m_ProjectFileCompression = value;
    m_Settings->setValue("ProjectFileCompression", m_ProjectFileCompression);
}

void Config::setSceneCreateDefaultRule(bool value)
{
    if(m_SceneCreateDefaultRule == value) return;
    
    m_SceneCreateDefaultRule = value;
    m_Settings->setValue("Scene/CreateDefaultRule", m_SceneCreateDefaultRule);
}

void Config::setEditRulesWindowWidth(int value)
{
    if(m_EditRulesWindowWidth == value) return;

    m_EditRulesWindowWidth = value;
    m_Settings->setValue("MainWindow/EditRulesWindowWidth", m_EditRulesWindowWidth);
}

void Config::setEditRulesWindowHeight(int value)
{
    if(m_EditRulesWindowHeight == value) return;

    m_EditRulesWindowHeight = value;
    m_Settings->setValue("MainWindow/EditRulesWindowHeight", m_EditRulesWindowHeight);
}

void Config::setPathLogsDir(const QString &value)
{
    m_PathLogsDir = value;
}

void Config::setPathPresetsDir(const QString &value)
{
    m_PathPresetsDir = value;
}

void Config::setPathRulesDir(const QString &value)
{
    m_PathRulesDir = value;
}

void Config::setPathPojectsDir(const QString &value)
{
    m_PathPojectsDir = value;
}

void Config::setPathReportsDir(const QString &value)
{
    m_PathReportsDir = value;
}

void Config::setCellCurseAgeIndicDiapason(int value)
{
    if(m_CellCurseAgeIndicDiapason == value) return;

    m_CellCurseAgeIndicDiapason = value;
    m_Settings->setValue("Scene/CellCurseAgeIndicationDiapason", m_CellCurseAgeIndicDiapason);
}

void Config::setCellCurseAgeIndicFactor(int value)
{
    if(m_CellCurseAgeIndicFactor == value) return;

    m_CellCurseAgeIndicFactor = value;
    m_Settings->setValue("Scene/CellCurseAgeIndicationFactor", m_CellCurseAgeIndicFactor);
}

void Config::setCellCurseAgeIndicBright(const QString &value)
{
    if(m_CellCurseAgeIndicBright == value) return;

    m_CellCurseAgeIndicBright = value;
    m_Settings->setValue("Scene/CellCurseAgeIndicationBrightness", m_CellCurseAgeIndicBright);
}

void Config::setCellCurseAgeIndicate(bool value)
{
    if(m_CellCurseAgeIndicate == value) return;

    m_CellCurseAgeIndicate = value;
    m_Settings->setValue("Scene/CellCurseAgeIndicate", m_CellCurseAgeIndicate);
}

void Config::setCellAliveAgeIndicDiapason(int value)
{
    if(m_CellAliveAgeIndicDiapason == value) return;

    m_CellAliveAgeIndicDiapason = value;
    m_Settings->setValue("Scene/CellAliveAgeIndicationDiapason", m_CellAliveAgeIndicDiapason);
}

void Config::setCellAliveAgeIndicFactor(int value)
{
    if(m_CellAliveAgeIndicFactor == value) return;

    m_CellAliveAgeIndicFactor = value;
    m_Settings->setValue("Scene/CellAliveAgeIndicationFactor", m_CellAliveAgeIndicFactor);
}

void Config::setCellAliveAgeIndicBright(const QString &value)
{
    if(m_CellAliveAgeIndicBright == value) return;

    m_CellAliveAgeIndicBright = value;
    m_Settings->setValue("Scene/CellAliveAgeIndicationBrightness", m_CellAliveAgeIndicBright);
}

void Config::setSceneCellAliveColor(const QString &value)
{
    if(m_SceneCellAliveColor == value) return;

    m_SceneCellAliveColor = value;
    m_Settings->setValue("Scene/CellAliveColor", m_SceneCellAliveColor);
}

void Config::setSceneCellTraitColor(const QString &value)
{
    if(m_SceneCellTraitColor == value) return;

    m_SceneCellTraitColor = value;
    m_Settings->setValue("Scene/CellTraitColor", m_SceneCellTraitColor);
}

void Config::setSplashSize(int value)
{
    if(m_SplashSize == value) return;

    m_SplashSize = value;
    m_Settings->setValue("MainWindow/SplashSize", m_SplashSize);
}

void Config::setSplashTime(int value)
{
    if(m_SplashTime == value) return;

    m_SplashTime = value;
    m_Settings->setValue("MainWindow/SplashTime", m_SplashTime);
}

void Config::setFieldFirstSnapshot(bool value)
{
    if(m_FieldFirstSnapshot == value) return;

    m_FieldFirstSnapshot = value;
    m_Settings->setValue("Field/FirstSnapshot", m_FieldFirstSnapshot);
}

void Config::setSceneSelAlfa(int value)
{
    if(m_SceneSelAlfa == value) return;

    m_SceneSelAlfa = value;
    m_Settings->setValue("Scene/SelectAlfa", m_SceneSelAlfa);
}

void Config::setRewriteResource(bool value)
{
    if(m_RewriteResource == value) return;

    m_RewriteResource = value;
    m_Settings->setValue("RewriteResource", m_RewriteResource);
}

void Config::setJsonIgnoreDataVersion(bool value)
{
    if(m_JsonIgnoreDataVersion == value) return;

    m_JsonIgnoreDataVersion = value;
    m_Settings->setValue("JsonIgnoreDataVersion", m_JsonIgnoreDataVersion);
}

void Config::setSaveToPresetExceptDead(bool value)
{
    if(m_SaveToPresetExceptDead == value) return;
    
    m_SaveToPresetExceptDead = value;
    m_Settings->setValue("SaveToPresetExceptDead", m_SaveToPresetExceptDead);
}

void Config::setCopyToClipboardExceptDead(bool value)
{
    if(m_CopyToClipboardExceptDead == value) return;

    m_CopyToClipboardExceptDead = value;
    m_Settings->setValue("CopyToClipboardExceptDead", m_CopyToClipboardExceptDead);
}

void Config::setJsonCompactMode(bool value)
{
    if(m_JsonCompactMode == value) return;
    
    m_JsonCompactMode = value;
    m_Settings->setValue("JsonCompactMode", m_JsonCompactMode);
}

void Config::setSceneMultiselAlfa(int value)
{
    if(m_SceneMultiselAlfa == value) return;
    
    m_SceneMultiselAlfa = value;
    m_Settings->setValue("Scene/MultiselectAlfa", m_SceneMultiselAlfa);
}

void Config::setSceneMultiselModifier(const Qt::KeyboardModifiers &value)
{
    if(m_SceneMultiselKeyModifier == value) return;

    m_SceneMultiselKeyModifier = value;
    m_Settings->setValue("Scene/MultiselectKeyModifier", static_cast<int>(m_SceneMultiselKeyModifier));
}

void Config::setWindowShowFieldInfo(bool value)
{
    if(m_WindowShowFieldInfo == value) return;

    m_WindowShowFieldInfo = value;
    m_Settings->setValue("MainWindow/ShowFieldInformation", m_WindowShowFieldInfo);
}

void Config::setImageFileFormat(const QString &value)
{
    if(m_ImageFileFormat == value) return;

    m_ImageFileFormat = value;
    m_Settings->setValue("ImageFileFormat", m_ImageFileFormat);
}

void Config::setFieldInfoWindowWidth(int value)
{
    if(m_FieldInfoWindowWidth == value) return;

    m_FieldInfoWindowWidth = value;
    m_Settings->setValue("MainWindow/FieldInfoWindowWidth", m_FieldInfoWindowWidth);
}

void Config::setFieldInfoWindowHeight(int value)
{
    if(m_FieldInfoWindowHeight == value) return;

    m_FieldInfoWindowHeight = value;
    m_Settings->setValue("MainWindow/FieldInfoWindowHeight", m_FieldInfoWindowHeight);
}

void Config::setSceneToolTipModifier(const Qt::KeyboardModifiers &value)
{
    if(m_SceneTooltipKeyModifier == value) return;

    m_SceneTooltipKeyModifier = value;
    m_Settings->setValue("Scene/TooltipKeyModifier", static_cast<int>(m_SceneTooltipKeyModifier));
}

void Config::setSceneItemZValue(int value)
{
    if(m_SceneItemZValue == value) return;
    
    m_SceneItemZValue = value;
    m_Settings->setValue("Scene/ItemZValue", m_SceneItemZValue);
}

void Config::setSceneFieldThreadPriority(const QString &value)
{
    if(m_SceneFieldThreadPriority == value) return;
    
    m_SceneFieldThreadPriority = value;
    m_Settings->setValue("Scene/FieldThreadPriority", m_SceneFieldThreadPriority);
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
    m_Settings->setValue("Scene/SelectColor", m_SceneSelectColor);
}

void Config::setCellAliveAgeIndicate(bool value)
{
    if(m_CellAliveAgeIndicate == value) return;

    m_CellAliveAgeIndicate = value;
    m_Settings->setValue("Scene/CellAliveAgeIndicate", m_CellAliveAgeIndicate);
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
    m_Settings->setValue("Field/Size", m_SceneFieldSize);
}

void Config::setSceneZoomModifier(const Qt::KeyboardModifiers &value)
{
    if(m_SceneZoomKeyModifier == value) return;

    m_SceneZoomKeyModifier = value;
    m_Settings->setValue("Scene/ZoomKeyModifier", static_cast<int>(m_SceneZoomKeyModifier));
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

void Config::setWriteLogsToFile(bool value)
{
    if(m_WriteLogsToFile == value) return;
    
    m_WriteLogsToFile = value;
    m_Settings->setValue("WriteLogsToFile", m_WriteLogsToFile);
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
    m_Settings->setValue("DateTimeFormat", m_DateTimeFormat);
}

void Config::setButtonSize(int value)
{
    if(m_ButtonSize == value) return;

    m_ButtonSize = value;
    m_Settings->setValue("MainWindow/ButtonSize", m_ButtonSize);
}

Qt::KeyboardModifiers Config::SceneMultiselModifier() const { return m_SceneMultiselKeyModifier; }
QString Config::SceneViewUpdateMode() const { return m_SceneViewUpdateMode; }
int Config::CellInfoWindowWidth() const { return m_CellInfoWindowWidth; }
int Config::CellInfoWindowHeight() const { return m_CellInfoWindowHeight; }
int Config::InfoWindowWidth() const { return m_InfoWindowWidth; }
int Config::InfoWindowHeight() const { return m_InfoWindowHeight; }
QString Config::SceneCellCurseColor() const { return m_SceneCellCurseColor; }
QString Config::SceneSelectColor() const { return m_SceneSelectColor; }
bool Config::CellAliveAgeIndicate() const { return m_CellAliveAgeIndicate; }
QString Config::SceneCellDeadColor() const { return m_SceneCellDeadColor; }
int Config::SceneFieldSize() const { return m_SceneFieldSize; }
Qt::KeyboardModifiers Config::SceneZoomModifier() const { return m_SceneZoomKeyModifier; }
Qt::KeyboardModifiers Config::SceneToolTipModifier() const { return m_SceneTooltipKeyModifier; }
bool Config::SceneViewAntialiasing() const { return m_SceneViewAntialiasing; }
int Config::SceneCellSize() const { return m_SceneCellSize; }
qreal Config::SceneScaleStep() const { return m_SceneScaleStep; }
QString Config::SceneBgColor() const { return m_SceneBgColor; }
bool Config::WriteLogsToFile() const { return m_WriteLogsToFile; }
QString Config::LastDir() const { return m_LastDir; }
QString Config::DateTimeFormat() const { return m_DateTimeFormat; }
int Config::ButtonSize() const { return m_ButtonSize; }
QString Config::PathAppDir() const { return m_PathAppDir; }
QString Config::PathAppConfig() const { return m_PathAppConfig; }
int Config::SceneCalculatingMinPause() const { return m_SceneCalculatingMinPause; }
QString Config::SceneFieldThreadPriority() const { return m_SceneFieldThreadPriority; }
int Config::SceneItemZValue() const { return m_SceneItemZValue; }
int Config::FieldInfoWindowWidth() const { return m_FieldInfoWindowWidth; }
int Config::FieldInfoWindowHeight() const { return m_FieldInfoWindowHeight; }
QString Config::ImageFileFormat() const { return m_ImageFileFormat; }
bool Config::WindowShowFieldInfo() const { return m_WindowShowFieldInfo; }
int Config::SceneMultiselAlfa() const { return m_SceneMultiselAlfa; }
bool Config::JsonCompactMode() const { return m_JsonCompactMode; }
bool Config::SaveToPresetExceptDead() const { return m_SaveToPresetExceptDead; }
bool Config::CopyToClipboardExceptDead() const { return m_CopyToClipboardExceptDead; }
bool Config::JsonIgnoreDataVersion() const { return m_JsonIgnoreDataVersion; }
QString Config::PathPresetsDir() const { return m_PathPresetsDir; }
QString Config::PathLogsDir() const { return m_PathLogsDir; }
bool Config::RewriteResource() const { return m_RewriteResource; }
int Config::SceneSelAlfa() const { return m_SceneSelAlfa; }
bool Config::FieldFirstSnapshot() const { return m_FieldFirstSnapshot; }
int Config::SplashSize() const { return m_SplashSize; }
int Config::SplashTime() const { return m_SplashTime; }
QString Config::SceneCellAliveColor() const { return m_SceneCellAliveColor; }
QString Config::CellAliveAgeIndicBright() const { return m_CellAliveAgeIndicBright; }
int Config::CellAliveAgeIndicFactor() const { return m_CellAliveAgeIndicFactor; }
int Config::CellAliveAgeIndicDiapason() const { return m_CellAliveAgeIndicDiapason; }
bool Config::CellCurseAgeIndicate() const { return m_CellCurseAgeIndicate; }
QString Config::CellCurseAgeIndicBright() const { return m_CellCurseAgeIndicBright; }
int Config::CellCurseAgeIndicFactor() const { return m_CellCurseAgeIndicFactor; }
int Config::CellCurseAgeIndicDiapason() const { return m_CellCurseAgeIndicDiapason; }
QString Config::PathRulesDir() const { return m_PathRulesDir; }
QString Config::PathPojectsDir() const { return m_PathPojectsDir; }
QString Config::PathReportsDir() const { return m_PathReportsDir; }
int Config::EditRulesWindowWidth() const { return m_EditRulesWindowWidth; }
int Config::EditRulesWindowHeight() const { return m_EditRulesWindowHeight; }
bool Config::SceneCreateDefaultRule() const { return m_SceneCreateDefaultRule; }
bool Config::ProjectFileCompression() const { return m_ProjectFileCompression; }
int Config::ProjectFileCompressionLevel() const { return m_ProjectFileCompressionLevel; }
int Config::FieldRandomisationValue() const { return m_FieldRandomisationValue; }
int Config::SceneGridLineWidth() const { return m_SceneGridLineWidth; }
int Config::CellMonitorWindowHeight() const { return m_CellMonitorWindowHeight; }
int Config::CellMonitorWindowWidth() const { return m_CellMonitorWindowWidth; }
Qt::KeyboardModifiers Config::FieldFasteditKeyModifier() const { return m_FieldFasteditKeyModifier; }
QString Config::SceneLastRule() const { return m_SceneLastRule; }
bool Config::FieldFirstSnapshotClearList() const { return m_FieldFirstSnapshotClearList; }
QString Config::SceneCellTraitColor() const { return m_SceneCellTraitColor; }
QString Config::HelpPage() const { return m_HelpPage; }
int Config::SetupWindowWidth() const { return m_SetupWindowWidth; }
int Config::SetupWindowHeight() const { return m_SetupWindowHeight; }
int Config::SnapshotWindowWidth() const { return m_SnapshotWindowWidth; }
int Config::SnapshotWindowHeight() const { return m_SnapshotWindowHeight; }
int Config::RandomFillWindowWidth() const { return m_RandomFillWindowWidth; }
int Config::RandomFillWindowHeight() const { return m_RandomFillWindowHeight; }
int Config::ProjectWindowWidth() const { return m_ProjectWindowWidth; }
int Config::ProjectWindowHeight() const { return m_ProjectWindowHeight; }
int Config::EditCellWindowWidth() const { return m_EditCellWindowWidth; }
int Config::EditCellWindowHeight() const { return m_EditCellWindowHeight; }
int Config::EditActivityWindowWidth() const { return m_EditActivityWindowWidth; }
int Config::EditActivityWindowHeight() const { return m_EditActivityWindowHeight; }
int Config::RulePropertyWindowWidth() const { return m_RulePropertyWindowWidth; }
int Config::RulePropertyWindowHeight() const { return m_RulePropertyWindowHeight; }
int Config::HelpWindowWidth() const { return m_HelpWindowWidth; }
int Config::HelpWindowHeight() const { return m_HelpWindowHeight; }
int Config::InfoRuleWindowWidth() const { return m_InfoRuleWindowWidth; }
int Config::InfoRuleWindowHeight() const { return m_InfoRuleWindowHeight; }
int Config::ReportWindowWidth() const { return m_ReportWindowWidth; }
int Config::ReportWindowHeight() const { return m_ReportWindowHeight; }
QString Config::ReportFileFormat() const { return m_ReportFileFormat; }
bool Config::ImageAutoopen() const { return m_ImageAutoopen; }
bool Config::ReportAutoopen() const { return m_ReportAutoopen; }
uint Config::FieldPauseAtAge() const { return m_FieldPauseAtAge; }
int Config::TasksWindowWidth() const { return m_TasksWindowWidth; }
int Config::TasksWindowHeight() const { return m_TasksWindowHeight; }
uint Config::FieldSnapshotAtEveryTime() const { return m_FieldSnapshotAtEveryTime; }
// Unsaved
bool Config::UnsavedTasksEnabled() const { return m_UnsavedTasksEnabled; }
void Config::setUnsavedTasksEnabled(bool value) { m_UnsavedTasksEnabled = value; }
