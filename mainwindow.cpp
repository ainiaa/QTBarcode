#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_encoderButton_clicked()
{
    QString barcode = ui->barcodeLineEdit->text();
    if (barcode.isEmpty()) {
        QMessageBox::information(this,"提示","请输入条码/二维码内容");
    } else {
        QString remark = ui->remarkLineEdit->text();

        if (!remark.isEmpty()) {
            int currentIndex = ui->tabWidget->currentIndex();
            ui->tabWidget->setTabText(currentIndex,remark);
        }

        QImage barcodeImg = QZXing::encodeData(barcode);
        ui->imgLabel->setPixmap(QPixmap::fromImage(barcodeImg));
    }
}

void MainWindow::on_encoderButton1_clicked()
{
    QString barcode = ui->barcodeLineEdit1->text();
    if (barcode == "") {
        QMessageBox::information(this,"提示","请输入条码/二维码内容");
    } else {
        QString remark = ui->remarkLineEdit1->text();

        if (!remark.isEmpty()) {
            int currentIndex = ui->tabWidget->currentIndex();
            ui->tabWidget->setTabText(currentIndex,remark);
        }

        QImage barcodeImg = QZXing::encodeData(barcode);
        ui->imgLabel1->setPixmap(QPixmap::fromImage(barcodeImg));
    }
}

void MainWindow::on_encoderButton2_clicked()
{
    QString barcode = ui->barcodeLineEdit2->text();
    if (barcode.isEmpty()) {
        QMessageBox::information(this,"提示","请输入条码/二维码内容");
    } else {
        QString remark = ui->remarkLineEdit2->text();

        if (!remark.isEmpty()) {
            int currentIndex = ui->tabWidget->currentIndex();
            ui->tabWidget->setTabText(currentIndex,remark);
        }

        QImage barcodeImg = QZXing::encodeData(barcode);
        ui->imgLabel2->setPixmap(QPixmap::fromImage(barcodeImg));
    }
}

void MainWindow::on_encoderButton3_clicked()
{
    QString barcode = ui->barcodeLineEdit3->text();
    if (barcode.isEmpty()) {
        QMessageBox::information(this,"提示","请输入条码/二维码内容");
    } else {
        QString remark = ui->remarkLineEdit3->text();

        if (!remark.isEmpty()) {
            int currentIndex = ui->tabWidget->currentIndex();
            ui->tabWidget->setTabText(currentIndex,remark);
        }

        QImage barcodeImg = QZXing::encodeData(barcode);
        ui->imgLabel3->setPixmap(QPixmap::fromImage(barcodeImg));
    }
}

void MainWindow::on_encoderButton4_clicked()
{
    QString barcode = ui->barcodeLineEdit4->text();
    if (barcode.isEmpty()) {
        QMessageBox::information(this,"提示","请输入条码/二维码内容");
    } else {
        QString remark = ui->remarkLineEdit4->text();

        if (!remark.isEmpty()) {
            int currentIndex = ui->tabWidget->currentIndex();
            ui->tabWidget->setTabText(currentIndex,remark);
        }

        QImage barcodeImg = QZXing::encodeData(barcode);
        ui->imgLabel4->setPixmap(QPixmap::fromImage(barcodeImg));
    }
}

void MainWindow::on_encodeBarcodeBtn_clicked()
{
    QString barcode = ui->barcodeLineEdit->text();
    if (barcode.isEmpty()) {
        QMessageBox::information(this,"提示","请输入条码/二维码内容");
    } else {
        QString remark = ui->remarkLineEdit->text();

        if (!remark.isEmpty()) {
            int currentIndex = ui->tabWidget->currentIndex();
            ui->tabWidget->setTabText(currentIndex,remark);
        }

        struct zint_symbol *my_symbol = ZBarcode_Create();
            if(my_symbol != NULL)  {
                printf("Symbol successfully created!\n");
                //QMessageBox::information(this,"提示","Symbol successfully created!\n");
                my_symbol->symbology=BARCODE_CODE128B;
                strcpy_s(my_symbol->outfile, "output.bmp");
                unsigned char* ch;
                QByteArray ba = barcode.toLatin1();
                ch=(unsigned char *)ba.data();
                ZBarcode_Encode(my_symbol,ch,0);
                ZBarcode_Print(my_symbol,0); //
                ZBarcode_Delete(my_symbol);
                QImage* barcodeImg= new QImage;
                barcodeImg->load("output.bmp");
                ui->imgLabel->setPixmap(QPixmap::fromImage(*barcodeImg));
            }

    }
}
