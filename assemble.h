#ifndef ASSEMBLE_H
#define ASSEMBLE_H

#include <QString>
#include <QStringList>
#include <QMap>
#include <QDebug>

class Assemble
{
public:
    Assemble();
    void Read(const QString &content);

private:
    QStringList code;
    QMap <QString, int> map;
};

#endif // ASSEMBLE_H
