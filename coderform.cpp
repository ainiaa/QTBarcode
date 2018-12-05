#include "coderform.h"
#include <QUuid>
#include <QMessageBox>
#include <zint/backend/zint.h>

int CoderForm::maxNum =0;

CoderForm::CoderForm(QWidget *parent, Config * cfg) :
    QWidget(parent)
{
    this->cfg = cfg;
    id = QString::number(++CoderForm::maxNum);
    this->setObjectName("CoderForm"+id);
   // this->resize(711, 481);

    codeLabel = new QLabel(this);
    codeLabel->setObjectName("codeLabel"+id);
    codeLabel->setGeometry(QRect(133, 50, 101, 23));
    imgLabel = new QLabel(this);
    imgLabel->setObjectName("imgLabel"+id);
    imgLabel->setGeometry(QRect(10, 90, 911, 361));
    imgLabel->setStyleSheet("background-color:gainsboro");
    barcodeLineEdit = new QLineEdit(this);
    barcodeLineEdit->setObjectName("barcodeLineEdit"+id);
    barcodeLineEdit->setGeometry(QRect(240, 50, 181, 23));
    encoderButton = new QPushButton(this);
    encoderButton->setObjectName("encoderButton"+id);
    encoderButton->setGeometry(QRect(440, 50, 75, 23));
    remarkLabel = new QLabel(this);
    remarkLabel->setObjectName("remarkLabel"+id);
    remarkLabel->setGeometry(QRect(200, 20, 31, 23));
    remarkLineEdit = new QLineEdit(this);
    remarkLineEdit->setObjectName("remarkLineEdit"+id);
    remarkLineEdit->setGeometry(QRect(240, 20, 181, 23));
    encodeBarcodeBtn = new QPushButton(this);
    encodeBarcodeBtn->setObjectName("encodeBarcodeBtn"+id);
    encodeBarcodeBtn->setGeometry(QRect(530, 50, 75, 23));
    latestOperateLabel = new QLabel(this);
    latestOperateLabel->setObjectName("latestOperateLabel"+id);
    latestOperateLabel->setGeometry(QRect(440, 20, 61, 21));

    connect(encoderButton,SIGNAL(clicked()), this,SLOT(on_encoderButton_clicked()));
    connect(encodeBarcodeBtn,SIGNAL(clicked()), this,SLOT(on_encodeBarcodeBtn_clicked()));

    encoderButton->setText(QApplication::translate("coderform", "\347\224\237\346\210\220\344\272\214\347\273\264\347\240\201", nullptr));
    latestOperateLabel->setText(QString());
    imgLabel->setText(QString());
    remarkLabel->setText(QApplication::translate("coderform", "\345\244\207\346\263\250", nullptr));
    encodeBarcodeBtn->setText(QApplication::translate("coderform", "\347\224\237\346\210\220\346\235\241\347\240\201", nullptr));
    codeLabel->setText(QApplication::translate("coderform", "\346\235\241\347\240\201/\344\272\214\347\273\264\347\240\201\345\206\205\345\256\271", nullptr));
}

CoderForm::~CoderForm()
{
}
QString CoderForm::getId()
{
    return id;
}

QString CoderForm::getRemark()
{
    return remarkLineEdit->text();
}
QString CoderForm::getBarcode()
{
    return barcodeLineEdit->text();
}

