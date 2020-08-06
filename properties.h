/*
 * параметры приложения
 */
#ifndef PROPERTIES_H
#define PROPERTIES_H

#include "config.h"

#include <QString>

extern Config* config; // интерфейс над QSettings

// дефолтные параметры
const QStringList SCENE_VIEW_UPDATE_MODES =         {"SMART", "MINIMAL", "FULL"};
const QStringList SCENE_FIELD_THREAD_PRIORITIES =   {"LOW", "NORMAL", "HIGH", "HIGHEST"};
const QString DT_FORMAT =                           "yyyy.MM.dd hh:mm:ss";
const QString LOG_FILE_NAME_FORMAT =                "yyyyMMdd_hhmmss";
const QString TEXT_CODEC =                          "UTF-8";
const QString APP_NAME =                            "Fungus";
const QString APP_CFG =                             "config.cfg";
const QString SCENE_SELECT_COLOR =                  "#5555FF";
const QString SCENE_BG_COLOR =                      "#757575";
const QString SCENE_CELL_DEAD_COLOR =               "#FFFFFF";
const QString SCENE_CELL_CURSE_COLOR =              "#FF0000";
const QString SCENE_VIEW_UPDATE_MODE =              "MINIMAL";
const QString SCENE_FIELD_THREAD_PRIORITY =         "NORMAL";
const QString IMAGE_FILE_FORMAT =                   "PNG";
const QString PRESET_FILE_EXTENSION =               "FPR";
const QString PROJECT_FILE_EXTENSION =              "FPJ";
const QString LOG_FILE_EXTENSION =                  "LOG";
const QString PRESET_DIRECTORY =                    "presets";
const QString LOG_DIRECTORY =                       "logs";
const qreal SCENE_SCALE_STEP =                      1.0001;
const auto SCENE_CELL_SIZE =                        10;
const auto SCENE_MULTISELECT_ALFA =                 50; // 0-254
const auto SCENE_SELECT_ALFA =                      100; // 0-254
const auto SCENE_VIEW_ANTIALIASING =                true;
const auto SCENE_ZOOM_KEY_MODIFIER =                Qt::ControlModifier;
const auto SCENE_TOOLTIP_KEY_MODIFIER =             Qt::ShiftModifier;
const auto SCENE_MULTISELECT_KEY_MODIFIER =         Qt::ControlModifier;
const auto SCENE_CALCULATING_MIN_PAUSE =            50;
const auto SCENE_FIELD_SIZE =                       100;
const auto WINDOW_HEIGHT =                          700;
const auto WINDOW_WIDTH =                           900;
const auto CELL_INFO_WINDOW_HEIGHT =                300;
const auto CELL_INFO_WINDOW_WIDTH =                 300;
const auto FIELD_INFO_WINDOW_HEIGHT =               300;
const auto FIELD_INFO_WINDOW_WIDTH =                300;
const auto BUTTON_SIZE =                            24;
const auto SCENE_ITEM_ZVALUE =                      0;
const auto SPLASH_SIZE =                            640;
const auto SPLASH_TIME =                            500;
const auto SCENE_CELL_AGE_INDICATE =                true;
const auto WINDOW_SHOW_FIELD_INFO =                 false;
const auto JSON_COMPACT_MODE =                      true;
const auto JSON_IGNORE_DATA_VERSION =               false;
const auto COPY_TO_CLIPBOARD_EXCEPT_DEAD =          true;
const auto SAVE_TO_PRESET_EXCEPT_DEAD =             true;
const auto REWITE_RESOURCES =                       false;
const auto SCENE_FIRST_SNAPSHOT =                   false;
const auto WRITE_LOGS_TO_FILE =                     false;

#endif // PROPERTIES_H
