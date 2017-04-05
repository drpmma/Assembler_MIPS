#ifndef DISASSEMBLE_H
#define DISASSEMBLE_H

#include <QMainWindow>
#include <QString>
#include <QMessageBox>
#include <QStringList>
#include <QDebug>

class Disassemble : public QMainWindow
{
    Q_OBJECT
public:
    explicit Disassemble(QWidget *parent = 0);
    void Read_coe(const QString &content);
    void vector_handle();
private:
    QStringList vector;
    void Error(const QString &err_mess);

signals:

public slots:
};

#endif // DISASSEMBLE_H
