#include "LongPollMgr.h"

QNetworkAccessManager *LongPollMgr::getPQNAM() const
{
    return pQNAM;
}

void LongPollMgr::setPQNAM(QNetworkAccessManager *value)
{
    pQNAM = value;
}

UserProfile *LongPollMgr::getPUserProf() const
{
    return pUserProf;
}

void LongPollMgr::setPUserProf(UserProfile *value)
{
    pUserProf = value;
}

bool LongPollMgr::getIsReady() const
{
    return isReady;
}

void LongPollMgr::setIsReady(bool value)
{
    isReady = value;
}

LongPollMgr::LongPollMgr(UserProfile *pUserProf, QNetworkAccessManager *pQNAM, DataBase *dataBase, QObject *parent):
    pQNAM(pQNAM),
    pUserProf(pUserProf),
    dataBase(dataBase),
    QObject(parent)
{
    isReady = false;
//    connect(this, &LongPollMgr::longPollMgrReady, &LongPollMgr::startLongPoll);
    if(pUserProf->getIsReady() == false)
        connect(pUserProf, &UserProfile::ready, this, &LongPollMgr::getLongPollServer);
    else
        getLongPollServer();
}

void LongPollMgr::getLongPollServer()
{
    qDebug() << "getLongPollServer()" << endl;
    QUrl url(VK::ApiUrl + "messages.getLongPollServer");
    QUrlQuery query;
    query.addQueryItem("v", VK::ApiVersion);
    query.addQueryItem("access_token", pUserProf->getAccsesToken());
    query.addQueryItem("need_pts", "1");
    url.setQuery(query);
    qDebug() << "запрос инициализации: " << url << endl;
    QNetworkRequest request(url);
    pServerReply = pQNAM->get(request);
    connect(pServerReply, &QNetworkReply::finished, this, &LongPollMgr::hrGetLongPollServer);
}

void LongPollMgr::hrGetLongPollServer()
{
    qDebug() << "hrGetLongPollServer()" << endl;
    QJsonDocument document = QJsonDocument::fromJson(pServerReply->readAll());
    pServerReply->deleteLater();
    QJsonObject response = document.object().value("response").toObject();
    key = response.value("key").toString();
    server = response.value("server").toString();
    ts = response.value("ts").toVariant().toUInt();
    pts = response.value("pts").toVariant().toUInt();
    disconnect(pServerReply, &QNetworkReply::finished, this, &LongPollMgr::hrGetLongPollServer);
//    connect(this, SIGNAL(longPollMgrReady()), this, SLOT(startLongPoll()));
    qDebug() << "document = " << document << endl
             << "key = " << key << endl
             << "server = " << server << endl
             << "ts = " << ts << endl
             << "pts = " << pts << endl;

    isReady = true;

    emit longPollMgrReady();
}

/* https://{$server}?act=a_check&key={$key}&ts={$ts}&wait=25&mode=2&version=2 */
void LongPollMgr::startLongPoll()
{
    QUrlQuery query;
    query.addQueryItem("act", "a_check");
    query.addQueryItem("key", key);
    query.addQueryItem("ts", QString::number(ts));
    query.addQueryItem("wait", QString::number(VK::LP::Wait));
    query.addQueryItem("mode", QString::number(VK::LP::mode::Attach |
                                               VK::LP::mode::Pts));
    query.addQueryItem("version", QString::number(VK::LP::Version));
    QUrl url("https://" + server);
    url.setQuery(query);
    qDebug() << "запрос для сервера: " << url << endl;
    pServerReply = pQNAM->get(QNetworkRequest(url));
    connect(pServerReply, &QNetworkReply::finished, this, &LongPollMgr::handleLPResponse);

}

void LongPollMgr::handleLPResponse()
{
    QJsonDocument document = QJsonDocument::fromJson(pServerReply->readAll());
    pServerReply->deleteLater();
    ts = document.object().value("ts").toVariant().toUInt();
    pts = document.object().value("pts").toVariant().toUInt();
    QJsonArray updates = document.object().value("updates").toArray();
    int failed = document.object().value("failed").toInt();

    qDebug()  << "Ответ LP-сервера:" << endl
            << "document = " << document << endl
             << "updates = " << updates << endl
             << "ts = " << ts << endl
             << "pts = " << pts << endl
             << "failed = " << failed << endl;

    if(failed)
        switch (failed) {
        case 1:
            emit longPollMgrReady();
            return;
        case 2:
        case 3:
            getLongPollServer();
            return;
        default:
            break;
        }
    if(updates.isEmpty())
    {
        emit longPollMgrReady();
        return;
    }

    qDebug() << "type of updates[0] = " << updates[0] << endl;

    foreach (QJsonValue jval, updates) {
        QJsonArray ja = jval.toArray();
        qDebug() << ja << endl;
        QVariantList varList = ja.toVariantList();
        qDebug() << varList << endl;
        switch (varList[0].toInt())
        {
            case VK::LP::EV::NewMessage:
                emit newMessage(varList);
                break;
            case VK::LP::EV::FriendOnline:
            case VK::LP::EV::FriendOffline:
                emit friendOnOff(varList);
                break;
            default:
                break;
        }
    }

    emit longPollMgrReady();
}

