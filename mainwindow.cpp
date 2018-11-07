#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    groupCompent();
    loadConfig();
    setFixedSize(this->width(), this->height());
}

MainWindow::~MainWindow()
{
    this->writeConfig();
    delete ui;
    delete cfg;
}

//加载config
void MainWindow::loadConfig()
{
    for(int i =0; i < GROUP_NUM;i++)
    {
        QString index = QString::number(i);
        QString remark = cfg->Get(index,"remark").toString();
        if (!remark.isEmpty())
        {
            this->getRemarkLineEdit(i)->setText(remark);
            ui->tabWidget->setTabText(i,remark);
        }

        QString barcode =  cfg->Get(index,"barcode").toString();
        if (!barcode.isEmpty())
        {
            this->getBarcodeLineEdit(i)->setText(barcode);
            QString latestOperate = cfg->Get(index,"latestOperate").toString();
            if (latestOperate == QString::number(BARCODE_QRCODE))
            {
                this->encodeQRButtonClicked(i);
            } else {
                this->encodeBarcodeButtonClicked(i);
            }
        }
    }
}

//写入config
void MainWindow::writeConfig()
{
    for(int i =0; i < GROUP_NUM;i++)
    {
        QString index = QString::number(i);
        QString remark = this->getRemarkLineEdit(i)->text();
        cfg->Set(index,"remark",remark);
        QString barcode = this->getBarcodeLineEdit(i)->text();
        cfg->Set(index,"barcode",barcode);
        QString latestOperate = this->getLatestOperateLabel(i)->text();
        cfg->Set(index,"latestOperate",latestOperate);
    }
}



void MainWindow::groupCompent()
{
    remarkLineEditGroup[0] = ui->remarkLineEdit;
    remarkLineEditGroup[1] = ui->remarkLineEdit1;
    remarkLineEditGroup[2] = ui->remarkLineEdit2;
    remarkLineEditGroup[3] = ui->remarkLineEdit3;
    remarkLineEditGroup[4] = ui->remarkLineEdit4;

    barcodeLineEditGroup[0] = ui->barcodeLineEdit;
    barcodeLineEditGroup[1] = ui->barcodeLineEdit1;
    barcodeLineEditGroup[2] = ui->barcodeLineEdit2;
    barcodeLineEditGroup[3] = ui->barcodeLineEdit3;
    barcodeLineEditGroup[4] = ui->barcodeLineEdit4;

    imgLabelGroup[0] = ui->imgLabel;
    imgLabelGroup[1] = ui->imgLabel1;
    imgLabelGroup[2] = ui->imgLabel2;
    imgLabelGroup[3] = ui->imgLabel3;
    imgLabelGroup[4] = ui->imgLabel4;

    latestOperateLabelGroup[0] = ui->latestOperateLabel;
    latestOperateLabelGroup[1] = ui->latestOperateLabel1;
    latestOperateLabelGroup[2] = ui->latestOperateLabel2;
    latestOperateLabelGroup[3] = ui->latestOperateLabel3;
    latestOperateLabelGroup[4] = ui->latestOperateLabel4;
}
//获得备注控件
QLineEdit* MainWindow::getRemarkLineEdit(int index)
{
    return (QLineEdit*)remarkLineEditGroup[index];
}
//获得条码控件
QLineEdit* MainWindow::getBarcodeLineEdit(int index)
{
    return (QLineEdit*)barcodeLineEditGroup[index];
}
//获得img展示容器
QLabel* MainWindow::getImgLabel(int index)
{
    return (QLabel*)imgLabelGroup[index];
}
QLabel* MainWindow::getLatestOperateLabel(int index)
{
    return (QLabel*)latestOperateLabelGroup[index];
}


