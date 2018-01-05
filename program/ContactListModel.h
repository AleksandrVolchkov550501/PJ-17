#ifndef CONTACTLISTMODEL_H
#define CONTACTLISTMODEL_H

#include "VkApi.h"
#include <QAbstractListModel>
#include <QStringList>
#include "Person.h"
#include "LongPollMgr.h"
#include "UserProfile.h"

class ContactListModel: public QAbstractListModel
{
    Q_OBJECT
    QList<int>              m_data;
    QMap<int, Person *>     contactList;
    QNetworkAccessManager * pQNAM;
    LongPollMgr *           pLPM;
    UserProfile *           pUserProf;
    QNetworkReply *         pReply;


protected:
    virtual QHash<int, QByteArray> roleNames() const;

public:
    enum Roles {
        UserID = Qt::UserRole + 1,
        FirstName,
        LastName,
        Online,
        Photo_50,
        Status
    };

    ContactListModel(UserProfile *pUserProf, LongPollMgr * pLPM, QObject *parent = nullptr);

    virtual int rowCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;

public slots:
    void add(Person *person);
    Person * getPerson(int id);
    void setFriendOnOff(QVariantList varList);
    void vkFriendsGet();
    void vkHandleFriendsGet();
    int getUserIdByCurIndex(int index);
    void downloadPhoto(QUrl url);
    void savePhoto();

};


#endif // CONTACTLISTMODEL_H
