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
#include <QList>
#include <config.h>
#include <coderform.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void groupCompent();
    void encodeQRButtonClicked(int index);
    void encodeBarcodeButtonClicked(int index);
    QLineEdit* getRemarkLineEdit(int index);
    QLineEdit* getBarcodeLineEdit(int index);
    QLabel* getImgLabel(int index);
    QLabel* getLatestOperateLabel(int index);
    void loadConfig();
    void writeConfig();
    void creatBtn();


private slots:
    void on_encoderButton_clicked();

    void on_encoderButton1_clicked();

    void on_encoderButton2_clicked();

    void on_encoderButton3_clicked();

    void on_encoderButton4_clicked();

    void on_encodeBarcodeBtn_clicked();

    void on_encodeBarcodeBtn4_clicked();

    void on_encodeBarcodeBtn3_clicked();

    void on_encodeBarcodeBtn2_clicked();

    void on_encodeBarcodeBtn1_clicked();
    void addNewTabPage();

private:
    static const int GROUP_NUM = 5;
    QList<QLineEdit*>* remarkLineEditList = new  QList<QLineEdit*>();
    QList<QLineEdit*>* barcodeLineEditList = new  QList<QLineEdit*>();
    QList<QLabel*>*  imgLabelList = new  QList<QLabel*>();
    QList<QLabel*>*  latestOperateLabelList = new  QList<QLabel*>();

    Ui::MainWindow *ui;
    Config* cfg = new Config();
};
#endif // MAINWINDOW_H
