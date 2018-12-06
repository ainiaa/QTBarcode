#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QtWidgets/QWidget>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QTextCodec>
#include <QSettings>
#include <QDir>
#include <zint/backend/zint.h>
#include <QHBoxLayout>
#include<QResizeEvent>
#include <QList>
#include <QMap>
#include <config.h>
#include <coderform.h>
#include <codeform.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    static int tabNum;
protected:
    void cloneTabPage();
    void encodeQRButtonClicked();
    void encodeBarcodeButtonClicked();

    void loadConfig();
    void writeConfig();
    void creatBtn();
    bool eventFilter(QObject *target, QEvent *e);
    virtual void resizeEvent(QResizeEvent *event);

private slots:
    void addNewTabPage();
    void removeSubTab(int);
    void receiveData(QString data);   //接收传递过来的数据的槽

    void on_encoderButton_clicked();
    void on_encodeBarcodeBtn_clicked();

private:
    QMap<QString, CodeForm*> qMap;
    Ui::MainWindow *ui;
    Config* cfg = new Config();
};
#endif // MAINWINDOW_H
