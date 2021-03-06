#ifndef DISASSEMBLE_H
#define DISASSEMBLE_H

#include <QMainWindow>
#include <QString>
#include <QMessageBox>
#include <QStringList>
#include <QDebug>
#include <QMap>
#include "assemble.h"

class Disassemble : public QMainWindow
{
    Q_OBJECT
public:
    explicit Disassemble(QWidget *parent = 0);
    void Read_coe(const QString &content);
    void Read_bin(const QString &content);
    void vector_handle(const int &num);
    const QString Write();

private:
    QStringList vector;
    QStringList instruction;
    QMap <int, QString> lb;
    void Error(const QString &err_mess);
    void complete_inst(QString &inst, const int &num,
                       const int &func_num, const int &base);
    void Rtype(const QString &inst);
    void Itype(const QString &inst);
    void Jtype(const QString &inst);
    void deal_label();
    QString get_reg(const QString &reg);
    QString get_func(const int &func_num, const QString &type);
    int lb_num;
    int inst_num;

signals:

public slots:
};

#endif // DISASSEMBLE_H
