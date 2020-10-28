#include"correction.h"
#include"weather.h"
#include<QtCore>
#include<QFile>
#include<QJsonArray>
#include<QJsonDocument>
#include<QJsonObject>
#include<QJsonParseError>
#include<QMessageBox>
#include<QString>
#include<QtNetwork>

Weather::Weather(QWidget *parent)
{
    QFile LoadConfig("config.json");
    if(!LoadConfig.open(QIODevice::ReadOnly))
    {
        Correction::WindowsPrint(parent,"Could't open Config json");
        return;
    }
    QByteArray Data = LoadConfig.readAll();
    LoadConfig.close();
    QJsonParseError JsonError;
    QJsonDocument JsonDoc(QJsonDocument::fromJson(Data,&JsonError));
    if(JsonError.error!=QJsonParseError::NoError)
    {
        Correction::WindowsPrint(parent,"Json error!");
        return;
    }
    QJsonObject JsonObj=JsonDoc.object();
    UrlString=JsonObj["url"].toString();
    ApiString=JsonObj["APIKey"].toString();
}

QByteArray Weather::GetInfo(QWidget *parent)
{
    QNetworkAccessManager *manager=new QNetworkAccessManager();
    QNetworkReply *reply=manager->get(QNetworkRequest(QUrl(UrlString+ApiString)));
    QByteArray ResponseData;
    QEventLoop EventLoop;
    QObject::connect(manager,SIGNAL(finished(QNetworkReply*)),&EventLoop,SLOT(quit()));
    EventLoop.exec();
    if(reply->error()==QNetworkReply::NoError)
    {
        ResponseData=reply->readAll();
    }
    else
    {
        Correction::WindowsPrint(parent,reply->errorString());
    }
    return ResponseData;
}

QString Weather::UpdateWeather(QWidget *parent)
{
    QByteArray data=this->GetInfo(parent);
    QJsonParseError JsonError;
    QJsonDocument JsonDoc(QJsonDocument::fromJson(data,&JsonError));
    if(JsonError.error!=QJsonParseError::NoError)
    {
        Correction::WindowsPrint(parent,"Can't analyse the data collected.");
        return "";
    }
    QJsonObject JsonObj=JsonDoc.object();
    QJsonArray arrayValue=JsonObj.value(QStringLiteral("lives")).toArray();
    QJsonObject WeatherCondition=arrayValue.at(0).toObject();
    QString str="";
    str.append("获取时间："+QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")+"\n");
    str.append("更新时间："+WeatherCondition["reporttime"].toString()+"\n");
    str.append("地区："+WeatherCondition["province"].toString()+
               WeatherCondition["city"].toString()+"\n");
    str.append("天气："+WeatherCondition["weather"].toString()+"\n");
    str.append("气温："+WeatherCondition["temperature"].toString()+"\n");
    str.append("风向："+WeatherCondition["winddirection"].toString()+"\n");
    str.append("风速："+WeatherCondition["windpower"].toString()+"\n");
    str.append("湿度："+WeatherCondition["humidity"].toString()+"\n");
    return str;
}
