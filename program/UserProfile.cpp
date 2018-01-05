#include "VkApi.h"
#include "UserProfile.h"
#include <QtWebEngine>
#include <qtwebengineglobal.h>
#include <QtWidgets>
#include <QQuickWidget>
#include <QQmlContext>
#include <QVBoxLayout>
#include <QUrlQuery>
#include <QDebug>

bool UserProfile::getIsReady() const
{
    return isReady;
}

void UserProfile::setIsReady(bool value)
{
    isReady = value;
    emit ready();
}

QString UserProfile::getUserId() const
{
    return userId;
}

void UserProfile::setUserId(const QString &value)
{
    userId = value;
}

UserProfile::UserProfile(QObject *parent) : QObject(parent)
{
    setIsReady(false);
    readSettings();
    if(userId == "")
        showUserAuthDialog();
    else
    {
        setIsReady(true);
    }
}

UserProfile::~UserProfile()
{
    writeSettings();  // FIXME
}

QString UserProfile::getAccsesToken() const
{
    return accessToken;
}

void UserProfile::setAccsesToken(const QString &value)
{
    accessToken = value;
}

void UserProfile::parseResponseUrl(QUrl value)
{
    QUrlQuery q(value.fragment());
    accessToken = q.queryItemValue("access_token");
    userId = q.queryItemValue("user_id");
    qDebug() << "accessToken = " << accessToken << endl;
    writeSettings();
    setIsReady(true);
//    emit ready();
}

void UserProfile::showUserAuthDialog()
{
    // TODO: возможно, это все надо сделать отдельным процессом
    // чтобы QtWebEngine убился.
    QtWebEngine::initialize();
    QQuickView* pv = new QQuickView();
    QQmlContext* pcon = pv->rootContext();
    pcon->setContextProperty("usrProf", this);
    pv->setSource(QUrl("qrc:///authorization.qml"));
    pv->show();
    connect(pv, SIGNAL(closing(QQuickCloseEvent*)), pv, SLOT(deleteLater()));
}

void UserProfile::readSettings()
{
    QSettings pAppSett(QCoreApplication::organizationName(),
                       QCoreApplication::applicationName());
    userId = pAppSett.value("Profiles/activeProfile", QString()).toString();
    accessToken = pAppSett.value("Profiles/" + userId + "/accessToken", "").toString();
}

void UserProfile::writeSettings()
{
    QSettings pAppSett(QCoreApplication::organizationName(),
                       QCoreApplication::applicationName());
    pAppSett.beginGroup("/Profiles");
        // WARNING: здесь возможно сохранение неверного активного профиля, если профилей много
        pAppSett.setValue("activeProfile", userId);
        pAppSett.setValue(userId + "/accessToken", accessToken);
        pAppSett.setValue(userId + "/userId", userId);
    pAppSett.endGroup();
}
