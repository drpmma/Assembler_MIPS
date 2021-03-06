#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    cpath("")
{
    setEditor();
    setFileMenu();
    setBuildMenu();
    setHelpMenu();
    setWindowTitle("Assembler *");
}

MainWindow::~MainWindow()
{
    delete ui;
    delete highlighter;
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
        out << ui->textEdit->toPlainText();
        file.close();
    }
    setWindowTitle("Assembler");
}

void MainWindow::on_actionExit_triggered()
{
     this->close();
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
        out << ui->textEdit->toPlainText();
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

void MainWindow::on_actionNew_triggered()
{
    cpath = "";
    if(windowTitle() == "Assembler *")
    {
        switch( QMessageBox::information(this,tr("提示"),tr("是否保存更改?"),tr("保存"), tr("不保存"), tr("取消"), 0, 1))
        {
        case 0:
            save_file();
            ui->textEdit->clear();
            break;
        case 1:
            ui->textEdit->clear();
            break;
        default:
            break;
        }
    }
    else
        ui->textEdit->clear();
}

void MainWindow::setEditor()
{
    ui->setupUi(this);

    highlighter = new Highlighter(ui->textEdit->document());

    QPalette palette;
    palette.setColor(QPalette::Background, QColor(255,255,255));
    this->setPalette(palette);
}

void MainWindow::setFileMenu()
{
    ui->actionNew->setShortcut(QKeySequence::New);
    ui->actionNew->setStatusTip(tr("新建一个文件"));
    ui->actionOpen->setShortcut(QKeySequence::Open);
    ui->actionOpen->setStatusTip(tr("打开一个文件"));
    ui->actionExit->setShortcut(QKeySequence::Quit);
    ui->actionExit->setStatusTip(tr("关闭程序"));
    ui->actionSave->setShortcut(QKeySequence::Save);
    ui->actionSave->setStatusTip((tr("保存")));
    ui->actionSave_as->setShortcut(QKeySequence::SaveAs);
    ui->actionSave_as->setStatusTip(tr("另存为"));
}

void MainWindow::setBuildMenu()
{
    QMenu *buildMenu = new QMenu(tr("Build"), this);

    ui->menuBar->addMenu(buildMenu);
    ui->menuBar->setFont(setFont());

    buildMenu->setFont(setFont());
    buildMenu->addAction(tr("Assemble (Binary file)"), this, SLOT(assemble_b()), tr("F5"));
    buildMenu->addAction(tr("Assemble (COE file)"), this, SLOT(assemble_c()), tr("F6"));
    buildMenu->addAction(tr("Disassemble"), this, SLOT(disassemble()), tr("F10"));
    buildMenu->addSeparator();
    buildMenu->addAction(tr("Output directory"), this, SLOT(open_dir()), tr("F12"));
}

void MainWindow::setHelpMenu()
{
    QMenu *helpMenu = new QMenu(tr("&Help"), this);
    ui->menuBar->addMenu(helpMenu);
    ui->menuBar->setFont(setFont());

    helpMenu->setFont(setFont());
    helpMenu->addAction(tr("&About"), this, SLOT(about()));
    helpMenu->addAction(tr("About &Qt"), qApp, SLOT(aboutQt()));
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About Assembler"),
                tr("<p> <p><b><font size = '4' face = 'Courier New'>Assembler</b></p> " \
                   "<font size = '4' face = 'Courier New'>copy right <a href='https://github.com/drpmma/Assembler_MIPS'>@drpmma</a> github" \
                   "</p>"));
}

QFont MainWindow::setFont()
{
    QFont font;
    font.setFamily("Microsoft JhengHei Light");
    font.setFixedPitch(true);
    font.setPointSize(10);
    return font;
}

void MainWindow::assemble_b()
{
    Assemble asb;
    asb.Read(ui->textEdit->toPlainText());
    asb.inst_handle();
    if(cpath.isEmpty() || windowTitle() == "Assembler *")
        save_file();
    else
        asb.save_bi(get_filename(cpath));
}

void MainWindow::assemble_c()
{
    Assemble asbc;
    asbc.Read(ui->textEdit->toPlainText());
    asbc.inst_handle();
    if(cpath.isEmpty() || windowTitle() == "Assembler *")
        save_file();
    else
        asbc.save_coe(get_filename(cpath));
}

void MainWindow::disassemble()
{
    Disassemble d;
    QString path = QFileDialog::getOpenFileName(this,tr("Open File"),".",tr("BIN/COE Files(*.bin *.coe)"));
    if(!path.isEmpty())
    {
        QFile file(path);
        if(get_filesuffix(path) == "coe")
        {
            if(file.open(QIODevice::ReadOnly|QIODevice::Text))
            {
                QTextStream in(&file);
                d.Read_coe(in.readAll());
                ui->textEdit->setText(d.Write());
            }
        }
        else if(get_filesuffix(path) == "bin")
        {
            if(file.open(QIODevice::ReadOnly))
            {
                d.Read_bin(file.readAll().toHex());
                ui->textEdit->setText(d.Write());
            }
        }
    }
}

void MainWindow::open_dir()
{
    QDesktopServices::openUrl(QApplication::applicationDirPath());
}

QString MainWindow::get_filename(const QString &path) const
{
    QFileInfo fileinfo;
    QString file_name;
    fileinfo = QFileInfo(path);
    file_name = fileinfo.fileName();
    file_name.replace("." + fileinfo.suffix(), "");
    return file_name;
}

QString MainWindow::get_filesuffix(const QString &path) const
{
    QFileInfo fileinfo;
    QString file_suffix;
    fileinfo = QFileInfo(path);
    file_suffix = fileinfo.suffix();
    return file_suffix;
}
