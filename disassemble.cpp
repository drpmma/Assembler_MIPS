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
        int func_num;
        if(num == 16)
        {
            func_num = (*i).toInt(nullptr, 16);
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
        if(opcode == tr("000000"))
            Rtype(*i);
        else if((opcode >= tr("000100") && opcode <= tr("001011"))
                || opcode == tr("100000") || opcode == tr("100001")
                || opcode == tr("100011") || opcode == tr("100100")
                || opcode == tr("100101") || opcode == tr("101000")
                || opcode == tr("101001") || opcode == tr("101011"))
            Itype(*i);
        else if(opcode == tr("000010") || opcode == tr("000011"))
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
    QString rs_a = get_reg(rs);
    QString rt_a = get_reg(rt);
    QString rd_a = get_reg(rd);
    QString shamt_a = QString::number(shamt.toInt(nullptr, 2));
    QString func_a = get_func(func.toInt(nullptr, 2));
    QString asb_code;
    if(func_a == "jr")
        asb_code = func_a + tr(" ") + rs_a + ";\n";
    else if(func_a == "jalr")
        asb_code = func_a + tr(" ") + rs_a + ","
                   + rd_a + ";\n";
    else if(func_a == "sll" || func_a == "srl" || func_a == "sra")
        asb_code = func_a + tr(" ") + rd_a + ","
                   + rt_a + "," + shamt + ";\n";
}

void Disassemble::Itype(const QString &inst)
{

}

void Disassemble::Jtype(const QString &inst)
{

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

QString Disassemble::get_func(const int &func_num)
{
    QString res = "";
    if(func_num == 32)
        res = tr("add");
    else if(func_num == 33)
        res = tr("addu");
    else if(func_num == 34)
        res = tr("sub");
    else if(func_num == 35)
        res = tr("subu");
    else if(func_num == 36)
        res = tr("and");
    else if(func_num == 37)
        res = tr("or");
    else if(func_num == 38)
        res = tr("xor");
    else if(func_num == 39)
        res = tr("nor");
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
    return res;
}
