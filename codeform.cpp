#include "codeform.h"
#include <QUuid>
#include <QMessageBox>

CodeForm::CodeForm( QLabel *latestOperateLabel, QLabel *imgLabel, QLineEdit *remarkLineEdit, QLineEdit *barcodeLineEdit)
{
    this->latestOperateLabel = latestOperateLabel;
    this->imgLabel = imgLabel;
    this->remarkLineEdit = remarkLineEdit;
    this->barcodeLineEdit = barcodeLineEdit;
}

CodeForm::~CodeForm()
{
}

QString CodeForm::getRemark()
{
    return remarkLineEdit->text();
}
QString CodeForm::getBarcode()
{
    return barcodeLineEdit->text();
}

QLabel*  CodeForm::getImgLabel()
{
    return imgLabel;
}

QString CodeForm::getLatestOperate()
{
    return latestOperateLabel->text();
}

