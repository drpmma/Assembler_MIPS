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
    vector[1].replace(QRegExp("[0-9]"), "");

    if(vector[0] == coe_1 && vector[1] == coe_2)
    {
        vector_handle(num);
    }
    else
    {
        Error(tr("coe file is wrong"));
    }

}

void Disassemble::vector_handle(const int &num)
{
    vector.removeAt(0);
    vector.removeAt(0);
    for(auto i = vector.begin(); i != vector.end(); ++i)
    {
        long long int func_num;
        if(num == 16)
        {
            func_num = (*i).toLongLong(nullptr, 16);
            complete_inst(*i, 32, func_num, 2);
        }
        else if(num == 8)
        {
            func_num = (*i).toInt(nullptr, 8);
            complete_inst(*i, 32, func_num, 2);
        }
        else if(num == 2)
        {
        }
        else
            Error("coe file is wrong");

        QString opcode;
        opcode = i->mid(0, 6);
        int num = opcode.toInt(nullptr, 2);
        if(num == 0)
            Rtype(*i);
        else if((num >= 4 && num <= 15) || num == 1 || (num >= 32 && num <= 33)
                || (num >= 35 && num <= 37) || (num >= 40 && num <= 41) || num == 43)
            Itype(*i);
        else if(num == 2 || num == 3)
            Jtype(*i);
    }
}

void Disassemble::Rtype(const QString &inst)
{
    QString rs = inst.mid(6, 5);
    QString rt = inst.mid(11, 5);
    QString rd = inst.mid(16, 5);
    QString shamt = inst.mid(21, 5);
    QString func = inst.mid(26, 6);
    QString rs_a = "";
    QString rt_a = "";
    QString rd_a = "";

    int func_num = func.toInt(nullptr, 2);
    QString shamt_a = "";
    QString func_a = get_func(func_num, "R");
    QString asb_code;
    if(func_a == "jr")
    {
        rs_a = get_reg(rs) + tr(";\n");
        rs_a.push_front(tr("$"));
    }
    else if(func_a == "jalr")
    {
        rs_a = get_reg(rs) + tr(",");
        rs_a.push_front(tr("$"));
        rd_a = get_reg(rd) + tr(";\n");
        rd_a.push_front(tr("$"));
    }
    else if(func_a == "sll" || func_a == "srl" || func_a == "sra")
    {
        shamt_a = QString::number(shamt.toInt(nullptr, 2)) + ";\n";
        rd_a = get_reg(rd) + tr(",");
        rd_a.push_front(tr("$"));
        rt_a = get_reg(rt) + tr(",");
        rt_a.push_front(tr("$"));
    }
    else if((func_num >= 32 && func_num <= 39) || func_a == "slt" || func_a == "sltu")
    {
        rs_a = get_reg(rs) + tr(",");
        rs_a.push_front(tr("$"));
        rd_a = get_reg(rd) + tr(",");
        rd_a.push_front(tr("$"));
        rt_a = get_reg(rt) + tr(";\n");
        rt_a.push_front(tr("$"));
    }
    else
        Error("No such operation");
    func_a.append(tr(" "));
    asb_code = func_a + rd_a + rs_a + rt_a;
    instrucion.append(asb_code);
}

