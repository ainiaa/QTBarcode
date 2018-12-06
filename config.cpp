#include "config.h"
#include <QtCore/QtCore>
#include <QDir>

Config::Config(QString qstrfilename)
{
    if (qstrfilename.isEmpty())
    {
        fileName = this->GetConfigPath()+ "Config.ini";
        printf("fileName:%s", fileName.toUtf8().data());
    }
    else
    {
        fileName = this->GetConfigPath() + qstrfilename;
    }

    cfg = new QSettings(fileName, QSettings::IniFormat);
    cfg->setIniCodec("UTF8");
}
Config::~Config()
{
    delete cfg;
    fileName = nullptr;
}
void Config::Set(QString qstrnodename,QString qstrkeyname,QVariant qvarvalue)
{
    cfg->setValue(QString("/%1/%2").arg(qstrnodename).arg(qstrkeyname), qvarvalue);
}

QVariant Config::Get(QString qstrnodename,QString qstrkeyname)
{
    QVariant qvar = cfg->value(QString("/%1/%2").arg(qstrnodename).arg(qstrkeyname));
    return qvar;
}

int Config::GetMaxNum()
{
    QVariant qvar = cfg->value("/init/max_num");
    return qvar.toInt();
}

QString Config::GetConfigPath()
{
    return QDir::homePath()+ "/.barcode/";
}
void Config::Clear()
{
    cfg->clear();
}
