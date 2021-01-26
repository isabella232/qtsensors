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

#include "sensorfwaccelerometer.h"

char const * const sensorfwaccelerometer::id("sensorfw.accelerometer");

sensorfwaccelerometer::sensorfwaccelerometer(QSensor *sensor)
    : SensorfwSensorBase(sensor),
      m_initDone(false)
{
    init();
    setDescription(QLatin1String("x, y, and z axes accelerations in m/s^2"));
    setRanges(GRAVITY_EARTH_THOUSANDTH);
    setReading<QAccelerometerReading>(&m_reading);
    sensor->setDataRate(50);//set a default rate
}

void sensorfwaccelerometer::slotDataAvailable(const XYZ& data)
{
    // Convert from milli-Gs to meters per second per second
    // Using 1 G = 9.80665 m/s^2
    m_reading.setX(data.x() * GRAVITY_EARTH_THOUSANDTH);
    m_reading.setY(data.y() * GRAVITY_EARTH_THOUSANDTH);
    m_reading.setZ(data.z() * GRAVITY_EARTH_THOUSANDTH);
    m_reading.setTimestamp(data.XYZData().timestamp_);
    newReadingAvailable();
}

void sensorfwaccelerometer::slotFrameAvailable(const QVector<XYZ>&  frame)
{
    for (int i=0, l=frame.size(); i<l; i++) {
        slotDataAvailable(frame.at(i));
    }
}

bool sensorfwaccelerometer::doConnect()
{
    Q_ASSERT(m_sensorInterface);
    if (m_bufferSize==1)
        return QObject::connect(m_sensorInterface, SIGNAL(dataAvailable(XYZ)), this, SLOT(slotDataAvailable(XYZ)));
    return QObject::connect(m_sensorInterface, SIGNAL(frameAvailable(QVector<XYZ>)),this, SLOT(slotFrameAvailable(QVector<XYZ>)));
}


QString sensorfwaccelerometer::sensorName() const
{
    return "accelerometersensor";
}


qreal sensorfwaccelerometer::correctionFactor() const
{
    return GRAVITY_EARTH_THOUSANDTH;
}

void sensorfwaccelerometer::init()
{
    m_initDone = false;
    initSensor<AccelerometerSensorChannelInterface>(m_initDone);
}

void sensorfwaccelerometer::start()
{
    if (reinitIsNeeded)
        init();
    SensorfwSensorBase::start();
}
