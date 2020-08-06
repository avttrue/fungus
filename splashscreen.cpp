#include "splashscreen.h"
#include "properties.h"

#include <QIcon>

SplashScreen::SplashScreen()
    : QSplashScreen()
{
    setPixmap(QPixmap(":/resources/img/mushrooms.svg").
                     scaled(config->SplashSize(), config->SplashSize(),
                            Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    setWindowIcon(QIcon(":/resources/img/mushroom.svg"));
    setWindowModality(Qt::WindowModal);
    setWindowState(Qt::WindowActive);
    setWindowFlag(Qt::WindowStaysOnTopHint);
}
