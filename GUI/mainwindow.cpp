#include"mainwindow.h"
#include"ui_mainwindow.h"
#include"weather.h"
#include"correction.h"
#include<QString>
#include<QJsonDocument>
#include<QJsonParseError>
#include<QFile>
#include<QJsonObject>
#include<QDebug>
#include<QJsonArray>
#include<QDateTime>

Weather *weather;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    weather=new Weather(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_UpdateWeather_clicked()
{
    QByteArray data=weather->GetInfo(this);
    QJsonParseError JsonError;
    QJsonDocument JsonDoc(QJsonDocument::fromJson(data,&JsonError));
    if(JsonError.error!=QJsonParseError::NoError)
    {
        Correction::WindowsPrint(this,"Can't analyse the data collected.");
        return;
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
    ui->WeatherInformationText->setText(str);
}
