#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    groupCompent();
    loadConfig();
    creatBtn();
    setFixedSize(this->width(), this->height());
}

MainWindow::~MainWindow()
{
    this->writeConfig();
    delete ui;
    delete cfg;

    qDeleteAll(*remarkLineEditList);
    remarkLineEditList->clear();

    qDeleteAll(*barcodeLineEditList);
    barcodeLineEditList->clear();

    qDeleteAll(*imgLabelList);
    imgLabelList->clear();

    qDeleteAll(*latestOperateLabelList);
    latestOperateLabelList->clear();
}

// 为tabwidget添加按钮
void MainWindow::creatBtn()
{
    QPushButton *addBtn = new QPushButton(tr("添加"));
    QWidget *cornerWidget = new QWidget;
    QHBoxLayout *hlayout = new QHBoxLayout;
    addBtn->setStyleSheet("height:20ex");

    hlayout->addWidget(addBtn);
    cornerWidget->setLayout(hlayout);
    ui->tabWidget->setCornerWidget(cornerWidget,Qt::TopRightCorner);

    connect(addBtn,SIGNAL(clicked()), this,SLOT(addNewTabPage()));
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
    remarkLineEditList->append(ui->remarkLineEdit);
    remarkLineEditList->append(ui->remarkLineEdit1);
    remarkLineEditList->append(ui->remarkLineEdit2);
    remarkLineEditList->append(ui->remarkLineEdit3);
    remarkLineEditList->append(ui->remarkLineEdit4);

    barcodeLineEditList->append(ui->barcodeLineEdit);
    barcodeLineEditList->append(ui->barcodeLineEdit1);
    barcodeLineEditList->append(ui->barcodeLineEdit2);
    barcodeLineEditList->append(ui->barcodeLineEdit3);
    barcodeLineEditList->append(ui->barcodeLineEdit4);

    imgLabelList->append(ui->imgLabel);
    imgLabelList->append(ui->imgLabel1);
    imgLabelList->append(ui->imgLabel2);
    imgLabelList->append(ui->imgLabel3);
    imgLabelList->append(ui->imgLabel4);

    latestOperateLabelList->append(ui->latestOperateLabel);
    latestOperateLabelList->append(ui->latestOperateLabel1);
    latestOperateLabelList->append(ui->latestOperateLabel2);
    latestOperateLabelList->append(ui->latestOperateLabel3);
    latestOperateLabelList->append(ui->latestOperateLabel4);

}
//获得备注控件
QLineEdit* MainWindow::getRemarkLineEdit(int index)
{
    return remarkLineEditList->at(index);
}
//获得条码控件
QLineEdit* MainWindow::getBarcodeLineEdit(int index)
{
    return barcodeLineEditList->at(index);
}
//获得img展示容器
QLabel* MainWindow::getImgLabel(int index)
{
    return imgLabelList->at(index);
}
QLabel* MainWindow::getLatestOperateLabel(int index)
{
    return latestOperateLabelList->at(index);
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
            QLabel* currentImgLabel =this->getImgLabel(index);
            int width = currentImgLabel->width();
            int height = currentImgLabel->height();
            printf("label height:%d width:%d!\n",height,width);
            QPixmap  fitpixmap = pixmap.scaled(height, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);
            currentImgLabel->setPixmap(fitpixmap);
            currentImgLabel->setAlignment(Qt::AlignCenter);
            currentImgLabel->setStyleSheet("background-color:gainsboro");
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
void MainWindow::encodeBarcodeButtonClicked(const int index)
{
    QString barcode = this->getBarcodeLineEdit(index)->text();
    if (barcode.isEmpty()) {
        QMessageBox::information(this,"提示","请输入条形码内容");
    } else {
        QString remark = this->getRemarkLineEdit(index)->text();
        int currentIndex = ui->tabWidget->currentIndex();
        if (!remark.isEmpty()) {

            ui->tabWidget->setTabText(currentIndex,remark);
        }

        QLabel* latestOperateLabel = this->getLatestOperateLabel(index);
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
            QLabel* currentImgLabel =this->getImgLabel(index);
            int width = currentImgLabel->width();
            int height = currentImgLabel->height();
            //height = pixmap.height();
            printf("label height:%d width:%d!\n",height,width);
            int width1 = 429;
            int height1 = 180;//pixmap.height();
            //printf("pixmap height:%d width:%d!\n",height1,width1);
            //currentImgLabel->setPixmap(pixmap);
            QPixmap fitpixmap = pixmap.scaled(width1, height1, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
            currentImgLabel->setPixmap(fitpixmap);
            currentImgLabel->setAlignment(Qt::AlignCenter);
            //ui->tabWidget->setStyleSheet("background-color:gainsboro");//gainsboro  silver ghostwhite
            currentImgLabel->setStyleSheet("background-color:gainsboro");
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

void MainWindow::addNewTabPage()
{
    ui->tabWidget->addTab(this,"new tab");
}

EncoderTab::EncoderTab(QWidget *parent)
{
     QWidget* newTab = new QWidget();
     newTab
}
