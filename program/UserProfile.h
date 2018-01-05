#ifndef USERPROFILE_H
#define USERPROFILE_H

#include "VkApi.h"
#include <QObject>
#include <QUrl>
#include <QUrlQuery>

class UserProfile : public QObject
{
    Q_OBJECT
    QString accessToken;
    QString userId;
    bool isReady;
public:
    explicit UserProfile(QObject *parent = nullptr);
    ~UserProfile();

    QString getAccsesToken() const;
    void setAccsesToken(const QString &value);

    bool getIsReady() const;
    void setIsReady(bool value);

    QString getUserId() const;
    void setUserId(const QString &value);

signals:
    //    void authFinished();    // NOTE: возможно, этот сигнал не нужен
    void notReady();
    void ready();
public slots:
    void parseResponseUrl(QUrl value);
    void showUserAuthDialog();
    void readSettings();
    void writeSettings();
};

#endif // USERPROFILE_H
