#ifndef ASSEMBLE_H
#define ASSEMBLE_H

#include <QMainWindow>
#include <QString>
#include <QStringList>
#include <QMap>
#include <QDebug>
#include <QFile>
#include <QDataStream>
#include <QByteArray>
#include <QMessageBox>
#include <QTextStream>


class Assemble : public QMainWindow
{
    Q_OBJECT
public:
    explicit Assemble(QWidget *parent = 0);
    ~Assemble();
    void Read(const QString &content);
    void inst_handle();
    void save_bi(const QString &name);
    void save_coe(const QString &name);
    QStringList get_reglist() const;
    QStringList get_rtype() const;
    QStringList get_itype() const;
    QStringList get_jtype() const;

private:
    QStringList Reglist;
    QStringList Rtypelist;
    QStringList Itypelist;
    QStringList Jtypelist;
    QStringList code;
    QString bi_output;
    QString coe_output;
    QMap <QString, int> map;
    enum Register
    {
        zero,
        at,
        v0, v1,
        a0, a1, a2, a3,
        t0, t1, t2, t3, t4, t5, t6, t7,
        s0, s1, s2, s3, s4, s5, s6, s7,
        t8, t9,
        k0, k1,
        gp, sp, fp, ra
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
        SLTU,
        JR,
        JALR
    };
    enum Itype
    {
        ADDI,
        ADDIU,
        SLTI,
        SLTIU,
        ANDI,
        ORI,
        XORI,
        LUI,
        LW,
        LB,
        LBU,
        LH,
        LHU,
        SW,
        SH,
        SB,
        BEQ,
        BNE,
        BLEZ,
        BGTZ,
        BLTZ,
        BGEZ
    };

    enum Jtype
    {
        J,
        Jal
    };

    void Rtype_inst(const QStringList &inst);
    void Itype_inst(const QStringList &inst, const int &inst_num);
    void Jtype_inst(const QStringList &inst);
    void complete_inst(QString &inst, const int &num, const int &func_num, const int &base,
                       const QString &fill = "0", const int &isfront = 1);
    int get_reg(const QString &reg) const;
    int get_inst(const QString &inst, const QString &type) const;
    void tocoe(QString &s) const;

};

#endif // ASSEMBLE_H
