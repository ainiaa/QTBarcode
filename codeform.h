#ifndef CODEFORM_H
#define CODEFORM_H

#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
class CodeForm
{
public:
    explicit CodeForm( QLabel *latestOperateLabel,  QLabel *imgLabel, QLineEdit *remarkLineEdit, QLineEdit *barcodeLineEdit);
    ~CodeForm();
    QString getRemark();
    QString getBarcode();
    QString getLatestOperate();
    QLabel* getImgLabel();

private:
    QLabel *latestOperateLabel;
    QLabel *imgLabel;
    QLineEdit *remarkLineEdit;
    QLineEdit *barcodeLineEdit;
};

#endif // CODEFORM_H
