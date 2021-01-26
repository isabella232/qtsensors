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

#include "winrtaccelerometer.h"
#include "winrtcompass.h"
#include "winrtgyroscope.h"
#include "winrtrotationsensor.h"
#include "winrtambientlightsensor.h"
#include "winrtorientationsensor.h"
#include <QtSensors/QAccelerometer>
#include <QtSensors/QAmbientLightSensor>
#include <QtSensors/QCompass>
#include <QtSensors/QGyroscope>
#include <QtSensors/QOrientationSensor>
#include <QtSensors/QRotationSensor>
#include <QtSensors/QSensorBackendFactory>
#include <QtSensors/QSensorManager>
#include <QtSensors/QSensorPluginInterface>

class WinRtSensorPlugin : public QObject, public QSensorPluginInterface, public QSensorBackendFactory
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.qt-project.Qt.QSensorPluginInterface/1.0" FILE "plugin.json")
    Q_INTERFACES(QSensorPluginInterface)
public:
    void registerSensors() override
    {
        QSensorManager::registerBackend(QAccelerometer::type, QByteArrayLiteral("WinRtAccelerometer"), this);
        QSensorManager::registerBackend(QCompass::type, QByteArrayLiteral("WinRtCompass"), this);
        QSensorManager::registerBackend(QGyroscope::type, QByteArrayLiteral("WinRtGyroscope"), this);
        QSensorManager::registerBackend(QRotationSensor::type, QByteArrayLiteral("WinRtRotationSensor"), this);
        QSensorManager::registerBackend(QAmbientLightSensor::type, QByteArrayLiteral("WinRtAmbientLightSensor"), this);
        QSensorManager::registerBackend(QOrientationSensor::type, QByteArrayLiteral("WinRtOrientationSensor"), this);
    }

    QSensorBackend *createBackend(QSensor *sensor) override
    {
        if (sensor->identifier() == QByteArrayLiteral("WinRtAccelerometer"))
            return new WinRtAccelerometer(sensor);

        if (sensor->identifier() == QByteArrayLiteral("WinRtCompass"))
            return new WinRtCompass(sensor);

        if (sensor->identifier() == QByteArrayLiteral("WinRtGyroscope"))
            return new WinRtGyroscope(sensor);

        if (sensor->identifier() == QByteArrayLiteral("WinRtRotationSensor"))
            return new WinRtRotationSensor(sensor);

        if (sensor->identifier() == QByteArrayLiteral("WinRtAmbientLightSensor"))
            return new WinRtAmbientLightSensor(sensor);

        if (sensor->identifier() == QByteArrayLiteral("WinRtOrientationSensor"))
            return new WinRtOrientationSensor(sensor);

        return 0;
    }
};

#include "main.moc"

