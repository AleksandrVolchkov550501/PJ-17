#ifndef LONGPOLLMGR_H
#define LONGPOLLMGR_H

#include "VkApi.h"
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QUrlQuery>
#include "UserProfile.h"

class LongPollMgr : public QObject
{
    Q_OBJECT
    QNetworkAccessManager * pQNAM;
    UserProfile * pUserProf;
    QString key;
    QString server;
    unsigned ts;
    unsigned pts;
    QNetworkReply * pServerReply;
    bool isReady;

public:
    explicit LongPollMgr(UserProfile * pUserProf, QNetworkAccessManager * pQNAM, QObject *parent = nullptr);


    QNetworkAccessManager *getPQNAM() const;
    void setPQNAM(QNetworkAccessManager *value);

    UserProfile *getPUserProf() const;
    void setPUserProf(UserProfile *value);

    bool getIsReady() const;
    void setIsReady(bool value);

signals:
    void longPollMgrReady();
    void friendOnOff(QVariantList);
    void newMessage(QVariantList);
public slots:
    void getLongPollServer();
    void hrGetLongPollServer();
    void startLongPoll();
    void handleLPResponse();
};

#endif // LONGPOLLMGR_H
