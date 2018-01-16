#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>
#include <QUrlQuery>
#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QFile>
#include <QTime>
#include <QDate>
#include <QList>
#include <QDebug>
#include "Person.h"
#include "UserProfile.h"

#define DATABASE_HOSTNAME   "HostName"
#define DATABASE_NAME       "DataBase.db"

#define TABLE                   "Test"

#define TABLE_DATE              "Date"
#define TABLE_TIME              "Time"
#define TABLE_MESSAGE           "Message"
#define TABLE_RANDOM            "Random"

#define TABLE_CONTACTS                  "Contacts"
#define TABLE_CONTACTS_VKID             "vkID"
#define TABLE_CONTACTS_FIRST_NAME       "FirstName"
#define TABLE_CONTACTS_LAST_NAME        "LastName"
#define TABLE_CONTACTS_SEX              "Sex"
#define TABLE_CONTACTS_BORN_DATE        "BornDate"
#define TABLE_CONTACTS_NICKNAME         "Nickname"
#define TABLE_CONTACTS_DOMAIN           "Domain"
#define TABLE_CONTACTS_STATUS           "Status"
#define TABLE_CONTACTS_FRIEND_STATUS    "FriendStatus"
#define TABLE_CONTACTS_PHOTO_50         "Photo_50"
#define TABLE_CONTACTS_TIME             "Time"
#define TABLE_CONTACTS_PLATFORM         "Platform"


class DataBase : public QObject
{
    Q_OBJECT
    QSqlDatabase    db;
    UserProfile *   pUserProf;
    QNetworkReply * pReply;

public:
    explicit DataBase(UserProfile *pUserProf);
    ~DataBase();

    QList<Person *> getListFriends();

public slots:
    void connectToDataBase();
    bool insertIntoTableContacts(Person &);

private:
    bool openDataBase();
    bool restoreDataBase();
    void closeDataBase();
    bool createContactsTable();

    void vkFriendsGet();

private slots:
    void vkHandleFriendsGet();

signals:
    void tableContactsChanged();

};

#endif // DATABASE_H
