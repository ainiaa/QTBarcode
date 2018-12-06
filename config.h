#ifndef CONFIG_H
#define CONFIG_H

#include <QVariant>
#include <QSettings>

class Config
{
public:
    Config(QString qstrfilename = "");
    virtual ~Config(void);
    void Set(QString,QString,QVariant);
    QString GetConfigPath();
    QVariant Get(QString,QString);
    int GetMaxNum();
    void Clear();
private:
    QString fileName;
    QSettings *cfg;
};
#endif // CONFIG_H
