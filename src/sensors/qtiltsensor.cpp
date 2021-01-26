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

#include "qtiltsensor.h"
#include "qtiltsensor_p.h"

#include "qsensorbackend.h"

#include <QtCore/QMetaObject>

QT_BEGIN_NAMESPACE

IMPLEMENT_READING(QTiltReading)

/*!
    \class QTiltReading
    \ingroup sensors_reading
    \inmodule QtSensors
    \since 5.1

    \brief The QTiltReading class holds readings from the tilt sensor.

    The tilt sensor reports the angle of tilt in degrees of the device along the X and Y plane.

*/


/*!
    \property QTiltReading::yRotation
    \brief  This property holds the amount of tilt on the Y axis.
*/
qreal QTiltReading::yRotation() const
{
    return d->yRotation;
}

/*!
    Sets yRotation to \a y.
*/
void QTiltReading::setYRotation(qreal y)
{
    d->yRotation = y;
}

/*!
    \property QTiltReading::xRotation
    \brief  This property holds the amount of tilt on the X axis.

*/
qreal QTiltReading::xRotation() const
{
    return d->xRotation;
}

/*!
    Sets xRotation to \a x.
*/
void QTiltReading::setXRotation(qreal x)
{
    d->xRotation = x;
}

// =====================================================================

/*!
    \class QTiltFilter
    \ingroup sensors_filter
    \inmodule QtSensors
    \since 5.1

    \brief The QTiltFilter class is a convenience wrapper around QSensorFilter.

    The only difference is that the filter() method features a pointer to QTiltReading
    instead of QSensorReading.
*/

/*!
    \fn QTiltFilter::filter(QTiltReading *reading)

    Called when \a reading changes. Returns false to prevent the reading from propagating.

    \sa QSensorFilter::filter()
*/

bool QTiltFilter::filter(QSensorReading *reading)
{
    return filter(static_cast<QTiltReading*>(reading));
}

char const * const QTiltSensor::type("QTiltSensor");

/*!
    \class QTiltSensor
    \ingroup sensors_type
    \inmodule QtSensors
    \since 5.1

    \brief The QTiltSensor class is a convenience wrapper around QSensor.

    The only behavioural difference is that this class sets the type properly.QMetaObject::invokeMethod(backend(), "calibrate");

    This class also features a reading() function that returns a QTiltReading instead of a QSensorReading.

    For details about how the sensor works, see \l QTiltReading.

    \sa QTiltReading
*/

/*!
    \fn QTiltSensor::QTiltSensor(QObject *parent)

    Construct the sensor as a child of \a parent.
*/
QTiltSensor::QTiltSensor(QObject *parent)
    : QSensor(QTiltSensor::type, parent)
{
}

/*!
    \fn QTiltSensor::~QTiltSensor()

    Destroy the sensor. Stops the sensor if it has not already been stopped.
*/
QTiltSensor::~QTiltSensor()
{
}
/*!
    \fn QTiltSensor::reading() const

    Returns the reading class for this sensor.

    \sa QSensor::reading()
*/

QTiltReading *QTiltSensor::reading() const
{
    return static_cast<QTiltReading*>(QSensor::reading());
}

/*!
   Calibrates the tilt sensor. Uses the current tilt angles as 0.
  */
void QTiltSensor::calibrate()
{
     QMetaObject::invokeMethod(backend(), "calibrate", Qt::DirectConnection);
}

QT_END_NAMESPACE

#include "moc_qtiltsensor.cpp"
