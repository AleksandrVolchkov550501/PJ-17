#include "Message.h"

int Message::getId() const
{
    return id;
}

void Message::setId(int value)
{
    id = value;
}

int Message::getUser_id() const
{
    return user_id;
}

void Message::setUser_id(int value)
{
    user_id = value;
}

unsigned Message::getFrom_id() const
{
    return from_id;
}

void Message::setFrom_id(const unsigned &value)
{
    from_id = value;
}

int Message::getDate() const
{
    return date;
}

void Message::setDate(int value)
{
    date = value;
}

int Message::getRead_state() const
{
    return read_state;
}

void Message::setRead_state(int value)
{
    read_state = value;
}

int Message::getOut() const
{
    return out;
}

void Message::setOut(int value)
{
    out = value;
}

QString Message::getTitle() const
{
    return title;
}

void Message::setTitle(const QString &value)
{
    title = value;
}

QString Message::getBody() const
{
    return body;
}

void Message::setBody(const QString &value)
{
    body = value;
}

int Message::getEmoji() const
{
    return emoji;
}

void Message::setEmoji(int value)
{
    emoji = value;
}

int Message::getImportant() const
{
    return important;
}

void Message::setImportant(int value)
{
    important = value;
}

int Message::getDeleted() const
{
    return deleted;
}

void Message::setDeleted(int value)
{
    deleted = value;
}

int Message::getRandom_id() const
{
    return random_id;
}

void Message::setRandom_id(int value)
{
    random_id = value;
}

Message::Message(int id, QObject *parent)
    : QObject(parent)
    , id(id)
{

}

int Message::getPosInDialogue() const
{
    return posInDialogue;
}

void Message::setPosInDialogue(int value)
{
    posInDialogue = value;
}

Message::Message(QObject *parent)
    : QObject(parent)
{
    
}
