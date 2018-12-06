#include "mainwindow.h"
#include "ui_mainwindow.h"

int MainWindow::tabNum = 0;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qMap.insert("tab", CodeForm(ui->latestOperateLabel,ui->imgLabel,ui->remarkLineEdit,ui->barcodeLineEdit));
    groupCompent();

    //加载数据（如果需要的话，新增tabpage）
    loadConfig();

    //添加新增tab按钮
    creatBtn();

    //关闭tab
    connect(ui->tabWidget,SIGNAL(tabCloseRequested(int)),this,SLOT(removeSubTab(int)));

    setFixedSize(this->width(), this->height());
}

void MainWindow::groupCompent()
{
    remarkLineEditList->append(ui->remarkLineEdit);
    barcodeLineEditList->append(ui->barcodeLineEdit);
    imgLabelList->append(ui->imgLabel);
    latestOperateLabelList->append(ui->latestOperateLabel);
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
    const static QString oldStyle = ui->tabWidget->styleSheet();

    QMainWindow::resizeEvent(event);

    QString style("%1;QTabWidget::tab-bar{width:%2px}");

    QString realStyle = style.arg(oldStyle).arg(this->width());
    //QMessageBox::information(this,"提示",realStyle);
    ui->tabWidget->setStyleSheet(realStyle);
    QString realStyle2 =ui->tabWidget->styleSheet();
    //QMessageBox::information(this,"提示2",realStyle2);
}

//关闭tab
void MainWindow::removeSubTab(int index)
{
    int cnt = ui->tabWidget->count();
    if (cnt > 1)
    {
        ui->tabWidget->removeTab(index);
    }
    else
    {
        QMessageBox::critical(this,"最后一个不允许关闭","最后一个不允许关闭");
    }
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
    addBtn->setStyleSheet("height:20ex;");

    hlayout->addWidget(addBtn);
    cornerWidget->setLayout(hlayout);
    ui->tabWidget->setCornerWidget(cornerWidget,Qt::TopRightCorner);

    connect(addBtn,SIGNAL(clicked()), this,SLOT(addNewTabPage()));
}

void MainWindow::receiveData(QString data)
{
    int currentIndex = ui->tabWidget->currentIndex();
    ui->tabWidget->setTabText(currentIndex,data); //获取传递过来的数据
}


//加载config
void MainWindow::loadConfig()
{
    int max = cfg->GetMaxNum();
    int i;
    if(max > 1)
    {
        for (i =0; i < max-1;i++)
        {
            MainWindow::cloneTabPage();
        }
    }
    for(i =0; i < max;i++)
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
    //清除老数据
    cfg->Clear();
    int cnt = ui->tabWidget->count();
    int max_num = 0;
    for(int i =0; i < cnt;i++)
    {
        QString index = QString::number(i);
        QString barcode = this->getBarcodeLineEdit(i)->text();
        if (!barcode.isEmpty())
        {
            cfg->Set(index,"barcode",barcode);
            QString latestOperate = this->getLatestOperateLabel(i)->text();
            cfg->Set(index,"latestOperate",latestOperate);
            QString remark = this->getRemarkLineEdit(i)->text();
            cfg->Set(index,"remark",remark);
            max_num++;
        }
    }
    cfg->Set("init","max_num",max_num);
}

