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
#include<QTimer>

WeatherDataCollector collector;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    if(!collector.init(this))
    {
        QTimer::singleShot(0,qApp,SLOT(quit()));
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_UpdateWeather_clicked()
{
    if(collector.update(this))
    {
        ui->WeatherInformationText->setText(collector.ToString());
    }
}
