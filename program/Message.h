#ifndef MESSAGE_H
#define MESSAGE_H

#include "VkApi.h"
#include <QObject>

class Message : public QObject
{
    Q_OBJECT
    int id;
    int user_id;    /* Сообщение находится в диалоге с этим пользователем */
    unsigned from_id;
    int date;       /* Unixtime */
    int read_state; /* 0 - не прочитано, 1 - прочитано */
    int out;        /* 0 - полученное, 1 - отправленное. Не возвращается для пересланных сообщений */
    QString title;  /* Заголовок сообщения или беседы */
    QString body;   /* Текст сообщения */
    QVariantList attachments;   /* TODO: разобраться в структуре массивов. https://vk.com/dev/objects/attachments_m */
    QVariantList fwd_messages;  /* Массив пересланных сообщений */
    int emoji;      /* Содержатся ли в сообщении emoji-смайлы */
    int important;  /* Является ли важным */
    int deleted;    /* Удалено ли сообщение */
    int random_id;  /* Идентификатор, используемый при отправке сообщения. Возвращается только для исходящих сообщений.*/

    int posInDialogue;

public:
    explicit Message(QObject *parent = nullptr);
    explicit Message(int id, QObject *parent = nullptr);

    int getId() const;
    void setId(int value);

    int getUser_id() const;
    void setUser_id(int value);

    unsigned getFrom_id() const;
    void setFrom_id(const unsigned &value);

    int getDate() const;
    void setDate(int value);

    int getRead_state() const;
    void setRead_state(int value);

    int getOut() const;
    void setOut(int value);

    QString getTitle() const;
    void setTitle(const QString &value);

    QString getBody() const;
    void setBody(const QString &value);

    int getEmoji() const;
    void setEmoji(int value);

    int getImportant() const;
    void setImportant(int value);

    int getDeleted() const;
    void setDeleted(int value);

    int getRandom_id() const;
    void setRandom_id(int value);

    int getPosInDialogue() const;
    void setPosInDialogue(int value);

signals:

public slots:
};

#endif // MESSAGE_H
