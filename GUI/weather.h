#ifndef WEATHER_H
#define WEATHER_H

#include"mainwindow.h"
#include<QDateTime>
#include<QMainWindow>
#include<QObject>
#include<QString>
#include<QWidget>

class WeatherCondition
{
    friend class WeatherDataCollector;
protected:
    QString CurrentDateTime,UpdateDateTime,area,WeatherType,temperature,WindDirection,WindPower,humidity;
public:
    WeatherCondition()=default;
    WeatherCondition(const QString &,const QString &,
                     const QString &,const QString &,const QString &,
                     const QString &,const QString &,const QString &);
    QString ToString() const;
};

class WeatherDataCollector
{
private:
    QString UrlString,ApiString;
    WeatherCondition Condition;
public:
    bool init(QWidget*);
    bool update(QWidget*);
    QString ToString() const;
    QString get_temperature() const;
    QString get_WindDirection() const;
    QString get_WindPower() const;
    QString get_humidity() const;
};

#endif // WEATHER_H
