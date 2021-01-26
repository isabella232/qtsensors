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

#include "simulatoraccelerometer.h"
#include <QDebug>
#include <QtGlobal>

const char *SimulatorAccelerometer::id("Simulator.Accelerometer");

SimulatorAccelerometer::SimulatorAccelerometer(QSensor *sensor)
    : SimulatorCommon(sensor)
{
    setReading<QAccelerometerReading>(&m_reading);
}

void SimulatorAccelerometer::poll()
{
    QtMobility::QAccelerometerReadingData data = SensorsConnection::instance()->qtAccelerometerData;
    quint64 newTimestamp;
    if (!data.timestamp.isValid())
        newTimestamp = static_cast<quint64>(QDateTime::currentMSecsSinceEpoch()) / 1000;
    else
        newTimestamp = static_cast<quint64>(data.timestamp.toSecsSinceEpoch());
    if (m_reading.x() != data.x
        || m_reading.y() != data.y
        || m_reading.z() != data.z) {
            m_reading.setTimestamp(newTimestamp);
            m_reading.setX(data.x);
            m_reading.setY(data.y);
            m_reading.setZ(data.z);

            newReadingAvailable();
    }
}

