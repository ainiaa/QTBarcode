#ifndef CODERFORM_H
#define CODERFORM_H

#include <QWidget>
#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>
#include <QtWidgets/QTabWidget>
#include <config.h>
class CoderForm : public QWidget
{
    Q_OBJECT

public:
    explicit CoderForm(QWidget *parent = nullptr, Config * cfg = nullptr);
    ~CoderForm();
    static int maxNum;
    QString getId();

signals:
    void sendData(QString );//信号，用来向父窗体传值

private slots:
    void on_encoderButton_clicked();

    void on_encodeBarcodeBtn_clicked();

private:
    QPushButton *encoderButton;
    QLabel *latestOperateLabel;
    QLabel *imgLabel;
    QLineEdit *remarkLineEdit;
    QLabel *remarkLabel;
    QLineEdit *barcodeLineEdit;
    QPushButton *encodeBarcodeBtn;
    QLabel *codeLabel;
    QString id;
    Config *cfg;
};

#endif // CODERFORM_H
