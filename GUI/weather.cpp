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

Weather::Weather()
{
    QFile LoadConfig("config.json");
    if(!LoadConfig.open(QIODevice::ReadOnly))
    {
        qDebug()<<"Could't open Config json\n";
        return;
    }
    QByteArray Data = LoadConfig.readAll();
    LoadConfig.close();
    QJsonParseError JsonError;
    QJsonDocument JsonDoc(QJsonDocument::fromJson(Data,&JsonError));
    if(JsonError.error!=QJsonParseError::NoError)
    {
        qDebug() << "Json error!\n";
        return;
    }
    QJsonObject JsonObj=JsonDoc.object();
    UrlString=JsonObj["url"].toString();
    ApiString=JsonObj["APIKey"].toString();
}

QByteArray Weather::GetInfo()
{
    QNetworkAccessManager *manager=new QNetworkAccessManager();
    QNetworkReply *reply=manager->get(QNetworkRequest(QUrl(UrlString)));
    QString data;
    QEventLoop loop;
    //connect(manager,SIGNAL(finished(QNetworkReply*)),&loop,SLOT(quit()));
    loop.exec();
    return reply->readAll();
}
