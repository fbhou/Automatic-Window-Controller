#include"correction.h"
#include<QString>
#include<QMessageBox>
#include<QWidget>

void Correction::WindowsPrint(QWidget *parent,QString str)
{
    QMessageBox::warning(parent,"Test",str);
}
