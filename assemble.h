#ifndef ASSEMBLE_H
#define ASSEMBLE_H

#include <QString>
#include <QStringList>
#include <QMap>
#include <QDebug>

QStringList Rtypelist;
QStringList Reglist;

class Assemble
{
public:
    Assemble();
    void Read(const QString &content);
    void inst_handle();
private:
    QStringList code;
    QMap <QString, int> map;
    enum Register
    {
        zero,
        v0 = 2, v1,
        a0, a1, a2, a3,
        t0, t1, t2, t3, t4, t5, t6, t7,
        s0, s1, s2, s3, s4, s5, s6, s7,
        t8, t9,
        gp = 28, sp, fp, ra
    };
    enum Rtype
    {
        ADD,
        ADDU,
        SUB,
        SUBU,
        AND,
        OR,
        XOR,
        NOR,
        SLL,
        SRL,
        SRA,
        SLT,
        SLTU
    };
    void Rtype_inst(const QStringList &inst);
    void complete_inst(QString &inst, const int &num, int func_num, int base);
};

#endif // ASSEMBLE_H
