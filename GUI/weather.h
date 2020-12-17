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
private:
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
};

#endif // WEATHER_H
