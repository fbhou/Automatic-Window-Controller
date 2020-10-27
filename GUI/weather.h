#ifndef WEATHER_H
#define WEATHER_H

#include<QMainWindow>
#include<QObject>
#include<QString>

class Weather
{
public:
    Weather();
    QByteArray GetInfo();
private:
    QString UrlString,ApiString;
};

#endif // WEATHER_H
