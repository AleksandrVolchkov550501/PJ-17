#include "DialogueModel.h"

DialogueModel::DialogueModel(Person * pPers, UserProfile *pUserProf, LongPollMgr *pLPM, QObject *parent)
    : QAbstractListModel(parent)
    , pUserProf(pUserProf)
    , pLPM(pLPM)
    , pCompanion(pPers)
{
    user_id = pPers->getId();
    pQNAM = pLPM->getPQNAM();
//    qDebug() << "Создался экземпляр DialogueModel " << endl;
}

int DialogueModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0;
    }

    return m_data.size();
}

QVariant DialogueModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }
    int msgId = m_data.at(index.row());
    switch (role) {
    case Date:
    {
        QDateTime dt;
        dt.setTime_t(listMsgs[msgId]->getDate());
        return QVariant(dt.toString("d:M:yyyy"));
    }
    case Time:
    {
        QDateTime dt;
        dt.setTime_t(listMsgs[msgId]->getDate());
        return QVariant(dt.toString("h:mm:ss"));
    }
    case ID:
        return QVariant(listMsgs[msgId]->getId());
    case User_id:
        return QVariant(listMsgs[msgId]->getUser_id());
    case From_id:
        return QVariant(listMsgs[msgId]->getFrom_id());
    case Read_state:
        return QVariant(listMsgs[msgId]->getRead_state());
    case Out:
        return QVariant(listMsgs[msgId]->getOut());
    case Title:
        return QVariant(listMsgs[msgId]->getTitle());
    case Body:
        return QVariant(listMsgs[msgId]->getBody());
    case Emoji:
        return QVariant(listMsgs[msgId]->getEmoji());
    case Important:
        return QVariant(listMsgs[msgId]->getImportant());
    case Deleted:
        return QVariant(listMsgs[msgId]->getDeleted());
    case Random_id:
        return QVariant(listMsgs[msgId]->getRandom_id());
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> DialogueModel::roleNames() const
{
    QHash<int, QByteArray> roles = QAbstractListModel::roleNames();

    roles[ID]           = "id";
    roles[User_id]      = "user_id";
    roles[From_id]      = "from_id";
    roles[Date]         = "date";
    roles[Time]         = "time";
    roles[Read_state]   = "read_state";
    roles[Out]          = "out";
    roles[Title]        = "title";
    roles[Body]         = "body";
    roles[Emoji]        = "emoji";
    roles[Important]    = "important";
    roles[Deleted]      = "deleted";
    roles[Random_id]    = "random_id";

    return roles;
}

void DialogueModel::add(Message * msg)
{
    foreach (Message * p, listMsgs) {
        p->setPosInDialogue(p->getPosInDialogue()+1);
    }
    beginInsertRows(QModelIndex(), 0, 0);
        msg->setPosInDialogue(0);
        msg->setParent(this);
        listMsgs[msg->getId()] = msg;
        m_data.prepend(msg->getId());
    endInsertRows();
}

void DialogueModel::vkGetHistory()
{
    QUrl url(VK::ApiUrl + "messages.getHistory");
    QUrlQuery query;
    query.addQueryItem("access_token", pUserProf->getAccsesToken());
    query.addQueryItem("v", VK::ApiVersion);
    query.addQueryItem("count", "20");
    query.addQueryItem("rev", "0");     /* В обратном хронологическом порядке */
    query.addQueryItem("user_id", QString::number(user_id));
    url.setQuery(query);

//    qDebug() << "url = " << url << endl;

    pReply = PJ::pQNAM->get(QNetworkRequest(url));
    connect(pReply, &QNetworkReply::finished, this, &DialogueModel::vkHandleGetHistory);
}

void DialogueModel::vkHandleGetHistory()
{
    QJsonDocument document = QJsonDocument::fromJson(pReply->readAll());
    pReply->deleteLater();
    QJsonObject response = document.object().value("response").toObject();
    QJsonArray items = response.value("items").toArray();

//    qDebug () << "vkHandleGetHistory()" << endl
//              << "document = " << document << endl << endl
//              << "response = " << response << endl;

    for(int i = items.size()-1; i >= 0; i--)
    {
        QJsonObject joMsg = items.at(i).toObject();
        Message * msg = new Message(this);
        msg->setBody(joMsg.value("body").toString());
        msg->setDate(joMsg.value("date").toVariant().toUInt());
        msg->setEmoji(joMsg.value("emoji").toInt());
        msg->setFrom_id(joMsg.value("from_id").toVariant().toUInt());   // NOTE: разберись с приведением типов
        msg->setId(joMsg.value("id").toInt());
        msg->setOut(joMsg.value("out").toInt());
        msg->setRandom_id(joMsg.value("random_id").toInt());
        msg->setRead_state(joMsg.value("read_state").toInt());
        msg->setUser_id(joMsg.value("user_id").toInt());

        QDateTime myDateTime;
        myDateTime.setTime_t(msg->getDate());

        qDebug()
                  << "id"         << "= " << msg->getId() << endl
                  << "user_id"    << "= " << msg->getUser_id()    << endl
                  << "from_id"    << "= " << msg->getFrom_id()    << endl
                  << "date"       << "= " << myDateTime   << endl
                  << "read_state" << "= " << msg->getRead_state()    << endl
                  << "out"        << "= " << msg->getOut()    << endl
                  << "title"      << "= " << msg->getTitle()    << endl
                  << "body"       << "= " << msg->getBody()    << endl
                  << "emoji"      << "= " << msg->getEmoji()    << endl
                  << "important"  << "= " << msg->getImportant()    << endl
                  << "deleted"    << "= " << msg->getDeleted()    << endl
                  << "random_id"  << "= " << msg->getRandom_id()    << endl;
        add(msg);
    }

}

void DialogueModel::vkSendMessage(QString msg)
{
    QUrl url(VK::ApiUrl + "messages.send");
    QUrlQuery query;
    query.addQueryItem("access_token", pUserProf->getAccsesToken());
    query.addQueryItem("v", VK::ApiVersion);
    query.addQueryItem("message", msg);
    query.addQueryItem("user_id", QString::number(user_id));
    url.setQuery(query);

    qDebug() << "Отправка сообщения… " << endl
             << "url = " << url << endl;

    pReply = PJ::pQNAM->get(QNetworkRequest(url));
    connect(pReply, &QNetworkReply::finished, this, &DialogueModel::vkHandleSendMessage);
}

void DialogueModel::vkHandleSendMessage()
{
    QJsonDocument document = QJsonDocument::fromJson(pReply->readAll());
    pReply->deleteLater();
    QJsonObject response = document.object().value("response").toObject();

    qDebug () << "vkHandleSendMessage()" << endl
              << "document = " << document << endl << endl
              << "response = " << response << endl;
}

