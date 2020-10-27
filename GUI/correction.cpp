#include"correction.h"
#include<QString>
#include<QMessageBox>

void Correction::WindowsPrint(QString str)
{
    QMessageBox::information(NULL,"Test",str);
}
