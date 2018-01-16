#ifndef PERSON_H
#define PERSON_H

#include "VkApi.h"
#include <QObject>
#include <QUrl>

class Person : public QObject
{
    Q_OBJECT
/* ---- Данные с vk.com ----->*/
    int id;
    QString firstName;
    QString lastName;
    int sex;            /*  1 - женский, 2 - мужской, 0 – не указан */
    QString bornDate;
    QString nickname;
    QString domain;
    QString status;     /*  строка, содержащая текст статуса, расположенного в профиле
                            под именем пользователя. */
    int friendStatus;   /*  является ли другом для пользователя */

    int online;         /*  Находится ли пользователь на сайте. */

    QUrl photo_50;

public:

/*<---- Данные с vk.com -----*/

    int posInCL;        /* позиция в списке модели */

    struct {
        unsigned time;  /* Unixtime */
        int platform;
    } lastSeen;

    explicit Person(QObject *parent = nullptr);

    explicit Person(int posInCL,
                    int userId,
                    QString lastName = QString(),
                    QString firstName = QString(),
                    QObject *parent = nullptr);

signals:

public slots:

    QString getFirstName() const;
    void setFirstName(const QString &value);

    QString getLastName() const;
    void setLastName(const QString &value);

    int getSex() const;
    void setSex(int value);

    QString getBornDate() const;
    void setBornDate(const QString &value);

    QString getNickname() const;
    void setNickname(const QString &value);

    QString getDomain() const;
    void setDomain(const QString &value);

    QString getStatus() const;
    void setStatus(const QString &value);

    int getFriendStatus() const;
    void setFriendStatus(int value);

    int getOnline() const;
    void setOnline(int value);

    QUrl getPhoto_50() const;
    void setPhoto_50(const QUrl &value);

    int getPosInCL() const;
    void setPosInCL(int value);

    int getId() const;
    void setId(int value);

};

#endif // PERSON_H
