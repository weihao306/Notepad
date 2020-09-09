#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    int MyMessageBox(QString string, QString Title);
    void closeEvent(QCloseEvent *event);
    QString fileName;   //全局变量，文件名称
    void WriteFile(QString filepath);     //写文件

private slots:
    void NewFile();     //新建文件
    void OpenFile();      //打开文件
    void SaveFile();      //保存文件
    void SetFont();       //字体设置
    void SetColor();      //颜色设置
    void SaveFileAs();      //文件另存为
    void DocumentModified();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
