#include "assemble.h"

Assemble::Assemble():code("")
{
    Reglist = QString("zero, ,v0,v1,a0,a1,a2,a3,"\
                      "t0,t1,t2,t3,t4,t5,t6,t7,"\
                      "s0,s1,s2,s3,s4,s5,s6,s7,"\
                      "t8,t9, , ,gp,sp,fp,ra").split(",");
    Rtypelist = QString("add,addu,sub,subu,and,or,xor,nor,"\
                        "sll,srl,sra,slt,sltu,jr,jalr").split(",");
    Itypelist = QString("addi,addiu,slti,sltiu,andi,ori,xori,lui,"\
                        "lw,lb,lbu,lh,lhu,sw,sh,sb,beq,bne,blez,bgtz,bltz,bgez").split(",");
    Jtypelist = QString("j,jal").split(",");
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
        if(Rtypelist.contains(line_split[0]) == true)
            Rtype_inst(line_split);
        else if(Itypelist.contains(line_split[0]) == true)
            Itype_inst(line_split, i - code.begin());
        else if(Jtypelist.contains(line_split[0]) == true)
            Jtype_inst(line_split, i - code.begin());
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
            QString temp = inst[i].mid(1);
            if(Reglist.contains(temp) == true || QRegExp("[1-2]?[0-9]|[30]|[31]").indexIn(temp) != -1)
            {
                int reg = get_reg(temp);
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
    int mark = get_inst(inst[0], "R");
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

void Assemble::Itype_inst(const QStringList &inst, const int &inst_num)
{
    QString bi_inst = "";
    QString opcode = "";
    QString rs = "", rt = "";
    QString imm = "";
    if(Itypelist.mid(ADDI, 7).contains(inst[0]) == true)
    {
        int op = get_inst(inst[0], "I");
        complete_inst(opcode, 6, op + 8, 2);
        for(int i = 1; i < 4; ++i)
        {
            int reg = get_reg(inst[i].mid(1));
            switch (i)
            {
            case 1:
                complete_inst(rt, 5, reg, 2);
                break;
            case 2:
                complete_inst(rs, 5, reg, 2);
                break;
            case 3:
            {
                int imm_num = inst[i].toInt();
                if(Itypelist.mid(ADDI, 4).contains(inst[0]) == true
                        && imm_num < 0)
                    complete_inst(imm, 16, qAbs(imm_num), 2, "1");
                else if(Itypelist.mid(ANDI, 3).contains(inst[0]) == true
                        || imm_num >= 0)
                    complete_inst(imm, 16, imm_num, 2);
                break;
            }
            default:
                break;
            }
        }
    }
    else if(Itypelist.mid(LUI, 1).contains(inst[0]) == true)
    {
        int op = get_inst(inst[0], "I");
        complete_inst(opcode, 6, op + 8, 2);
        for(int i = 1; i < 4; ++i)
        {
            int reg = get_reg(inst[i].mid(1));
            complete_inst(rs, 5, 0, 2);
            switch (i)
            {
            case 1:
                complete_inst(rt, 5, reg, 2);
                break;
            case 2:
            {
                int imm_num = inst[i].toInt();
                if(imm_num < 0)
                    complete_inst(imm, 16, qAbs(imm_num), 2, "1");
                else
                    complete_inst(imm, 16, imm_num, 2);
                break;
            }
            default:
                break;
            }
        }
    }
    else if(Itypelist.mid(LW, 8).contains(inst[0]) == true)
    {
        QString offset = "";
        int mark = get_inst(inst[0], "I");
        switch (mark)
        {
        case LB:
            complete_inst(opcode, 6, 32, 2);
            break;
        case LBU:
            complete_inst(opcode, 6, 36, 2);
            break;
        case LH:
            complete_inst(opcode, 6, 33, 2);
            break;
        case LHU:
            complete_inst(opcode, 6, 37, 2);
            break;
        case LW:
            complete_inst(opcode, 6, 35, 2);
            break;
        case SB:
            complete_inst(opcode, 6, 40, 2);
            break;
        case SH:
            complete_inst(opcode, 6, 41, 2);
            break;
        case SW:
            complete_inst(opcode, 6, 43, 2);
            break;
        default:
            break;
        }
        for(int i = 1; i <= 2; ++i)
        {
            int reg;
            int num;
            int index;
            switch (i) {
            case 1:
                reg = get_reg(inst[i].mid(1));
                complete_inst(rt, 5, reg, 2);
                break;
            case 2:
                if(inst[i].indexOf("(") != -1)
                {
                    index = inst[i].indexOf("(");
                    num = inst[i].mid(0, index).toInt();
                    if(num < 0)
                        complete_inst(offset, 16, qAbs(num), 2, "1");
                    else
                        complete_inst(offset, 16, num, 2);
                    if(inst[i][index + 1] == "$" && inst[i].endsWith(")"))
                    {
                        int reg = get_reg(inst[i].mid(index + 2).remove(")"));
                        complete_inst(rs, 5, reg, 2);
                    }
                }
                else
                {

                }
            default:
                break;
            }
            imm = offset;
        }
    }
    else if(Itypelist.mid(BEQ, 6).contains(inst[0]) == true)
    {
        int op = get_inst(inst[0], "I");
        if(inst[0] != "bltz" && inst[0] != "bgez")
            complete_inst(opcode, 6, op - 12, 2);
        else
        {
            complete_inst(opcode, 6, 1, 2);
        }

        for(int i = 1; i < 4; ++i)
        {
            int reg;
            switch (i)
            {
            case 1:
                reg = get_reg(inst[i].mid(1));
                complete_inst(rs, 5, reg, 2);
                break;
            case 2:
                reg = get_reg(inst[i].mid(1));
                if(inst[0] != "bltz" && inst[0] != "bgez" && inst[0] != "bgtz" && inst[0] != "blez")
                    complete_inst(rt, 5, reg, 2);
                else
                {
                    complete_inst(rt, 5, 0, 2);
                    int offset;
                    if(inst[i].contains(QRegExp("^[1-9]?[0-9]*$")) == true)
                        offset = inst[i].toInt();
                    else
                        offset = map[inst[i]] - inst_num - 1;
                    if(offset < 0)
                        complete_inst(imm, 16, qAbs(offset), 2, "1");
                    else
                        complete_inst(imm, 16, offset, 2);
                }
                break;
            case 3:
            {
                if(inst[0] != "bltz" && inst[0] != "bgez" && inst[0] != "bgtz" && inst[0] != "blez")
                {
                    int offset;
                    if(inst[i].contains(QRegExp("^[1-9]?[0-9]*$")) == true)
                        offset = inst[i].toInt();
                    else
                        offset = map[inst[i]] - inst_num - 1;
                    if(offset < 0)
                        complete_inst(imm, 16, qAbs(offset), 2, "1");
                    else
                        complete_inst(imm, 16, offset, 2);
                }
                break;
            }
            default:
                break;
            }
        }
    }
    bi_inst = opcode + rs + rt + imm;
    qDebug() << bi_inst;
}

void Assemble::Jtype_inst(const QStringList &inst)
{
    QString bi_inst = "";
    QString opcode = "";
    QString imm = "";
    int op = get_inst(inst[0], "J");
    complete_inst(opcode, 6, 2 + op, 2);
    int offset;
    if(inst[1].contains(QRegExp("^[1-9]?[0-9]*$")) == true)
        offset = inst[1].toInt();
    else
        offset = map[inst[1]];
    if(offset < 0)
        complete_inst(imm, 26, qAbs(offset), 2, "1");
    else
        complete_inst(imm, 26, offset, 2);
    bi_inst = opcode + imm;
    qDebug() << bi_inst;
}

void Assemble::complete_inst(QString &inst, const int &num, const int &func_num, const int &base,
                             const QString &fill, const int &isfront)
{
    inst = QString::number(func_num, base);
    if(fill == "1")
    {
        for(auto i = inst.begin(); i != inst.end(); ++i)
        {
            if(*i == "0")
                inst.replace(i - inst.begin(), 1, '1');
            else
                inst.replace(i - inst.begin(), 1, '0');
        }
        if(*(inst.end() - 1) == "0")
            inst.replace(inst.length() - 1, 1, '1');
        else
            inst.replace(inst.length() - 1, 1, '0');
    }
    while(inst.length() < num)
    {
        if(isfront == 1)
            inst.push_front(fill);
        else
            inst.push_back(fill);
    }
}
int Assemble::get_reg(const QString &reg) const
{
    auto j = Reglist.begin();
    for(; j != Reglist.end(); ++j)
        if(*j == reg)
            break;
    return j - Reglist.begin();
}

int Assemble::get_inst(const QString &inst, const QString &type) const
{
    decltype(Rtypelist.begin()) j, begin, end;
    if(type == "R")
    {
        begin = Rtypelist.begin();
        j = Rtypelist.begin();
        end = Rtypelist.end();
    }
    else if(type == "I")
    {
        begin = Itypelist.begin();
        j = Itypelist.begin();
        end = Itypelist.end();
    }
    else if(type == "J")
    {
        begin = Jtypelist.begin();
        j = Jtypelist.begin();
        end = Jtypelist.end();
    }
    for(; j != end; ++j)
        if(*j == inst)
            break;
    return j - begin;
}
