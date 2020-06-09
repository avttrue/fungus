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
const QString SCENE_COLOR =                 "#DCDCDC";
const QString SCENE_BORDER_COLOR =          "#8B0000";
const QString SCENE_OBJECT_DEAD_COLOR =     "#FFFFFF";
const QString SCENE_OBJECT_CURSE_COLOR =    "#FF0000";
const QString SCENE_OBJECT_ALIVE0_COLOR =   "#000000";
const QString SCENE_OBJECT_ALIVE1_COLOR =   "#2F4F4F";
const QString SCENE_OBJECT_ALIVE2_COLOR =   "#708090";
const QString SCENE_OBJECT_ALIVE3_COLOR =   "#778899";
const QString SCENE_OBJECT_ALIVE4_COLOR =   "#696969";
const QString SCENE_OBJECT_ALIVE5_COLOR =   "#808080";
const QString SCENE_OBJECT_ALIVE6_COLOR =   "#A9A9A9";
const QString SCENE_OBJECT_ALIVE7_COLOR =   "#C0C0C0";
const QString SCENE_OBJECT_ALIVE8_COLOR =   "#D3D3D3";
const QString SCENE_OBJECT_ALIVE9_COLOR =   "#DCDCDC";
const bool SCENE_OBJECT_AGE_INDICATE =      true;
const qreal SCENE_SCALE_STEP =              1.001;
const auto SCENE_OBJECT_SIZE =              1;
const auto SCENE_BSP_TREE_INDEX =           true;
const auto SCENE_BSP_TREE_DEPTH =           10;
const auto SCENE_VIEW_ANTIALIASING =        true;
const auto SCENE_PREFILL =                  true;
const auto SCENE_ZOOM_MODIFIER =            Qt::ControlModifier;
const auto SCENE_OBJECT_MODIFIER =          Qt::ControlModifier;
const auto SCENE_SIZE =                     100;
const auto WINDOW_HEIGHT =                  700;
const auto WINDOW_WIDTH =                   900;
const auto CELL_INFO_WINDOW_HEIGHT =        300;
const auto CELL_INFO_WINDOW_WIDTH =         300;
const auto LOG_SIZE =                       0;
const auto BUTTON_SIZE =                    32;
const auto SI_METRIC =                      false;


#endif // PROPERTIES_H
