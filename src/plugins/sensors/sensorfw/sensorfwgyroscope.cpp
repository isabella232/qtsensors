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

#include "sensorfwgyroscope.h"

char const * const SensorfwGyroscope::id("sensorfw.gyroscope");
const float SensorfwGyroscope::MILLI = 0.001;

SensorfwGyroscope::SensorfwGyroscope(QSensor *sensor)
    : SensorfwSensorBase(sensor)
    , m_initDone(false)
{
    init();
    setDescription(QLatin1String("angular velocities around x, y, and z axis in degrees per second"));
    setRanges(MILLI);
    setReading<QGyroscopeReading>(&m_reading);
    addDataRate(10, 10);
    addDataRate(50, 50);
    sensor->setDataRate(50);//set a default rate
}

void SensorfwGyroscope::slotDataAvailable(const XYZ& data)
{
    m_reading.setX((qreal)(data.x()*MILLI));
    m_reading.setY((qreal)(data.y()*MILLI));
    m_reading.setZ((qreal)(data.z()*MILLI));
    m_reading.setTimestamp(data.XYZData().timestamp_);
    newReadingAvailable();
}

void SensorfwGyroscope::slotFrameAvailable(const QVector<XYZ>&  frame)
{
    for (int i=0, l=frame.size(); i<l; i++) {
        slotDataAvailable(frame.at(i));
    }
}

bool SensorfwGyroscope::doConnect()
{
    Q_ASSERT(m_sensorInterface);
    if (m_bufferSize==1)
        return QObject::connect(m_sensorInterface, SIGNAL(dataAvailable(XYZ)), this, SLOT(slotDataAvailable(XYZ)));
    return QObject::connect(m_sensorInterface, SIGNAL(frameAvailable(QVector<XYZ>)),this, SLOT(slotFrameAvailable(QVector<XYZ>)));
}

QString SensorfwGyroscope::sensorName() const
{
    return "gyroscopesensor";
}

qreal SensorfwGyroscope::correctionFactor() const
{
    return MILLI;
}

void SensorfwGyroscope::init()
{
    m_initDone = false;
    initSensor<GyroscopeSensorChannelInterface>(m_initDone);
}

void SensorfwGyroscope::start()
{
    if (reinitIsNeeded)
        init();
    SensorfwSensorBase::start();
}
