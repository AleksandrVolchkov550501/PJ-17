#ifndef DIALOGUEMODEL_H
#define DIALOGUEMODEL_H

#include "VkApi.h"
#include <QAbstractListModel>
#include "LongPollMgr.h"
#include "UserProfile.h"
#include "Message.h"
#include "Person.h"

class DialogueModel : public QAbstractListModel
{
    Q_OBJECT
    int                     user_id;
    QList<int>              m_data;
    QMap<int, Message *>    listMsgs;
    QNetworkAccessManager * pQNAM;
    LongPollMgr *           pLPM;
    UserProfile *           pUserProf;
    Person *                pCompanion;
    QNetworkReply *         pReply;

protected:
    virtual QHash<int, QByteArray> roleNames() const;

public:
    enum Roles {
        ID = Qt::UserRole + 1,
        User_id,
        From_id,
        Date,
        Time,
        Read_state,
        Out,
        Title,
        Body,
        Emoji,
        Important,
        Deleted,
        Random_id
    };

    DialogueModel(Person * рCompanion, UserProfile *pUserProf,
                  LongPollMgr * pLPM, QObject *parent = nullptr);

    virtual int rowCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;

public slots:

    void add(Message *);
    void vkGetHistory();
    void vkHandleGetHistory();
    void vkSendMessage(QString msg);
    void vkHandleSendMessage();

};

#endif // DIALOGUEMODEL_H
