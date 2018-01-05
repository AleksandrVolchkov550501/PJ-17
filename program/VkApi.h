#ifndef VKAPI_H
#define VKAPI_H

#include <QString>
#include <QUrl>
#include <QUrlQuery>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

namespace VK {
    const QString ApiUrl = "https://api.vk.com/method/";
    const QString ApiVersion = "5.65";
    const QString FriendsGetFields =
            "sex,"
            "born_date,"
            "nickname,"
            "domain,"
            "status,"
            "friend_status,"
            "online,"
            "photo_50,"
            "last_seen";
    namespace LP {
        const int Wait = 25;
        const int Version = 2;
        namespace mode {
            const int Attach = 2;
            const int ExtendEvents = 8;
            const int Pts = 32;
            const int Platform = 64;
            const int RandomId = 128;
        }
        /* Флаги событий */
        namespace EV {
            const int NewMessage = 4;
            const int FriendOnline = 8;
            const int FriendOffline = 9;
        }
        /* Флаги сообщений */
        namespace MsgFlg {
            const int  Unread	 =  1	   ;  // сообщение не прочитано
            const int  Outbox	 =  2	   ;  // исходящее сообщение
            const int  Replied	 =  4	   ;  // на сообщение был создан ответ
            const int  Important =  8	   ;  // помеченное сообщение
            const int  Chat	     =  16	   ;  // сообщение отправлено через чат (Возможно, в ВК что-то напутали)
            const int  Friends	 =  32	   ;  // сообщение отправлено другом. Не применяется для сообщений из групповых бесед.
            const int  Spam	     =  64	   ;  // сообщение помечено как "Спам"
            const int  Delеtеd	 =  128	   ;  // сообщение удалено (в корзине)
            const int  Fixed	 =  256	   ;  // сообщение проверено пользователем на спам
            const int  Media	 =  512	   ;  // сообщение содержит медиаконтент
            const int  GroupChat =  8192   ;  // сообщение из группового чата
            const int  Hidden	 =  65536  ;  /* приветственное сообщение от сообщества. Диалог с таким сообщением
                                                не нужно поднимать в списке (отображать его только при открытии диалога
                                                напрямую). Флаг недоступен для версий <2. */
        }
    }
}

namespace PJ {
    QNetworkAccessManager * const pQNAM = new QNetworkAccessManager();
}

#endif // VKAPI_H
