#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextEdit>
#include <QTextStream>
#include <QCloseEvent>
#include "highlighter.h"
#include "assemble.h"
#include "disassemble.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


private slots:
    void about();

    void assemble_b();

    void assemble_c();

    void disassemble();

    void on_actionExit_triggered();

    void on_actionNew_triggered();

    void on_actionOpen_triggered();

    void on_actionSave_as_triggered();

    void on_actionSave_triggered();

    void on_textEdit_textChanged();


private:
    Ui::MainWindow *ui;
    QString cpath;
    Highlighter *highlighter;
    void setEditor();
    void setFileMenu();
    void setBuildMenu();
    void setHelpMenu();
    QFont setFont();
    QString get_filename(const QString &path) const;
    QString get_filesuffix(const QString &path) const;
    void closeEvent(QCloseEvent *event);
    void save_file();
    Assemble asb;
};

#endif // MAINWINDOW_H
