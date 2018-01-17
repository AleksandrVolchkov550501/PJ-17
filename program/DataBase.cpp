#include "DataBase.h"

DataBase::DataBase(UserProfile * pUserProf)
    : pUserProf(pUserProf)
{
//    vkFriendsGet();
}

DataBase::~DataBase()
{
    db.close();
}

QList<Person *> DataBase::getListFriends()
{
    QSqlQuery query;
    if(!query.exec("SELECT * FROM " TABLE_CONTACTS))
    {
        qDebug() << "DataBase: error of read " << TABLE_CONTACTS;
        qDebug() << query.lastError().text();
        return QList<Person * >();
    }
//    qDebug() << "query.size() = "<< query.size() << endl;
//    if(query.size() <= 0)
//    {
//        vkFriendsGet();
//        return QList<Person * >();
//    }

    QList<Person *> listPersons;
    QSqlRecord record = query.record();

    while(query.next())
    {
        Person * person = new Person();

        person->setId              (query.value(record.indexOf( TABLE_CONTACTS_VKID          )).toInt()     );
        person->setFirstName       (query.value(record.indexOf( TABLE_CONTACTS_FIRST_NAME    )).toString()  );
        person->setLastName        (query.value(record.indexOf( TABLE_CONTACTS_LAST_NAME     )).toString()  );
        person->setSex             (query.value(record.indexOf( TABLE_CONTACTS_SEX           )).toInt()     );
        person->setBornDate        (query.value(record.indexOf( TABLE_CONTACTS_BORN_DATE     )).toString()  );
        person->setNickname        (query.value(record.indexOf( TABLE_CONTACTS_NICKNAME      )).toString()  );
        person->setDomain          (query.value(record.indexOf( TABLE_CONTACTS_DOMAIN        )).toString()  );
        person->setStatus          (query.value(record.indexOf( TABLE_CONTACTS_STATUS        )).toString()  );
        person->setFriendStatus    (query.value(record.indexOf( TABLE_CONTACTS_FRIEND_STATUS )).toInt()     );
        person->setPhoto_50        (query.value(record.indexOf( TABLE_CONTACTS_PHOTO_50      )).toString()  );
        person->lastSeen.time     = query.value(record.indexOf( TABLE_CONTACTS_TIME          )).toInt()     ;
        person->lastSeen.platform = query.value(record.indexOf( TABLE_CONTACTS_PLATFORM      )).toInt()     ;

        listPersons.append(person);
    }
    return listPersons;
}

void DataBase::connectToDataBase()
{
    if(!QFile(DATABASE_NAME).exists()){
        this->restoreDataBase();
    } else {
        this->openDataBase();
    }
}

bool DataBase::restoreDataBase()
{
    if(this->openDataBase()){
        if(!this->createContactsTable()){
            return false;
        } else {
            return true;
        }
    } else {
        qDebug() << "Не удалось восстановить базу данных";
        return false;
    }
    return false;
}

bool DataBase::openDataBase()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setHostName(DATABASE_HOSTNAME);
    db.setDatabaseName(DATABASE_NAME);
    if(db.open()){
        return true;
    } else {
        return false;
    }
}

void DataBase::closeDataBase()
{
    db.close();
}

bool DataBase::createContactsTable()
{
    QSqlQuery query;
    if(!query.exec( "CREATE TABLE " TABLE_CONTACTS " ("
                    TABLE_CONTACTS_VKID             " INTEGER PRIMARY KEY NOT NULL      ,"
                    TABLE_CONTACTS_FIRST_NAME       " TEXT                              ,"
                    TABLE_CONTACTS_LAST_NAME        " TEXT                              ,"
                    TABLE_CONTACTS_SEX              " INTEGER                           ,"
                    TABLE_CONTACTS_BORN_DATE        " TEXT                              ,"
                    TABLE_CONTACTS_NICKNAME         " TEXT                              ,"
                    TABLE_CONTACTS_DOMAIN           " TEXT                              ,"
                    TABLE_CONTACTS_STATUS           " TEXT                              ,"
                    TABLE_CONTACTS_FRIEND_STATUS    " INTEGER                           ,"
                    TABLE_CONTACTS_PHOTO_50         " TEXT                              ,"
                    TABLE_CONTACTS_TIME             " INTEGER                           ,"
                    TABLE_CONTACTS_PLATFORM         " INTEGER                           );"
                    ))
    {
        qDebug() << "DataBase: error of create " << TABLE_CONTACTS;
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }
    return false;
}

