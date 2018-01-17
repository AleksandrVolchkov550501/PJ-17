#include "ContactListModel.h"
#include <QFile>
#include <QDir>

ContactListModel::ContactListModel(UserProfile * pUserProf, LongPollMgr *pLPM, DataBase *dataBase, QObject *parent)
    : QAbstractListModel(parent)
    , pUserProf(pUserProf)
    , pLPM(pLPM)
    , dataBase(dataBase)
{
    pQNAM = pLPM->getPQNAM();
    connect(pLPM, &LongPollMgr::friendOnOff, this, &ContactListModel::setFriendOnOff);
    connect(dataBase, &DataBase::tableContactsChanged, this, &ContactListModel::updateListContactsFromDB);

//    updateListContactsFromDB();
}

int ContactListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0;
    }
    return listIDs.size();
}

QVariant ContactListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    switch (role) {
    case UserID:
        return QVariant(contactList[listIDs.at(index.row())]->getId());
    case FirstName:
        return QVariant(contactList[listIDs.at(index.row())]->getFirstName());
    case LastName:
        return QVariant(contactList[listIDs.at(index.row())]->getLastName());
    case Online:
        return QVariant(contactList[listIDs.at(index.row())]->getOnline());
    case Photo_50:
        return QVariant(contactList[listIDs.at(index.row())]->getPhoto_50());
    case Status:
        return QVariant(contactList[listIDs.at(index.row())]->getStatus());
    default:
        return QVariant();
    }
}

void ContactListModel::clearContactList()
{
    beginResetModel();
        listIDs.clear();
        contactList.clear();
    endResetModel();
}

bool ContactListModel::updateListContactsFromDB()
{
    clearContactList();
    QList<Person *> listPersons = dataBase->getListFriends();
    if(!listPersons.empty())
    {
        foreach (Person * p, listPersons) {
            add(p);
        }
        return true;
    }
    return false;
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
    beginInsertRows(QModelIndex(), listIDs.size(), listIDs.size());
        person->setPosInCL(listIDs.size());
        contactList[person->getId()] = person;
        listIDs << person->getId();
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

    if(!contactList.contains(id))
        return;

    contactList[id]->setOnline(evNum == VK::LP::EV::FriendOnline);
    contactList[id]->lastSeen.platform = platform;
    contactList[id]->lastSeen.platform = time;

    QModelIndex index = createIndex(contactList[id]->getPosInCL(), 0, static_cast<void *>(0));
    emit dataChanged(index, index);
}

int ContactListModel::getUserIdByCurIndex(int index)
{
    return listIDs[index];
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
