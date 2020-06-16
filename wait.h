#ifndef WAIT_H
#define WAIT_H

#include <QTime>
#include <QCoreApplication>

/*!
 * \brief delay - пауза в выполнении приложения. Для тестов.
 */
void delay(int millisecondsToWait)
{
    QTime dieTime = QTime::currentTime().addMSecs(millisecondsToWait);
    while(QTime::currentTime() < dieTime)
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
}

#endif // WAIT_H
