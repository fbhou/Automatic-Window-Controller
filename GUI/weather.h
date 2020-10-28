#ifndef WEATHER_H
#define WEATHER_H

#include<QMainWindow>
#include<QObject>
#include<QString>
#include<QWidget>

class Weather
{
public:
    Weather(QWidget*);
    QByteArray GetInfo(QWidget*);
private:
    QString UrlString,ApiString;
};

#endif // WEATHER_H
