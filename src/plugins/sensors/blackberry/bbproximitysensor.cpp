/****************************************************************************
**
** Copyright (C) 2012 Research In Motion
** Contact: http://www.qt-project.org/
**
** This file is part of the QtSensors module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this
** file. Please review the following information to ensure the GNU Lesser
** General Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU General
** Public License version 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of this
** file. Please review the following information to ensure the GNU General
** Public License version 3.0 requirements will be met:
** http://www.gnu.org/copyleft/gpl.html.
**
** Other Usage
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/
#include "bbproximitysensor.h"

class BbProximityReadingPrivate
{
public:
    BbProximityReadingPrivate()
        : distance(0),
          reflectance(0)
    {
    }

    qreal distance;
    qreal reflectance;
};

BbProximityReading::BbProximityReading(QObject *parent)
    : QProximityReading(parent),
      d(new BbProximityReadingPrivate)
{
}

BbProximityReading::~BbProximityReading()
{
}

void BbProximityReading::copyValuesFrom(QSensorReading *other)
{
    QProximityReading::copyValuesFrom(other);
    const BbProximityReading * const reading = qobject_cast<const BbProximityReading *>(other);
    if (reading) {
        d->distance = reading->distance();
        d->reflectance = reading->reflectance();
    }
}

qreal BbProximityReading::distance() const
{
    return d->distance;
}

void BbProximityReading::setDistance(qreal distance)
{
    d->distance = distance;
}

qreal BbProximityReading::reflectance() const
{
    return d->reflectance;
}

void BbProximityReading::setReflectance(qreal reflectance)
{
    d->reflectance = reflectance;
}

BbProximitySensor::BbProximitySensor(QSensor *sensor)
    : BbSensorBackend<BbProximityReading>(devicePath(), SENSOR_TYPE_PROXIMITY, sensor)
{
    setDescription(QLatin1String("Proximity"));
}

QString BbProximitySensor::devicePath()
{
    return QLatin1String("/dev/sensor/prox");
}

bool BbProximitySensor::updateReadingFromEvent(const sensor_event_t &event, BbProximityReading *reading)
{
    // TODO: I was unable to test this since the device I was testing this with did not have
    //       a proximity sensor. Verify that this works, check that the units are correct
    //       and that the threshold makes sense.
    const qreal minProximity = sensor()->outputRanges().first().minimum;
    reading->setClose(event.proximity_s.distance <= minProximity);
    reading->setDistance(event.proximity_s.distance);
    reading->setReflectance(1.0f - event.proximity_s.normalized);
    return true;
}