#include "ContactListModel.h"
#include <QFile>
#include <QDir>

ContactListModel::ContactListModel(UserProfile * pUserProf, LongPollMgr *pLPM, QObject *parent)
    : QAbstractListModel(parent)
    , pUserProf(pUserProf)
    , pLPM(pLPM)
{
    pQNAM = pLPM->getPQNAM();
    connect(pLPM, &LongPollMgr::friendOnOff, this, &ContactListModel::setFriendOnOff);
//    QDir dir;
//    dir.mkpath("Profiles/" + pUserProf->getUserId());
    vkFriendsGet();

}

int ContactListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0;
    }

    return m_data.size();
}

QVariant ContactListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    switch (role) {
    case UserID:
        return QVariant(contactList[m_data.at(index.row())]->getId());
    case FirstName:
        return QVariant(contactList[m_data.at(index.row())]->getFirstName());
    case LastName:
        return QVariant(contactList[m_data.at(index.row())]->getLastName());
    case Online:
        return QVariant(contactList[m_data.at(index.row())]->getOnline());
    case Photo_50:
        return QVariant(contactList[m_data.at(index.row())]->getPhoto_50());
    case Status:
        return QVariant(contactList[m_data.at(index.row())]->getStatus());
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> ContactListModel::roleNames() const
{
    QHash<int, QByteArray> roles = QAbstractListModel::roleNames();

    roles[UserID]       = "userID";
    roles[FirstName]    = "firstName";
    roles[LastName]     = "lastName";
    roles[Online]       = "online";
    roles[Photo_50]     = "photo_50";
    roles[Status]       = "status";

    return roles;
}

void ContactListModel::add(Person * person)
{
    beginInsertRows(QModelIndex(), m_data.size(), m_data.size());
        person->setPosInCL(m_data.size());
        contactList[person->getId()] = person;
        m_data << person->getId();
        endInsertRows();
}

Person *ContactListModel::getPerson(int id)
{
    if(contactList.contains(id))
        return contactList[id];
    return nullptr;
}

void ContactListModel::setFriendOnOff(QVariantList varList)
{
    qDebug() << "clmodel : обработка друга онлайн/оффлайн" << endl;
    int evNum = varList[0].toInt();
    int id = qAbs(varList[1].toInt());
    int platform = varList[2].toInt();
    unsigned time = varList[3].toUInt();
    qDebug() << "id = " << id << ", online = " << platform << ", time = " << time << endl;
    contactList[id]->setOnline(evNum == VK::LP::EV::FriendOnline);
    contactList[id]->lastSeen.platform = platform;
    contactList[id]->lastSeen.platform = time;

    QModelIndex index = createIndex(contactList[id]->getPosInCL(), 0, static_cast<void *>(0));
    emit dataChanged(index, index);
}

void ContactListModel::vkFriendsGet()
{
    QUrl url(VK::ApiUrl + "friends.get");
    QUrlQuery query;
    query.addQueryItem("access_token", pUserProf->getAccsesToken());
    query.addQueryItem("v", VK::ApiVersion);
    query.addQueryItem("order", "hints");
    query.addQueryItem("count", "0");
    query.addQueryItem("offset", "0");
    query.addQueryItem("fields", VK::FriendsGetFields);
    url.setQuery(query);
    qDebug() << "url = " << url << endl;

    pReply = PJ::pQNAM->get(QNetworkRequest(url));
    connect(pReply, &QNetworkReply::finished, this, &ContactListModel::vkHandleFriendsGet);
}

void ContactListModel::vkHandleFriendsGet()
{
    QJsonDocument document = QJsonDocument::fromJson(pReply->readAll());
    pReply->deleteLater();
    QJsonObject response = document.object().value("response").toObject();
    int count = response.value("count").toInt();
    QJsonArray items = response.value("items").toArray();
    qDebug () << "vkHandleFriendsGet()" << endl
              << "document = " << document << endl
              << "response = " << response << endl
              << "count = " << count << endl
              << "items = " << items << endl;
    foreach (QJsonValue jsv, items) {
        QJsonObject joPerson = jsv.toObject();
        Person * person = new Person(this);
        person->setId(joPerson.value("id").toInt());
        person->setFirstName(joPerson.value("first_name").toString());
        person->setLastName(joPerson.value("last_name").toString());
        person->setDomain(joPerson.value("domain").toString());
        person->setFriendStatus(joPerson.value("friend_status").toInt());
        person->setBornDate(joPerson.value("bdate").toString());
        person->setNickname(joPerson.value("nickname").toString());
        person->setOnline(joPerson.value("online").toInt());
        person->setPhoto_50(QUrl(joPerson.value("photo_50").toString()));
        person->setSex(joPerson.value("sex").toInt());
        person->setStatus(joPerson.value("status").toString());

//  TODO: закачка картинок работает. Нуожн как-то их подключить в qml-части
//        QString strFileName = person->getPhoto_50().path().section('/', -1);
//        QFile   file("./Profiles/" + pUserProf->getUserId() + "/" + strFileName );
//        qDebug() << "strFileName" << strFileName << endl;
//        if(!file.exists())
//            downloadPhoto(person->getPhoto_50());

//        qDebug()    << person->getId() << endl
//                    << person->getFirstName() << endl
//                    << person->getLastName() << endl
//                    << person->getDomain() << endl
//                    << person->getFriendStatus() << endl
//                    << person->getBornDate() << endl
//                    << person->getNickname() << endl
//                    << person->getOnline() << endl
//                    << person->getPhoto_50() << endl
//                    << person->getSex() << endl
//                    << person->getStatus() << endl;

        add(person);
    }
}

int ContactListModel::getUserIdByCurIndex(int index)
{
    return m_data[index];
}

void ContactListModel::downloadPhoto(QUrl url)
{
    QNetworkRequest request(url);
    QNetworkReply * pReply = PJ::pQNAM->get(request);
    connect(pReply, &QNetworkReply::finished, this, &ContactListModel::savePhoto);
}

void ContactListModel::savePhoto()
{
    qDebug () << "загрузка фото" << endl;
    QNetworkReply * pReply = qobject_cast<QNetworkReply *> (sender());
    QString strFileName = pReply->url().path().section('/', -1);
    QFile   file("./Profiles/" + pUserProf->getUserId() + "/" + strFileName );
    qDebug() << "file path = " << file.symLinkTarget() << endl;
    if (file.open(QIODevice::WriteOnly)) {
        qDebug() << "запись в файл" << endl;
        file.write(pReply->readAll());
        pReply->deleteLater();
        file.close();
    }

}
