#ifndef CONTACTLISTMODEL_H
#define CONTACTLISTMODEL_H

#include "VkApi.h"
#include <QAbstractListModel>
#include <QStringList>
#include <QList>
#include "Person.h"
#include "LongPollMgr.h"
#include "UserProfile.h"
#include "DataBase.h"

class ContactListModel: public QAbstractListModel
{
    Q_OBJECT
    QList<int>              listIDs;
    QMap<int, Person *>     contactList;
    QNetworkAccessManager * pQNAM;
    LongPollMgr *           pLPM;
    UserProfile *           pUserProf;
    DataBase *              dataBase;
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

    ContactListModel(UserProfile *  pUserProf,
                     LongPollMgr *  pLPM,
                     DataBase *     dataBase,
                     QObject *parent = nullptr);

    virtual int rowCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;

    void clearContactList();

public slots:
    bool updateListContactsFromDB();
    void vkGetOnlineFriends();
    void vkHandleGetOnlineFriends();
    void add(Person *person);
    Person * getPerson(int id);
    void setFriendOnOff(QVariantList varList);
    int getUserIdByCurIndex(int index);
    void downloadPhoto(QUrl url);
    void savePhoto();
signals:
    void newPerson(Person *);
};


#endif // CONTACTLISTMODEL_H