//生成二维码
void MainWindow::encodeQRButtonClicked(int index)
{
    QString barcode = this->getBarcodeLineEdit(index)->text();
    if (barcode.isEmpty()) {
        QMessageBox::information(this,"提示","请输入二维码内容");
    } else {
        QString remark = this->getRemarkLineEdit(index)->text();

        if (!remark.isEmpty()) {
            int currentIndex = ui->tabWidget->currentIndex();
            ui->tabWidget->setTabText(currentIndex,remark);
        }
        QLabel* latestOperateLabel = this->getLatestOperateLabel(index);
        latestOperateLabel->setVisible(false);
        latestOperateLabel->setText(QString::number(BARCODE_QRCODE));

        struct zint_symbol *my_symbol = ZBarcode_Create();
        if(my_symbol != NULL)  {
            my_symbol->symbology= BARCODE_QRCODE;
            my_symbol->border_width = 2;
            my_symbol->scale = 3;
            QString fileName = cfg->GetConfigPath() +"output.bmp" ;
            QByteArray fileNameBa = fileName.toLatin1();
            strcpy_s(my_symbol->outfile, fileNameBa.data());
            unsigned char* ch;
            QByteArray ba = barcode.toLatin1();
            ch=(unsigned char *)ba.data();
            ZBarcode_Encode(my_symbol,ch,0);
            ZBarcode_Print(my_symbol,0);
            ZBarcode_Delete(my_symbol);
            QImage* barcodeImg= new QImage();
            barcodeImg->load("output.bmp");
            QPixmap pixmap = QPixmap::fromImage(*barcodeImg);
            QLabel* currentImgLabel =this->getImgLabel(index);
            int width = currentImgLabel->width();
            int height = currentImgLabel->height();
            printf("label height:%d width:%d!\n",height,width);
            int width1 = pixmap.width();
            int height1 = pixmap.height();
            printf("pixmap height:%d width:%d!\n",height1,width1);
            QPixmap fitpixmap = pixmap.scaled(width, height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
            currentImgLabel->setPixmap(fitpixmap);
        }
    }
}

QString GBK2UTF8(const QString &str)
{
    QTextCodec *utf8 = QTextCodec::codecForName("UTF-8");
    return utf8->toUnicode(str.toUtf8());
}

QString UTF82GBK(const QString &str)
{
    QTextCodec *gbk = QTextCodec::codecForName("GB18030");
    return gbk->toUnicode(str.toLocal8Bit());
}

std::string GBK2UTF8(std::string &str)
{
    QString temp = QString::fromLocal8Bit(str.c_str());
    std::string ret = temp.toUtf8().data();
    return ret;
}

std::string UTF82GBK(std::string &str)
{
    QString temp = QString::fromUtf8(str.c_str());
    std::string ret = temp.toLocal8Bit().data();
    return ret;
}

//生成条码
void MainWindow::encodeBarcodeButtonClicked(int index)
{
    QString barcode = this->getBarcodeLineEdit(index)->text();
    if (barcode.isEmpty()) {
        QMessageBox::information(this,"提示","请输入条形码内容");
    } else {
        QString remark = this->getRemarkLineEdit(index)->text();

        if (!remark.isEmpty()) {
            int currentIndex = ui->tabWidget->currentIndex();
            ui->tabWidget->setTabText(currentIndex,remark);
        }

        QLabel* latestOperateLabel = this->getLatestOperateLabel(index);
        latestOperateLabel->setVisible(false);
        latestOperateLabel->setText(QString::number(BARCODE_CODE128));

        struct zint_symbol *my_symbol = ZBarcode_Create();
        if(my_symbol != NULL)  {
            my_symbol->symbology= BARCODE_CODE128;
            my_symbol->border_width = 2;
            my_symbol->scale = 5;
            my_symbol->input_mode = DATA_MODE;

            QString fileName = cfg->GetConfigPath() +"output.bmp" ;
            QByteArray fileNameBa = fileName.toLatin1();
            strcpy_s(my_symbol->outfile, fileNameBa.data());

            unsigned char* ch;
            QByteArray ba = barcode.toUtf8();
            ch=(unsigned char *)ba.data();
            ZBarcode_Encode(my_symbol,ch,0);
            ZBarcode_Print(my_symbol,0); //
            ZBarcode_Delete(my_symbol);
            QImage* barcodeImg= new QImage();
            barcodeImg->load("output.bmp");
            QPixmap pixmap = QPixmap::fromImage(*barcodeImg);
            QLabel* currentImgLabel =this->getImgLabel(index);
            int width = currentImgLabel->width();
            int height = currentImgLabel->height();
            //height = pixmap.height();
            printf("label height:%d width:%d!\n",height,width);
            int width1 = pixmap.width();
            int height1 = 100;//pixmap.height();
            printf("pixmap height:%d width:%d!\n",height1,width1);
            QPixmap fitpixmap = pixmap.scaled(width, height1, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
            currentImgLabel->setPixmap(fitpixmap);
        }
    }
}

void MainWindow::on_encoderButton_clicked()
{
    encodeQRButtonClicked(0);
}

void MainWindow::on_encoderButton1_clicked()
{
    encodeQRButtonClicked(1);
}

void MainWindow::on_encoderButton2_clicked()
{
    encodeQRButtonClicked(2);
}

void MainWindow::on_encoderButton3_clicked()
{
    encodeQRButtonClicked(3);
}

void MainWindow::on_encoderButton4_clicked()
{
    encodeQRButtonClicked(4);
}

void MainWindow::on_encodeBarcodeBtn_clicked()
{
    encodeBarcodeButtonClicked(0);
}

void MainWindow::on_encodeBarcodeBtn4_clicked()
{
    encodeBarcodeButtonClicked(4);
}

void MainWindow::on_encodeBarcodeBtn3_clicked()
{
    encodeBarcodeButtonClicked(3);
}

void MainWindow::on_encodeBarcodeBtn2_clicked()
{
    encodeBarcodeButtonClicked(2);
}

void MainWindow::on_encodeBarcodeBtn1_clicked()
{
    encodeBarcodeButtonClicked(1);
}
