/****************************************************************************
**
** Copyright (C) 2017 Lorn Potter
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

#ifndef SENSORTAGGYROSCOPE_H
#define SENSORTAGGYROSCOPE_H

#include "sensortagbase.h"
#include <QtSensors/qgyroscope.h>

class SensorTagGyroscope : public SensorTagBase
{
    Q_OBJECT

public:
    static char const * const id;
    SensorTagGyroscope(QSensor *sensor);
protected:
    void start() override;
    void stop() override;
    virtual void init();

private:
    QGyroscopeReading m_reading;
    bool m_initDone = false;
    static const float MILLI;
private slots:
    void slotDataAvailable(const QGyroscopeReading &data);
};

#endif // SENSORTAGGYROSCOPE_H
