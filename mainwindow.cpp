#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTextEdit>
#include <QDebug>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QSignalMapper>
#include <QLabel>
#include <QFont>
#include <QFontDialog>
#include <QColorDialog>
#include <QToolBar>
#include <QToolButton>
#include <QSpinBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //设置窗口标题
    this->setWindowTitle(tr("无标题 - MyNotepad[*]"));

    connect(ui->action_New, SIGNAL(triggered()), this, SLOT(NewFile()));
    connect(ui->action_Open, SIGNAL(triggered()), this, SLOT(OpenFile()));
    connect(ui->action_Save, SIGNAL(triggered()), this, SLOT(SaveFile()));
    connect(ui->action_SaveAs, SIGNAL(triggered()), this, SLOT(SaveFile()));
    connect(ui->action_Close, SIGNAL(triggered()), this, SLOT(SaveFile));
    connect(ui->action_Font, SIGNAL(triggered(bool)), this, SLOT(SetFont()));
    connect(ui->action_Color, SIGNAL(triggered(bool)), this, SLOT(SetClolor()));
    connect(ui->textEdit->document(), &QTextDocument::contentsChanged, this, &MainWindow::DocumentModified);

    //添加状态栏,临时消息显示2秒
    ui->statusbar->showMessage(tr("Hello World!"), 2000);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::DocumentModified()   //利用textEdit调用isModified函数来判断内容是否修改，并且将更改结果传给主窗口。
{
    setWindowModified(ui->textEdit->document()->isModified());
}

void MainWindow::NewFile()
{
    bool isModified = ui->textEdit->document()->isModified();//当前文档是否修改
    if(isModified)
    {
        if(fileName.isEmpty())
        {
            switch (MyMessageBox(tr("是否将更改保存到 无标题？"), tr("记事本")))
            {
                case QMessageBox::Save:
                    SaveFileAs();
                    break;
                case QMessageBox::Discard:
                    break;
                case QMessageBox::Cancel:
                    return;
                default:
                    break;
            }
        }
    }
    else
        SaveFile();
    ui->textEdit->clear();   //清理文本框
    fileName.clear();      //清除文件名
    this->setWindowTitle(tr("无标题 - 测试记事本[*]"));     //恢复主窗口标题为初始状态
    setWindowModified(false);       //窗口不显示被更改标志
}

void MainWindow::SaveFile()     //保存文件
{
    if(fileName.isEmpty())
        SaveFileAs();//如果文件名为空则文件另存为
    else
        WriteFile(fileName);
}

void MainWindow::SaveFileAs()   //文件另存为xx
{
    fileName = QFileDialog::getSaveFileName(this, tr("另存为"),
                                            "*.txt",
                                            tr("文本文档(*.txt);;所有文件 (*.*)"));
    if(!fileName.isEmpty())     //如果文件名非空，则将内容写入新文件
        WriteFile(fileName);
}

void MainWindow::WriteFile(QString filepath)    //写文件
{
    if(ui->textEdit->document()->isModified())
    {
        QFile file;
        file.setFileName(filepath);//设置文件名称
        bool ok = file.open(QIODevice::WriteOnly);
        if(ok)
        {
            QTextStream out(&file);
            out << ui->textEdit->toPlainText();
            file.close();
            ui->textEdit->document()->setModified(false);
            setWindowModified(false);

            QFileInfo fileInfo = QFileInfo(filepath);
            this->setWindowTitle(fileInfo.fileName() + "[*]");
        }
        else
        {
            QMessageBox::information(this, tr("错误提示"), tr("打开文件错误:") + filepath,
                                     QMessageBox::Abort);
        }
    }
}


int MainWindow::MyMessageBox(QString string, QString Title) //消息对话框
{
    QMessageBox mybox;
    mybox.setWindowTitle(string);
    mybox.setText(Title);
    mybox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    mybox.setButtonText(QMessageBox::Save, tr("保存"));
    mybox.setButtonText(QMessageBox::Discard, tr("不保存"));
    mybox.setButtonText(QMessageBox::Cancel, tr("取消"));

    return mybox.exec();
}

void MainWindow::OpenFile()
{
    if(ui->textEdit->document()->isModified())//如果已经被修改过了，应该先保存再打开
    {
        QString text(tr("是否将更改保存到 "));
        text += fileName.isEmpty() ? (tr("无标题？")) : fileName;
        switch (MyMessageBox(text, tr(("记事本"))))
        {
            case QMessageBox::Save:
                SaveFile();
                break;
            case QMessageBox::Discard:
                break;
            case QMessageBox::Cancel:
                return;
            default:
                break;
        }
    }

    QString openfilename = QFileDialog::getOpenFileName(this, tr("Open File"),
                           QDir::currentPath(),
                           tr("*.*"));

    if(openfilename.isEmpty())
        return;

    QFile file;
    file.setFileName(openfilename);//设置文件名称

    bool ok = file.open(QIODevice::ReadOnly);   //以只读方式打开文件
    if(ok)
    {
        QTextStream in(&file);
        ui->textEdit->setText(in.readAll());
        file.close();

        fileName = openfilename;
        ui->textEdit->document()->setModified(false);   //文档没有被修改过
        setWindowModified(false);   //窗口不显示被更改标志


        QFileInfo fi = QFileInfo(fileName);
        this->setWindowTitle(fi.fileName() + "[*]");
    }
    else
    {
        QMessageBox::information(this, tr("错误提示"), tr("打开文件错误") + file.errorString(),
                                 QMessageBox::Abort);
        return ;
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    bool isModified = ui->textEdit->document()->isModified();   //当前文档已经改变

}
