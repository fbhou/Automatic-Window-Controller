#ifndef WEATHER_H
#define WEATHER_H

#include"mainwindow.h"
#include<QMainWindow>
#include<QObject>
#include<QString>
#include<QWidget>

class Weather
{
public:
    Weather(QWidget*);
    QByteArray GetInfo(QWidget*);
    QString UpdateWeather(QWidget*);
private:
    QString UrlString,ApiString;
};

#endif // WEATHER_H
