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
const QString TEXT_CODEC =                          "UTF-8";
const QString APP_NAME =                            "Fungus";
const QString APP_CFG =                             "config.cfg";
const QString SCENE_SELECT_COLOR =                  "#5555FF";
const QString SCENE_BG_COLOR =                      "#757575";
const QString SCENE_CELL_DEAD_COLOR =               "#FFFFFF";
const QString SCENE_CELL_CURSE_COLOR =              "#FF0000";
const QString SCENE_VIEW_UPDATE_MODE =              "MINIMAL";
const QString SCENE_FIELD_THREAD_PRIORITY=          "NORMAL";
const QString SCENE_IMAGE_FILE_FORMAT=              "PNG";
const bool SCENE_CELL_AGE_INDICATE =                true;
const bool WINDOW_SHOW_FIELD_INFO =                 false;
const qreal SCENE_SCALE_STEP =                      1.001;
const auto SCENE_CELL_SIZE =                        10;
const auto SCENE_MULTISELECT_ALFA =                 50; // 0-254
const auto SCENE_VIEW_ANTIALIASING =                true;
const auto SCENE_ZOOM_MODIFIER =                    Qt::ControlModifier;
const auto SCENE_TOOLTIP_MODIFIER =                 Qt::ShiftModifier;
const auto SCENE_MULTISELECT_MODIFIER =             Qt::ControlModifier;
const auto SCENE_CALCULATING_MIN_PAUSE =            50;
const auto SCENE_FIELD_SIZE =                       100;
const auto WINDOW_HEIGHT =                          700;
const auto WINDOW_WIDTH =                           900;
const auto CELL_INFO_WINDOW_HEIGHT =                300;
const auto CELL_INFO_WINDOW_WIDTH =                 300;
const auto FIELD_INFO_WINDOW_HEIGHT =               300;
const auto FIELD_INFO_WINDOW_WIDTH =                300;
const auto LOG_SIZE =                               0;
const auto BUTTON_SIZE =                            32;
const auto SI_METRIC =                              false;
const auto SCENE_ITEM_ZVALUE =                      0;

#endif // PROPERTIES_H