bool DataBase::insertIntoTableContacts(Person &person)
{
    QSqlQuery query;
    query.prepare("INSERT INTO " TABLE_CONTACTS       " ("
                      TABLE_CONTACTS_VKID             ", "
                      TABLE_CONTACTS_FIRST_NAME       ", "
                      TABLE_CONTACTS_LAST_NAME        ", "
                      TABLE_CONTACTS_SEX              ", "
                      TABLE_CONTACTS_BORN_DATE        ", "
                      TABLE_CONTACTS_NICKNAME         ", "
                      TABLE_CONTACTS_DOMAIN           ", "
                      TABLE_CONTACTS_STATUS           ", "
                      TABLE_CONTACTS_FRIEND_STATUS    ", "
                      TABLE_CONTACTS_PHOTO_50         ", "
                      TABLE_CONTACTS_TIME             ", "
                      TABLE_CONTACTS_PLATFORM         ") "
                  "VALUES ("
                      ":" TABLE_CONTACTS_VKID         ","
                      ":" TABLE_CONTACTS_FIRST_NAME   ","
                      ":" TABLE_CONTACTS_LAST_NAME    ","
                      ":" TABLE_CONTACTS_SEX          ","
                      ":" TABLE_CONTACTS_BORN_DATE    ","
                      ":" TABLE_CONTACTS_NICKNAME     ","
                      ":" TABLE_CONTACTS_DOMAIN       ","
                      ":" TABLE_CONTACTS_STATUS       ","
                      ":" TABLE_CONTACTS_FRIEND_STATUS","
                      ":" TABLE_CONTACTS_PHOTO_50     ","
                      ":" TABLE_CONTACTS_TIME         ","
                      ":" TABLE_CONTACTS_PLATFORM     ");"
                  );
    query.bindValue(":" TABLE_CONTACTS_VKID          , person.getId()            );
    query.bindValue(":" TABLE_CONTACTS_FIRST_NAME    , person.getFirstName()     );
    query.bindValue(":" TABLE_CONTACTS_LAST_NAME     , person.getLastName()      );
    query.bindValue(":" TABLE_CONTACTS_SEX           , person.getSex()           );
    query.bindValue(":" TABLE_CONTACTS_BORN_DATE     , person.getBornDate()      );
    query.bindValue(":" TABLE_CONTACTS_NICKNAME      , person.getNickname()      );
    query.bindValue(":" TABLE_CONTACTS_DOMAIN        , person.getDomain()        );
    query.bindValue(":" TABLE_CONTACTS_STATUS        , person.getStatus()        );
    query.bindValue(":" TABLE_CONTACTS_FRIEND_STATUS , person.getFriendStatus()  );
    query.bindValue(":" TABLE_CONTACTS_PHOTO_50      , person.getPhoto_50()      );
    query.bindValue(":" TABLE_CONTACTS_TIME          , person.lastSeen.time      );
    query.bindValue(":" TABLE_CONTACTS_PLATFORM      , person.lastSeen.platform  );

    if(!query.exec()){
        qDebug() << "error insert into " << TABLE_CONTACTS;
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }
    return false;
}

void DataBase::vkFriendsGet()
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
    connect(pReply, &QNetworkReply::finished, this, &DataBase::vkHandleFriendsGet);
}

void DataBase::vkHandleFriendsGet()
{
    QJsonDocument document = QJsonDocument::fromJson(pReply->readAll());
    pReply->deleteLater();
    QJsonObject response = document.object().value("response").toObject();
    int count = response.value("count").toInt();
    QJsonArray items = response.value("items").toArray();

    qDebug() << "vkHandleFriendsGet()" << endl
              << "document = " << document << endl
              << "response = " << response << endl
              << "count = " << count << endl
              << "items = " << items << endl;

    foreach (QJsonValue jsv, items) {
        QJsonObject joPerson = jsv.toObject();
        Person person;
        person.setId(joPerson.value("id").toInt());
        person.setFirstName(joPerson.value("first_name").toString());
        person.setLastName(joPerson.value("last_name").toString());
        person.setDomain(joPerson.value("domain").toString());
        person.setFriendStatus(joPerson.value("friend_status").toInt());
        person.setBornDate(joPerson.value("bdate").toString());
        person.setNickname(joPerson.value("nickname").toString());
        person.setOnline(joPerson.value("online").toInt());
        person.setPhoto_50(QUrl(joPerson.value("photo_50").toString()));
        person.setSex(joPerson.value("sex").toInt());
        person.setStatus(joPerson.value("status").toString());

        insertIntoTableContacts(person);
    }
    emit tableContactsChanged();
}
