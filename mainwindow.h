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
    void groupCompent();
    void cloneTabPage();
    void encodeQRButtonClicked(int index);
    void encodeBarcodeButtonClicked(int index);
    virtual void resizeEvent(QResizeEvent *event);
    void loadConfig();
    void writeConfig();
    void creatBtn();

    QLineEdit* getRemarkLineEdit(int index);
    QLineEdit* getBarcodeLineEdit(int index);
    QLabel* getImgLabel(int index);
    QLabel* getLatestOperateLabel(int index);

    bool eventFilter(QObject *target, QEvent *e);


private slots:
    void addNewTabPage();
    void removeSubTab(int);
    void receiveData(QString data);   //接收传递过来的数据的槽

    void on_encoderButton_clicked();
    void on_encodeBarcodeBtn_clicked();

private:
    QList<QLineEdit*>* remarkLineEditList = new  QList<QLineEdit*>();
    QList<QLineEdit*>* barcodeLineEditList = new  QList<QLineEdit*>();
    QList<QLabel*>*  imgLabelList = new  QList<QLabel*>();
    QList<QLabel*>*  latestOperateLabelList = new  QList<QLabel*>();
    QMap<QString, CodeForm> qMap;
    Ui::MainWindow *ui;
    Config* cfg = new Config();
};
#endif // MAINWINDOW_H
