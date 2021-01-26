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

#include <qsensorplugin.h>
#include <qsensorbackend.h>
#include <qsensormanager.h>

#include "iosmotionmanager.h"
#include "iosaccelerometer.h"
#include "iosgyroscope.h"
#include "iosmagnetometer.h"
#include "ioscompass.h"
#include "iosproximitysensor.h"

#import <CoreLocation/CoreLocation.h>
#ifdef HAVE_COREMOTION
#import <CoreMotion/CoreMotion.h>
#endif

class IOSSensorPlugin : public QObject, public QSensorPluginInterface, public QSensorBackendFactory
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.qt-project.Qt.QSensorPluginInterface/1.0" FILE "plugin.json")
    Q_INTERFACES(QSensorPluginInterface)
public:
    void registerSensors() override
    {
#ifdef HAVE_COREMOTION
        QSensorManager::registerBackend(QAccelerometer::type, IOSAccelerometer::id, this);
        if ([QIOSMotionManager sharedManager].gyroAvailable)
            QSensorManager::registerBackend(QGyroscope::type, IOSGyroscope::id, this);
        if ([QIOSMotionManager sharedManager].magnetometerAvailable)
            QSensorManager::registerBackend(QMagnetometer::type, IOSMagnetometer::id, this);
#endif
#ifdef HAVE_COMPASS
        if ([CLLocationManager headingAvailable])
            QSensorManager::registerBackend(QCompass::type, IOSCompass::id, this);
#endif
#ifdef HAVE_UIDEVICE
        if (IOSProximitySensor::available())
            QSensorManager::registerBackend(QProximitySensor::type, IOSProximitySensor::id, this);
#endif
    }

    QSensorBackend *createBackend(QSensor *sensor) override
    {
#ifdef HAVE_COREMOTION
        if (sensor->identifier() == IOSAccelerometer::id)
            return new IOSAccelerometer(sensor);
        if (sensor->identifier() == IOSGyroscope::id)
            return new IOSGyroscope(sensor);
        if (sensor->identifier() == IOSMagnetometer::id)
            return new IOSMagnetometer(sensor);
#endif
#ifdef HAVE_COMPASS
        if (sensor->identifier() == IOSCompass::id)
            return new IOSCompass(sensor);
#endif
#ifdef HAVE_UIDEVICE
        if (sensor->identifier() == IOSProximitySensor::id)
            return new IOSProximitySensor(sensor);
#endif
        return 0;
    }
};

#include "main.moc"

