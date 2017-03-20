#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
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
    ui->actionSave_as->setShortcut(tr("Ctrl+Shift+S"));
    ui->actionSave_as->setStatusTip(tr("另存为"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionExit_triggered()
{
     QApplication::exit( 0 );
}

void MainWindow::on_actionOpen_triggered()
{
    QString path = QFileDialog::getOpenFileName(this,tr("Open File"),".",tr("Assembler/BIN/COE Files(*.ASM *.bin *.coe)"));
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
}


void MainWindow::on_actionSave_as_triggered()
{
    QString path = QFileDialog::getSaveFileName(this,tr("Save File"),".",tr("Assembler Source(*.ASM);;COE File(*.coe);;BIN File(*.bin)"));
    if(!path.isEmpty())
    {
       QFile file(path);
       if(!file.open(QIODevice::WriteOnly|QIODevice::Text))//已只写方式和文本方式打开这个文件
       {
           QMessageBox::warning(this,tr("Save As File"),tr("can't save file"));
           return;
       }
       QTextStream out(&file);
       out<<ui->textEdit->toPlainText();
       file.close();
    }
}
