#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    cpath("")
{
    ui->setupUi(this);
    setWindowTitle("Assembler");
    QPalette palette;
    palette.setColor(QPalette::Background, QColor(255,255,255));
    this->setPalette(palette);
    ui->actionOpen->setShortcut(tr("Ctrl+O"));
    ui->actionOpen->setStatusTip(tr("打开一个文件"));
    ui->actionExit->setShortcut(tr("Ctrl+W"));
    ui->actionExit->setStatusTip(tr("关闭程序"));
    ui->actionSave->setShortcut(tr("Ctrl+S"));
    ui->actionSave->setStatusTip((tr("保存")));
    ui->actionSave_as->setShortcut(tr("Ctrl+Shift+S"));
    ui->actionSave_as->setStatusTip(tr("另存为"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(windowTitle() == "Assembler *")
    {
        switch( QMessageBox::information(this,tr("提示"),tr("是否保存更改?"),tr("保存"), tr("不保存"), tr("取消"), 0, 1))
        {
        case 0:
            save_file();
            event->accept();
            break;
        case 1:
            event->accept();
            break;
        default:
            event->ignore();
            break;
        }
    }
    else
        event->accept();
}

void MainWindow::save_file()
{
    QString path = QFileDialog::getSaveFileName(this,tr("Save File"),".",tr("Assembler Source(*.ASM);;COE File(*.coe);;BIN File(*.bin)"));
    QFile file(path);
    cpath = path;
    if(!path.isEmpty())
    {
        if(!file.open(QIODevice::WriteOnly|QIODevice::Text))//已只写方式和文本方式打开这个文件
        {
           QMessageBox::warning(this,tr("Save File"),tr("can't save file"));
           return;
        }
        QTextStream out(&file);
        out<<ui->textEdit->toPlainText();
        file.close();
    }
    setWindowTitle("Assembler");
}

void MainWindow::on_actionExit_triggered()
{
     QApplication::exit( 0 );
}

void MainWindow::on_actionOpen_triggered()
{
    QString path = QFileDialog::getOpenFileName(this,tr("Open File"),".",tr("Assembler/BIN/COE Files(*.ASM *.bin *.coe)"));
    cpath = path;
    if(!path.isEmpty())
    {
        QFile file(path);
        if(!file.open(QIODevice::ReadOnly|QIODevice::Text))//以只读方式和文本方式打开这个文件
        {
          QMessageBox::warning(this,tr("Read File"),tr("can't open file"));
          return;
        }
        QTextStream in(&file);
        ui->textEdit->setText(in.readAll());
        file.close();
    }
    setWindowTitle("Assembler");
}

void MainWindow::on_actionSave_triggered()
{
    if(!cpath.isEmpty())
    {
        QFile cfile(cpath);
        if(!cfile.open(QIODevice::WriteOnly|QIODevice::Text))//已只写方式和文本方式打开这个文件
        {
           QMessageBox::warning(this,tr("Save File"),tr("can't save file"));
           return;
        }
        QTextStream out(&cfile);
        out<<ui->textEdit->toPlainText();
        cfile.close();
        setWindowTitle("Assembler");
    }
    else
    {
        save_file();
    }
}

void MainWindow::on_actionSave_as_triggered()
{
    save_file();
}

void MainWindow::on_textEdit_textChanged()
{
    setWindowTitle("Assembler *");
}
