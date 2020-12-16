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


WeatherCondition::WeatherCondition(const QString &CurrentDateTime,const QString &UpdateDateTime,
                                   const QString &area,const QString &WeatherType,const QString &temperature,
                                   const QString &WindDirection,const QString &WindPower,const QString &humidity)
{
    this->CurrentDateTime=CurrentDateTime;
    this->UpdateDateTime=UpdateDateTime;
    this->area=area;
    this->WeatherType=WeatherType;
    this->temperature=temperature;
    this->WindDirection=WindDirection;
    this->WindPower=WindPower;
    this->humidity=humidity;
}

QString WeatherCondition::ToString() const
{
    QString str="";
    str.append("获取时间："+this->CurrentDateTime+"\n");
    str.append("更新时间："+this->UpdateDateTime+"\n");
    str.append("地区："+this->area+"\n");
    str.append("天气："+this->WeatherType+"\n");
    str.append("气温："+this->temperature+"\n");
    str.append("风向："+this->WindDirection+"\n");
    str.append("风速："+this->WindPower+"\n");
    str.append("湿度："+this->humidity);
    return str;
}

bool WeatherDataCollector::init(QWidget *parent)
{
    QFile LoadConfig("config.json");
    if(!LoadConfig.open(QIODevice::ReadOnly))
    {
        Correction::WindowsPrint(parent,"Could't open Config json");
        return false;
    }
    QByteArray Data = LoadConfig.readAll();
    LoadConfig.close();
    QJsonParseError JsonError;
    QJsonDocument JsonDoc(QJsonDocument::fromJson(Data,&JsonError));
    if(JsonError.error!=QJsonParseError::NoError)
    {
        Correction::WindowsPrint(parent,"Json error!");
        return false;
    }
    QJsonObject JsonObj=JsonDoc.object();
    UrlString=JsonObj["url"].toString();
    ApiString=JsonObj["APIKey"].toString();
    return true;
}

bool WeatherDataCollector::update(QWidget *parent)
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
        return false;
    }
    QJsonParseError JsonError;
    QJsonDocument JsonDoc(QJsonDocument::fromJson(ResponseData,&JsonError));
    if(JsonError.error!=QJsonParseError::NoError)
    {
        Correction::WindowsPrint(parent,"Can't analyse the data collected.");
        return false;
    }
    QJsonObject JsonObj=JsonDoc.object();
    QJsonArray arrayValue=JsonObj.value(QStringLiteral("lives")).toArray();
    QJsonObject WeatherCollected=arrayValue.at(0).toObject();
    this->Condition=WeatherCondition(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"),
                                     WeatherCollected["reporttime"].toString(),
                                     WeatherCollected["province"].toString()+WeatherCollected["city"].toString(),
                                     WeatherCollected["weather"].toString(),WeatherCollected["temperature"].toString(),
                                     WeatherCollected["winddirection"].toString(),WeatherCollected["windpower"].toString(),
                                     WeatherCollected["humidity"].toString());
    return true;
}

QString WeatherDataCollector::ToString() const
{
    return this->Condition.ToString();
}