//生成二维码
void MainWindow::encodeQRButtonClicked(int index)
{
    QString barcode = this->getBarcodeLineEdit(index)->text();
    if (barcode.isEmpty()) {
        QMessageBox::information(this,"提示","请输入二维码内容");
    } else {
        QString remark = this->getRemarkLineEdit(index)->text();

        if (remark.isEmpty()) {
            remark = barcode;
            this->getRemarkLineEdit(index)->setText(remark);
        }
        int currentIndex = ui->tabWidget->currentIndex();
        ui->tabWidget->setTabText(currentIndex,remark);
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

//生成条码
void MainWindow::encodeBarcodeButtonClicked(const int index)
{
    QString barcode = this->getBarcodeLineEdit(index)->text();
    if (barcode.isEmpty()) {
        QMessageBox::information(this,"提示","请输入条形码内容");
    } else {
        QString remark = this->getRemarkLineEdit(index)->text();
        int currentIndex = ui->tabWidget->currentIndex();
        if (remark.isEmpty()) {
            remark = barcode;
            this->getRemarkLineEdit(index)->setText(remark);
        }
        ui->tabWidget->setTabText(currentIndex,remark);

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
            //printf("label height:%d width:%d!\n",height,width);
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

void MainWindow::addNewTabPage()
{
    MainWindow::cloneTabPage();
    return;
    QWidget * tab = new CoderForm(nullptr,cfg);
    ui->tabWidget->addTab(tab,"new tab " +((CoderForm*) tab)->getId());

    ui->tabWidget->setCurrentIndex(ui->tabWidget->count()-1);//新增设置为当前tab页

    connect(tab, SIGNAL(sendData(QString)), this, SLOT(receiveData(QString)));
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

//复制  tabPage 内容
void MainWindow::cloneTabPage()
{
    QString  id = QString::number(++MainWindow::tabNum);
    QString tabName = "tab_"+id;
    QWidget *tab = new QWidget();
    tab->setObjectName(tabName);

    QLabel *codeLabel = new QLabel(tab);
    QLabel *imgLabel = new QLabel(tab);
    QLineEdit *barcodeLineEdit = new QLineEdit(tab);
    QLabel *remarkLabel = new QLabel(tab) ;
    QLineEdit *remarkLineEdit = new QLineEdit(tab) ;
    QLabel *latestOperateLabel= new QLabel(tab);
    QPushButton *encoderButton = new QPushButton(tab);
    QPushButton *encodeBarcodeBtn=new QPushButton(tab);

    codeLabel->setObjectName("codeLabel_"+id);
    codeLabel->setText(ui->codeLabel->text());
    codeLabel->setGeometry(ui->codeLabel->geometry());
    codeLabel->setAlignment(ui->codeLabel->alignment());
    codeLabel->setStyleSheet(ui->codeLabel->styleSheet());

    remarkLineEditList->append(remarkLineEdit);
    barcodeLineEditList->append(barcodeLineEdit);
    imgLabelList->append(imgLabel);
    latestOperateLabelList->append(latestOperateLabel);

    imgLabel->setObjectName("imgLabel_"+id);
    imgLabel->setGeometry(ui->imgLabel->geometry());
    imgLabel->setAlignment(ui->imgLabel->alignment());
    imgLabel->setStyleSheet(ui->imgLabel->styleSheet());

    barcodeLineEdit->setObjectName("barcodeLineEdit_"+id);
    //barcodeLineEdit->setText(ui->barcodeLineEdit->text());
    barcodeLineEdit->setPlaceholderText(ui->barcodeLineEdit->placeholderText());
    barcodeLineEdit->setGeometry(ui->barcodeLineEdit->geometry());
    barcodeLineEdit->setAlignment(ui->barcodeLineEdit->alignment());
    barcodeLineEdit->setStyleSheet(ui->barcodeLineEdit->styleSheet());

    remarkLabel->setObjectName("remarkLabel_"+id);
    remarkLabel->setGeometry(ui->remarkLabel->geometry());
    remarkLabel->setAlignment(ui->remarkLabel->alignment());
    remarkLabel->setStyleSheet(ui->remarkLabel->styleSheet());

    remarkLineEdit->setObjectName("remarkLineEdit_"+id);
    //remarkLineEdit->setText(ui->remarkLineEdit->text());
    remarkLineEdit->setPlaceholderText(ui->remarkLineEdit->placeholderText());
    remarkLineEdit->setGeometry(ui->remarkLineEdit->geometry());
    remarkLineEdit->setAlignment(ui->remarkLineEdit->alignment());
    remarkLineEdit->setStyleSheet(ui->remarkLineEdit->styleSheet());

    latestOperateLabel->setObjectName("latestOperateLabel_"+id);
    //latestOperateLabel->setText(ui->latestOperateLabel->text());
    latestOperateLabel->setGeometry(ui->latestOperateLabel->geometry());
    latestOperateLabel->setAlignment(ui->latestOperateLabel->alignment());
    latestOperateLabel->setStyleSheet(ui->latestOperateLabel->styleSheet());
    latestOperateLabel->setVisible(ui->latestOperateLabel->isVisible());

    encoderButton->setObjectName("encoderButton_"+id);
    encoderButton->setText(ui->encoderButton->text());
    encoderButton->setGeometry(ui->encoderButton->geometry());
    encoderButton->setStyleSheet(ui->encoderButton->styleSheet());

    encodeBarcodeBtn->setObjectName("encodeBarcodeBtn_"+id);
    encodeBarcodeBtn->setText(ui->encodeBarcodeBtn->text());
    encodeBarcodeBtn->setGeometry(ui->encodeBarcodeBtn->geometry());
    encodeBarcodeBtn->setStyleSheet(ui->encodeBarcodeBtn->styleSheet());

    ui->tabWidget->addTab(tab,"New Tab " + id);
    ui->tabWidget->setCurrentIndex(ui->tabWidget->count()-1);//新增设置为当前tab页

    encoderButton->installEventFilter(this);
    encodeBarcodeBtn->installEventFilter(this);

    qMap.insert(tabName, CodeForm(latestOperateLabel,imgLabel,remarkLineEdit,barcodeLineEdit));

    //connect(encoderButton,SIGNAL(clicked()), this,SLOT(on_encoderButton_clicked()));//这种方式会报段错误
    //connect(encodeBarcodeBtn,SIGNAL(clicked()), this,SLOT(on_encodeBarcodeBtn_clicked()));
}

bool MainWindow::eventFilter(QObject *target, QEvent *e)
{
    if(e->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent *ms=static_cast<QMouseEvent*>(e);
        if(ms->button()==Qt::LeftButton)
        {
            QString objectName=target->objectName();
            int currentIndex = ui->tabWidget->currentIndex();
            if (objectName.startsWith("encoderButton_",Qt::CaseSensitive)) {
                this->encodeQRButtonClicked(currentIndex);
            } else if (objectName.startsWith("encodeBarcodeBtn_",Qt::CaseSensitive)) {
                this->encodeBarcodeButtonClicked(currentIndex);
            }
        }
    }

    return QMainWindow::eventFilter(target, e);
}

//生成二维码
void MainWindow::on_encoderButton_clicked()
{
    int currentIndex = ui->tabWidget->currentIndex();
    QWidget* tab = ui->tabWidget->currentWidget();
    QMessageBox::information(this,"提示信息",tab->objectName());
    this->encodeQRButtonClicked(currentIndex);
}

//生成条码
void MainWindow::on_encodeBarcodeBtn_clicked()
{
    int currentIndex = ui->tabWidget->currentIndex();
    QWidget* tab = ui->tabWidget->currentWidget();
    QMessageBox::information(this,"提示信息",tab->objectName());
    this->encodeBarcodeButtonClicked(currentIndex);
}
