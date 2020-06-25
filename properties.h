/*
 * параметры приложения
 */
#ifndef PROPERTIES_H
#define PROPERTIES_H

#include "config.h"

#include <QString>

extern Config* config; // интерфейс над QSettings

// дефолтные параметры
const QString DT_FORMAT =                   "yyyy.MM.dd hh:mm:ss";
const QString TEXT_CODEC =                  "UTF-8";
const QString APP_NAME =                    "Fungus";
const QString APP_CFG =                     "config.cfg";
const QString SCENE_SELECT_COLOR =          "#0000FF";
const QString SCENE_BG_COLOR =              "#2E2E2E";
const QString SCENE_CELL_DEAD_COLOR =       "#FFFFFF";
const QString SCENE_CELL_CURSE_COLOR =      "#FF0000";
const bool SCENE_OBJECT_AGE_INDICATE =      true;
const qreal SCENE_SCALE_STEP =              1.001;
const auto SCENE_CELL_SIZE =                5;
const auto SCENE_BSP_TREE_INDEX =           true;
const auto SCENE_BSP_TREE_DEPTH =           10;
const auto SCENE_VIEW_ANTIALIASING =        true;
const auto SCENE_ZOOM_MODIFIER =            Qt::ControlModifier;
const auto SCENE_OBJECT_MODIFIER =          Qt::ControlModifier;
const auto SCENE_CALCULATING_MIN_PAUSE =    0;
const auto SCENE_FIELD_SIZE =               100;
const auto WINDOW_HEIGHT =                  700;
const auto WINDOW_WIDTH =                   900;
const auto CELL_INFO_WINDOW_HEIGHT =        300;
const auto CELL_INFO_WINDOW_WIDTH =         300;
const auto LOG_SIZE =                       0;
const auto BUTTON_SIZE =                    32;
const auto SI_METRIC =                      false;


#endif // PROPERTIES_H
