#include"weather.h"
#include"correction.h"
#include<QtNetwork>
#include<QJsonDocument>
#include<QJsonParseError>
#include<QFile>
#include<QJsonObject>
#include<QDebug>
#include<QJsonArray>
#include<QtCore>
#include<QMessageBox>
#include<QString>

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
