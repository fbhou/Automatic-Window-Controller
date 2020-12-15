#include"correction.h"
#include"mainwindow.h"
#include"ui_mainwindow.h"
#include"weather.h"
#include<QDateTime>
#include<QFile>
#include<QJsonArray>
#include<QJsonDocument>
#include<QJsonObject>
#include<QJsonParseError>
#include<QString>

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
    QString str=weather->UpdateWeather(this);
    ui->WeatherInformationText->setText(str);
}