void Disassemble::Itype(const QString &inst)
{
    QString opcode = inst.mid(0, 6);
    QString rs = inst.mid(6, 5);
    QString rt = inst.mid(11, 5);
    QString imm = inst.mid(16);
    QString rs_a = "";
    QString rt_a = "";
    QString address;
    QString imm_a;
    int func_num = opcode.toInt(nullptr, 2);
    if(imm.left(1) == "0")
        imm_a = QString::number(imm.toInt(nullptr, 2));
    else
        imm_a = QString::number(imm.toInt(nullptr, 2) - 65536);
    QString func_a;
    if(opcode != 1)
        func_a= get_func(func_num, "I");
    else
    {
        if(rt.toInt(nullptr, 2) == 0)
            func_a = "bltz";
        else if(rt.toInt(nullptr, 2) == 1)
            func_a = "bgez";
        else
            Error("No such operation");
    }
    if(func_a != "bgtz" || func_a != "blez"
       || func_a != "bltz" || func_a != "bgez")
    {
        rt_a = get_reg(rt) + tr(",");
        rt_a.push_front(tr("$"));
    }

    rs_a = get_reg(rs);
    rs_a.push_front(tr("$"));
    if(func_a == "lw" || func_a == "lb" || func_a == "lbu" || func_a == "lh" || func_a == "lhu"
       || func_a == "sw" || func_a == "sb" || func_a == "sh")
    {
        rs_a.push_front('(');
        rs_a.push_back(')');
        address = imm_a + rs_a;
        qDebug() << rs_a;
    }
    else if(func_a == "lui")
    {
        rs_a = tr("");
        address = imm_a;
    }
    else
    {
        rs_a.push_back(tr(","));
        rs_a.push_front(tr("$"));
        address = rs_a + imm_a;
    }
    func_a.append(" ");
    QString asb_code = func_a  + rt_a + address + ";\n";
    instrucion.append(asb_code);
}

void Disassemble::Jtype(const QString &inst)
{
    QString opcode = inst.mid(0, 6);
    QString target = inst.mid(6);
    QString tgt_a;
    int func_num = opcode.toInt(nullptr, 2);
    QString func_a = get_func(func_num, "J");
    if(target.left(1) == "0")
        tgt_a = QString::number(target.toInt(nullptr, 2));
    else
        tgt_a = QString::number(target.toInt(nullptr, 2)- 65536);
    func_a.append(tr(" "));
    QString asb_code = func_a + tgt_a + tr(";\n");
    instrucion.append(asb_code);
}


void Disassemble::complete_inst(QString &inst, const int &num, const int &func_num, const int &base)
{
    inst = QString::number(func_num, base);
    inst = inst.right(num);
    while(inst.length() < num)
    {
        inst.push_front(tr("0"));
    }
}

void Disassemble::Error(const QString &err_mess)
{
    QMessageBox::information(this, "ERROR", err_mess);
}

QString Disassemble::get_reg(const QString &reg)
{
    Assemble temp;
    QStringList regtemp;
    int index = reg.toInt(nullptr, 2);
    regtemp = temp.get_reglist();
    return regtemp[index];
}

QString Disassemble::get_func(const int &func_num, const QString &type)
{
    Assemble temp;
    QStringList functemp;
    QString res = "";
    if(type == "R")
    {
        functemp = temp.get_rtype();
        if(func_num >= 32 && func_num <= 39)
            res = functemp[func_num - 32];
        else if(func_num == 0)
            res = tr("sll");
        else if(func_num == 2)
            res = tr("srl");
        else if(func_num == 3)
            res = tr("sra");
        else if(func_num == 42)
            res = tr("slt");
        else if(func_num == 43)
            res = tr("sltu");
        else if(func_num == 8)
            res = tr("jr");
        else if(func_num == 9)
            res = tr("jalr");
        else
            Error("wrong code");
    }
    else if(type == "I")
    {
        functemp = temp.get_itype();
        if(func_num >= 8 && func_num <= 15)
            res = functemp[func_num - 8];
        else if(func_num == 32)
            res = tr("lb");
        else if(func_num == 33)
            res = tr("lh");
        else if(func_num == 35)
            res = tr("lw");
        else if(func_num == 36)
            res = tr("lbu");
        else if(func_num == 38)
            res = tr("lhu");
        else if(func_num == 40 || func_num == 41)
            res = functemp[func_num - 26];
        else if(func_num == 43)
            res = functemp[func_num - 30];
        else if(func_num >= 4 && func_num <= 7)
            res = functemp[func_num + 12];
        else
            Error("No such operation");
    }
    else if(type == "J")
    {
        functemp = temp.get_jtype();
        if(func_num == 2 || func_num == 3)
        {
            res = functemp[func_num - 2];
        }
        else
            Error("No such operation");
    }
    return res;
}

const QString Disassemble::Write()
{
    return instrucion;
}
