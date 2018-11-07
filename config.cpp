#include "config.h"
#include <QtCore/QtCore>
#include <QDebug>
#include <QDir>

Config::Config(QString qstrfilename)
{
    if (qstrfilename.isEmpty())
    {
        m_qstrFileName = QDir::homePath()+ "/.barcode/Config.ini";
        printf("fileName:%s", m_qstrFileName.toUtf8().data());
    }
    else
    {
        m_qstrFileName = qstrfilename;
    }

    m_psetting = new QSettings(m_qstrFileName, QSettings::IniFormat);
    m_psetting->setIniCodec("UTF8");
    qDebug() << m_qstrFileName;
}
Config::~Config()
{
    delete m_psetting;
    m_psetting = 0;
}
void Config::Set(QString qstrnodename,QString qstrkeyname,QVariant qvarvalue)
{
    m_psetting->setValue(QString("/%1/%2").arg(qstrnodename).arg(qstrkeyname), qvarvalue);
}

QVariant Config::Get(QString qstrnodename,QString qstrkeyname)
{
    QVariant qvar = m_psetting->value(QString("/%1/%2").arg(qstrnodename).arg(qstrkeyname));
    return qvar;
}
