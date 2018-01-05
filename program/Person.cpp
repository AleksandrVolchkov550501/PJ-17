#include "Person.h"

QString Person::getFirstName() const
{
    return firstName;
}

void Person::setFirstName(const QString &value)
{
    firstName = value;
}

QString Person::getLastName() const
{
    return lastName;
}

void Person::setLastName(const QString &value)
{
    lastName = value;
}

int Person::getSex() const
{
    return sex;
}

void Person::setSex(int value)
{
    sex = value;
}

QString Person::getBornDate() const
{
    return bornDate;
}

void Person::setBornDate(const QString &value)
{
    bornDate = value;
}

QString Person::getNickname() const
{
    return nickname;
}

void Person::setNickname(const QString &value)
{
    nickname = value;
}

QString Person::getDomain() const
{
    return domain;
}

void Person::setDomain(const QString &value)
{
    domain = value;
}

QString Person::getStatus() const
{
    return status;
}

void Person::setStatus(const QString &value)
{
    status = value;
}

int Person::getFriendStatus() const
{
    return friendStatus;
}

void Person::setFriendStatus(int value)
{
    friendStatus = value;
}

int Person::getOnline() const
{
    return online;
}

void Person::setOnline(int value)
{
    online = value;
}

QUrl Person::getPhoto_50() const
{
    return photo_50;
}

void Person::setPhoto_50(const QUrl &value)
{
    photo_50 = value;
}

int Person::getPosInCL() const
{
    return posInCL;
}

void Person::setPosInCL(int value)
{
    posInCL = value;
}

void Person::vkReqPersonData()
{
// FIXME
//    QUrl url(VK::ApiUrl + "users.get");
//    QUrlQuery query;
//    query.addQueryItem("v", VK::ApiVersion);
//    query.addQueryItem("user_ids", id);
//    query.addQueryItem("fields", VK::UserGetFields);
}

int Person::getId() const
{
    return id;
}

void Person::setId(int value)
{
    id = value;
}

Person::Person(QObject *parent) : QObject(parent)
{
    
}

Person::Person(int posInCL, int userId, QString lastName, QString firstName, QObject *parent) :
    QObject(parent),
    posInCL(posInCL),
    id(userId),
    firstName(firstName),
    lastName(lastName)
{
}