void CoderForm::on_encoderButton_clicked()
{
    QString barcode = barcodeLineEdit->text();
    if (barcode.isEmpty()) {
        QMessageBox::information(this,"提示","请输入二维码内容");
    } else {
        QString remark = remarkLineEdit->text();

        latestOperateLabel->setVisible(false);
        latestOperateLabel->setText(QString::number(BARCODE_QRCODE));

        struct zint_symbol *my_symbol = ZBarcode_Create();
        if(my_symbol != NULL)  {
            my_symbol->symbology= BARCODE_QRCODE;
            my_symbol->border_width = 1;
            my_symbol->whitespace_width = 1;
            my_symbol->scale = 2;
            strcpy(my_symbol->bgcolour,"fff0f0");
            QString fileName = cfg->GetConfigPath() +"output.bmp" ;
            QByteArray fileNameBa = fileName.toLatin1();
            const char * fileNameCC =fileNameBa.data();
            strcpy_s(my_symbol->outfile,fileNameCC);
            unsigned char* ch;
            QByteArray ba = barcode.toUtf8();
            ch=(unsigned char *)ba.data();
            int errNum = ZBarcode_Encode(my_symbol,ch,0);
            if (errNum > 5)
            {
                QMessageBox::critical(this,"错误",my_symbol->errtxt);
                return;
            }
            ZBarcode_Print(my_symbol,0);
            ZBarcode_Delete(my_symbol);
            QImage* barcodeImg= new QImage();
            barcodeImg->load(fileName);
            QPixmap pixmap = QPixmap::fromImage(*barcodeImg);
            remove(fileNameCC);

            int width = imgLabel->width();
            int height = imgLabel->height();
            printf("label height:%d width:%d!\n",height,width);
            QPixmap  fitpixmap = pixmap.scaled(height, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);
            imgLabel->setPixmap(fitpixmap);
            imgLabel->setAlignment(Qt::AlignCenter);
            imgLabel->setStyleSheet("background-color:gainsboro");

            emit sendData(remark);  //获取lineEdit的输入并且传递出去
        }
    }
}


void CoderForm::on_encodeBarcodeBtn_clicked()
{
    QString barcode = barcodeLineEdit->text();
    if (barcode.isEmpty()) {
        QMessageBox::information(this,"提示","请输入条形码内容");
    } else {
        QString remark = remarkLineEdit->text();
        latestOperateLabel->setVisible(false);
        latestOperateLabel->setText(QString::number(BARCODE_CODE128));

        struct zint_symbol *my_symbol = ZBarcode_Create();
        if(my_symbol != NULL)  {
            //my_symbol->eci = 26; //utf-8
            //string text = "dddddaaa";
            //memcpy(my_symbol->text, &text[0], text.size());
            my_symbol->output_options = BOLD_TEXT;
            my_symbol->symbology= BARCODE_CODE128;
            my_symbol->border_width = 1;
            my_symbol->whitespace_width = 2;
            //my_symbol->fontsize=2;
            my_symbol->scale = 3;
            strcpy(my_symbol->fgcolour,"000000");//2F4F4F
            strcpy(my_symbol->bgcolour,"FFFFFF");
            my_symbol->input_mode = DATA_MODE;
            //my_symbol->width = 5;
            my_symbol->height = 40;

            QString fileName = cfg->GetConfigPath() +"output.bmp" ;
            QByteArray fileNameBa = fileName.toUtf8();
            const char * fileNameCC =fileNameBa.data();
            strcpy_s(my_symbol->outfile, fileNameCC);

            unsigned char* ch;
            QByteArray ba = barcode.toUtf8();
            ch=(unsigned char *)ba.data();
            int errNum = ZBarcode_Encode(my_symbol,ch,0);
            if (errNum > 5)
            {
                QMessageBox::critical(this,"错误",my_symbol->errtxt);
                return;
            }
            ZBarcode_Print(my_symbol,0);
            ZBarcode_Delete(my_symbol);
            QImage* barcodeImg= new QImage();
            barcodeImg->load(fileName);
            QPixmap pixmap = QPixmap::fromImage(*barcodeImg);
            remove(fileNameCC);

            int width = imgLabel->width();
            int height = imgLabel->height();
            //height = pixmap.height();
            printf("label height:%d width:%d!\n",height,width);
            int width1 = 429;
            int height1 = 180;//pixmap.height();
            //printf("pixmap height:%d width:%d!\n",height1,width1);
            //currentImgLabel->setPixmap(pixmap);
            QPixmap fitpixmap = pixmap.scaled(width1, height1, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
            imgLabel->setPixmap(fitpixmap);
            imgLabel->setAlignment(Qt::AlignCenter);
            //ui->tabWidget->setStyleSheet("background-color:gainsboro");//gainsboro  silver ghostwhite
            imgLabel->setStyleSheet("background-color:gainsboro");

            emit sendData(remark);  //获取lineEdit的输入并且传递出去
        }
    }
}
