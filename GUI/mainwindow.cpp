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
#include<QProcess>
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
    startTimer(10000);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::timerEvent(QTimerEvent *event)
{
    if(collector.update(this))
    {
        ui->WeatherInformationText->setText(collector.ToString());
        QProcess FCNN(this);
        QString program="/FCNN/FCNN.exe";
        QStringList arg;
        arg.append("w");
        arg.append(collector.get_temperature());
        arg.append(collector.get_WindPower());
        arg.append(collector.get_WindDirection());
        arg.append(collector.get_humidity());
        FCNN.start(program,arg);
        int tmp=FCNN.exitCode();
        if(tmp==0)
        {
            QProcess::startDetached("/Arduino/close.bat",QStringList());
        }
        else if(tmp==1)
        {
            QProcess::startDetached("/Arduino/open.bat",QStringList());
        }
        else
        {
            Correction::WindowsPrint(this,"Unexcepted Return Code of FCNN.exe: "+QString::number(tmp,16));
        }
    }
}

void MainWindow::on_UpdateWeather_clicked()
{
    if(collector.update(this))
    {
        ui->WeatherInformationText->setText(collector.ToString());
    }
}
