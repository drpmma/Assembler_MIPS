#include "assemble.h"

Assemble::Assemble():code("")
{
    QString a = QString("zero, ,v0,v1,a0,a1,a2,a3,"\
                    "t0,t1,t2,t3,t4,t5,t6,t7,"\
                    "s0,s1,s2,s3,s4,s5,s6,s7,"\
                    "t8,t9, , ,gp,sp,fp,ra");
    Reglist = a.split(",");
    QString b = QString("add,addu,sub,subu,and,or,xor,nor,sll,srl,sra,slt,sltu,jr,jalr");
    Rtypelist = b.split(",");
}

void Assemble::Read(const QString &content)
{
    QString temp = content.trimmed();
    temp.replace(QRegExp(":"), ":\n");
    code = temp.split("\n");
    for(auto i = code.begin(); i != code.end(); ++i)
    {
        *i = i->trimmed();
        if(!(i->isEmpty()))
        {
            i->replace(QRegExp("//[^\n]*|#[^\n]*"), "");
            if(i->contains(QRegExp("\\b[A-Za-z0-9_]+:")) == true)
            {
                QString lb = i->left(i->indexOf(":"));
                map.insert(lb, i - code.begin());
                code.removeAt(i - code.begin());
            }
        }
        else
            code.removeAt(i - code.begin());
    }
}

void Assemble::inst_handle()
{
    for(auto i = code.begin(); i != code.end(); ++i)
    {
        QStringList line_split = i->split(QRegExp("\\s+|\\s*[,]\\s*|\\s*[;]"));
        qDebug() << line_split;
        if(Rtypelist.contains(line_split[0]) == true)
            Rtype_inst(line_split);
        //
    }
}

void Assemble::Rtype_inst(const QStringList &inst)
{
    QString bi_inst = "";
    QString opcode = "000000";
    QString rd = "00000", rs = "00000", rt = "00000";
    QString shamt = "00000";
    QString func = "";
    int func_base = 32;
    for(int i = 1; i < 4; ++i)
    {
        if(i == 3 && inst[0] == "jalr")
            break;
        if(i == 2 && inst[0] == "jr")
            break;
        if(i == 3 && inst[i][0] != '$' && inst[i][0] != 'r')
        {
            complete_inst(shamt, 5, inst[i].toInt(), 2);
            complete_inst(rs, 5, 0, 2);
        }
        else if(inst[i][0] == '$' || inst[i][0] == 'r')
        {
            QString temp = "";
            if(inst[i] == "$zero")
                temp = "zero";
            else
                temp = inst[i].mid(1,2);
            if(Reglist.contains(temp) == true || QRegExp("[1-2]?[0-9]|[30]|[31]").indexIn(temp) != -1)
            {
                auto j = Reglist.begin();
                for(; j != Reglist.end(); ++j)
                    if(*j == temp)
                        break;
                int reg = j - Reglist.begin();

                switch (i)
                {
                    {
                        case 1:
                        if(inst[0] == "jalr" || inst[0] == "jr")
                            complete_inst(rs, 5, reg, 2);
                        else
                            complete_inst(rd, 5, reg, 2);
                            break;
                        case 2:
                        if(inst[0] != "sll" && inst[0] != "srl" && inst[0] != "sra")
                            complete_inst(rs, 5, reg, 2);
                        else if(inst[0] == "jr")
                            complete_inst(rd, 5, reg, 2);
                        else
                            complete_inst(rt, 5, reg, 2);
                            break;
                        case 3:
                        if(inst[0] != "sll" && inst[0] != "srl" && inst[0] != "sra")
                            complete_inst(rt, 5, reg, 2);
                            break;
                    }
                }
            }
            //
        }


    }
    auto i = Rtypelist.begin();
    for(; i != Rtypelist.end(); ++i)
    {
        if(*i == inst[0])
            break;
    }
    int mark = i - Rtypelist.begin();
    switch (mark)
    {
    case ADD:
        complete_inst(func, 6, mark+func_base, 2);
        break;
    case ADDU:
        complete_inst(func, 6, mark+func_base, 2);
        break;
    case SUB:
        complete_inst(func, 6, mark+func_base, 2);
        break;
    case SUBU:
        complete_inst(func, 6, mark+func_base, 2);
        break;
    case AND:
        complete_inst(func, 6, mark+func_base, 2);
        break;
    case OR:
        complete_inst(func, 6, mark+func_base, 2);
        break;
    case XOR:
        complete_inst(func, 6, mark+func_base, 2);
        break;
    case NOR:
        complete_inst(func, 6, mark+func_base, 2);
        break;
    case SLL:
        complete_inst(func, 6, 0, 2);
        break;
    case SRL:
        complete_inst(func, 6, 2, 2);
        break;
    case SRA:
        complete_inst(func, 6, 3, 2);
        break;
    case SLT:
        complete_inst(func, 6, 42, 2);
        break;
    case SLTU:
        complete_inst(func, 6, 43, 2);
        break;
    case JR:
        complete_inst(func, 6, 8, 2);
        break;
    case JALR:
        complete_inst(func, 6, 9, 2);
        break;
    default:
        break;
    }
    bi_inst = opcode + rs + rt + rd + shamt + func;
    qDebug() << bi_inst;
}

void Assemble::complete_inst(QString &inst, const int &num, int func_num, int base)
{
    inst = QString::number(func_num, base);
    while(inst.length() < num)
    {
        inst.push_front('0');
    }
}
