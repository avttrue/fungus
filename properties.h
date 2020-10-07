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
const QStringList SCENE_CELL_BRIGHTNESS_VALUES =    {"LIGHTER", "DARKER"};
const QString APP_NAME =                            "Fungus";
const QString APP_VERSION =                         "1.1.8";
const QString APP_CFG =                             "config.cfg";
const QString FORMAT_VERSION =                      "3";
const QString IMAGE_FILE_FORMAT =                   "PNG";
const QString PRESET_FILE_EXTENSION =               "FPR";
const QString PROJECT_FILE_EXTENSION =              "FPJ";
const QString RULE_FILE_EXTENSION =                 "FRL";
const QString LOG_FILE_EXTENSION =                  "LOG";
const QString HELP_PAGE =                           "qrc:/resources/md/help/doc_en.md";
const QString HELP_PAGE_TRIGGER =                   "language.md";
const QString PRESET_DIRECTORY =                    "presets";
const QString RULES_DIRECTORY =                     "rules";
const QString PROJECTS_DIRECTORY =                  "projects";
const QString LOG_DIRECTORY =                       "logs";
const QString TEXT_CODEC =                          "UTF-8";
const QString FORMAT_DT =                           "yyyy.MM.dd hh:mm:ss";
const QString LOG_FILE_NAME_FORMAT =                "yyyyMMdd_hhmmss";
const QString SCENE_SELECT_COLOR =                  "#5555FF";
const QString SCENE_BG_COLOR =                      "#AAAA00";
const QString SCENE_CELL_DEAD_COLOR =               "#FFFFFF";
const QString SCENE_CELL_ALIVE_COLOR =              "#000000";
const QString SCENE_CELL_TRAIT_COLOR =              "#005500";
const QString SCENE_CELL_CURSE_COLOR =              "#AA0000";
const QString SCENE_VIEW_UPDATE_MODE =              "MINIMAL";
const QString SCENE_FIELD_THREAD_PRIORITY =         "NORMAL";
const QString CELL_ALIVE_AGE_INDIC_BRIGHTNESS =     "LIGHTER";
const QString CELL_CURSE_AGE_INDIC_BRIGHTNESS =     "LIGHTER";
const QString EXTERN_URL_RG = "https?:\\/\\/(www\\.)?[-a-zA-Z0-9@:%._\\+~#=]"
                              "{1,256}\\.[a-zA-Z0-9()]{1,6}\\b([-a-zA-Z0-9()@:%_\\+.~#?&//=]*)";
const qreal SCENE_SCALE_STEP =                      1.0001;
const auto DENSITY_VALUE_PRECISION =                4;
const auto RULE_ACTIVITY_LEN =                      8;
const auto FIELD_RULE_PROPERTY_LENGTH =             50;
const auto SCENE_GRID_LINE_WIDTH =                  0;
const auto SCENE_CELL_SIZE =                        10;
const auto SCENE_MULTISELECT_ALFA =                 50;  // 0-254
const auto SCENE_SELECT_ALFA =                      100; // 0-254
const auto SCENE_VIEW_ANTIALIASING =                true;
const auto SCENE_ZOOM_KEY_MODIFIER =                Qt::ControlModifier;
const auto SCENE_TOOLTIP_KEY_MODIFIER =             Qt::ShiftModifier;
const auto SCENE_MULTISELECT_KEY_MODIFIER =         Qt::ControlModifier;
const auto FIELD_FASTEDIT_KEY_MODIFIER =            Qt::ControlModifier;
const auto SCENE_CALCULATING_MIN_PAUSE =            20;
const auto SCENE_FIELD_SIZE =                       100;
const auto CELL_ALIVE_AGE_INDIC_FACTOR =            700; // 150 - 1000
const auto CELL_ALIVE_AGE_INDIC_DIAPASON =          50;  // 2-50
const auto CELL_CURSE_AGE_INDIC_FACTOR =            700; // 150 - 1000
const auto CELL_CURSE_AGE_INDIC_DIAPASON =          50;  // 2-50
const auto WINDOW_HEIGHT =                          700;
const auto WINDOW_WIDTH =                           900;
const auto SETUP_WINDOW_HEIGHT =                    500;
const auto SETUP_WINDOW_WIDTH =                     400;
const auto EDITACTIVITY_WINDOW_HEIGHT =             500;
const auto EDITACTIVITY_WINDOW_WIDTH =              300;
const auto EDITCELL_WINDOW_HEIGHT =                 350;
const auto EDITCELL_WINDOW_WIDTH =                  250;
const auto PROJECT_WINDOW_HEIGHT =                  400;
const auto PROJECT_WINDOW_WIDTH =                   300;
const auto SNAPSHOT_WINDOW_HEIGHT =                 200;
const auto SNAPSHOT_WINDOW_WIDTH =                  250;
const auto RANDOMFILL_WINDOW_HEIGHT =               200;
const auto RANDOMFILL_WINDOW_WIDTH =                250;
const auto CELL_INFO_WINDOW_HEIGHT =                200;
const auto CELL_INFO_WINDOW_WIDTH =                 200;
const auto CELL_MONITOR_WINDOW_HEIGHT =             200;
const auto CELL_MONITOR_WINDOW_WIDTH =              600;
const auto FIELD_INFO_WINDOW_HEIGHT =               350;
const auto FIELD_INFO_WINDOW_WIDTH =                200;
const auto EDIT_RULES_WINDOW_HEIGHT =               300;
const auto EDIT_RULES_WINDOW_WIDTH =                600;
const auto RULEPROPERTY_WINDOW_HEIGHT =             350;
const auto RULEPROPERTY_WINDOW_WIDTH =              300;
const auto INFO_WINDOW_HEIGHT =                     600;
const auto INFO_WINDOW_WIDTH =                      600;
const auto HELP_WINDOW_HEIGHT =                     600;
const auto HELP_WINDOW_WIDTH =                      800;
const auto INFORULE_WINDOW_HEIGHT =                 300;
const auto INFORULE_WINDOW_WIDTH =                  650;
const auto BUTTON_SIZE =                            24;
const auto SCENE_ITEM_ZVALUE =                      0;
const auto SPLASH_SIZE =                            640;
const auto SPLASH_TIME =                            500;
const auto PROJECT_FILE_COMPRESSION_LEVEL =         -1; // -1 - default, 0-9
const auto FIELD_RANDOMISATION_VALUE =              50; // 1-100
const auto PROJECT_FILE_COMPRESSION =               true;
const auto SCENE_CREATE_DEFAULT_RULE =              true;
const auto SCENE_ALIVE_CELL_AGE_INDICATE =          true;
const auto SCENE_CURSE_CELL_AGE_INDICATE =          true;
const auto WINDOW_SHOW_FIELD_INFO =                 false;
const auto JSON_COMPACT_MODE =                      true;
const auto JSON_IGNORE_DATA_VERSION =               false;
const auto COPY_TO_CLIPBOARD_EXCEPT_DEAD =          true;
const auto SAVE_TO_PRESET_EXCEPT_DEAD =             true;
const auto REWITE_RESOURCES =                       true;
const auto SCENE_FIRST_SNAPSHOT =                   true;
const auto SCENE_FIRST_SNAPSHOT_CLEAR_LIST =        true;
const auto WRITE_LOGS_TO_FILE =                     false;

#endif // PROPERTIES_H
