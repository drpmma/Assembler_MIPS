#include "disassemble.h"

Disassemble::Disassemble(QWidget *parent) : QMainWindow(parent)
{
}

void Disassemble::Read_coe(const QString &content)
{
    QString temp = content.simplified();
    temp.replace(" ", "");
    temp.replace(";", "");
    const QString coe_1 = tr("memory_initialization_radix");
    const QString coe_2 = tr("memory_initialization_vector");
    vector = temp.split(QRegExp(",|="));
    int num;
    if(vector[1][0] == '2')
        num = 2;
    else if(vector[1][0] == '8')
        num = 8;
    else if(vector[1].mid(0, 2) == "16")
        num = 16;
    qDebug() << num;
    qDebug() << vector[2].toInt(nullptr, 16);
    vector[1].replace(QRegExp("[0-9]"), "");

    qDebug() << vector;
    if(vector[0] == coe_1 && vector[1] == coe_2)
    {
        qDebug() << vector;
    }
    else
    {
        Error(tr("coe file is wrong"));
    }

}

void Disassemble::vector_handle()
{
    for(auto i = vector.begin(); i != vector.end(); ++i)
    {

    }
}

void Disassemble::Error(const QString &err_mess)
{
    QMessageBox::information(this, "ERROR", err_mess);
}
