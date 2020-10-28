#include"correction.h"
#include<QMessageBox>
#include<QString>
#include<QWidget>

void Correction::WindowsPrint(QWidget *parent,QString str)
{
    QMessageBox::warning(parent,"Test",str);
}
