/****************************************************************************
**
** Copyright (C) 2021 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtSensors module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:COMM$
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** $QT_END_LICENSE$
**
**
**
**
**
**
**
**
**
**
**
**
**
**
**
**
**
**
**
****************************************************************************/

#include <QDebug>
#include <QTimer>

#include "qshakerecognizer.h"

QShakeSensorGestureRecognizer::QShakeSensorGestureRecognizer(QObject *parent)
    : QSensorGestureRecognizer(parent)
    , timerTimeout(450)
    , active(0)
    , shaking(0)
    , shakeCount(0)
{
}

QShakeSensorGestureRecognizer::~QShakeSensorGestureRecognizer()
{
}

void QShakeSensorGestureRecognizer::create()
{
    accel = new QAccelerometer(this);
    accel->connectToBackend();
    accel->setDataRate(50);

    qoutputrangelist outputranges = accel->outputRanges();

    if (outputranges.count() > 0)
        accelRange = (int)(outputranges.at(0).maximum *2) / 9.8; //approx range in g's
    else
        accelRange = 4; //this should never happen

    connect(accel,SIGNAL(readingChanged()),this,SLOT(accelChanged()));
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(timeout()));
    timer->setSingleShot(true);
    timer->setInterval(timerTimeout);
}

bool QShakeSensorGestureRecognizer::start()
{
    active = accel->start();
    return active;
}

bool QShakeSensorGestureRecognizer::stop()
{
    accel->stop();
    active = accel->isActive();
    return !active;
}

bool QShakeSensorGestureRecognizer::isActive()
{
    return active;
}

QString QShakeSensorGestureRecognizer::id() const
{
    return QString("QtSensors.shake");
}

#define NUMBER_SHAKES 3
#define THRESHOLD 25

void QShakeSensorGestureRecognizer::accelChanged()
{
    qreal x = accel->reading()->x();
    qreal y = accel->reading()->y();
    qreal z = accel->reading()->z();

    currentData.x = x;
    currentData.y = y;
    currentData.z = z;

    if (qAbs(prevData.x - currentData.x)  < 1
            && qAbs(prevData.y - currentData.y)  < 1
            && qAbs(prevData.z - currentData.z)  < 1) {
        prevData.x = currentData.x;
        prevData.y = currentData.y;
        prevData.z = currentData.z;
        return;
    }

    bool wasShake = checkForShake(prevData, currentData, THRESHOLD);
    if (!shaking && wasShake &&
        shakeCount >= NUMBER_SHAKES) {
        shaking = true;
        shakeCount = 0;
        Q_EMIT shake();
        Q_EMIT detected("shake");

    } else if (wasShake) {

        shakeCount++;
        if (shakeCount > NUMBER_SHAKES) {
            timer->start();
        }
    }

    prevData.x = currentData.x;
    prevData.y = currentData.y;
    prevData.z = currentData.z;
}

void QShakeSensorGestureRecognizer::timeout()
{
    shakeCount = 0;
    shaking = false;
}

bool QShakeSensorGestureRecognizer::checkForShake(AccelData prevSensorData, AccelData currentSensorData, qreal threshold)
{
    double deltaX = qAbs(prevSensorData.x - currentSensorData.x);
    double deltaY = qAbs(prevSensorData.y - currentSensorData.y);
    double deltaZ = qAbs(prevSensorData.z - currentSensorData.z);

    return (deltaX > threshold
            || deltaY > threshold
            || deltaZ > threshold);
}

