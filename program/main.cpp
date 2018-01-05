#include "VkApi.h"
#include <QApplication>
#include <QtWidgets>
#include <QQmlApplicationEngine>
#include <qtwebengineglobal.h>
#include "UserProfile.h"
#include "LongPollMgr.h"
#include "ContactList.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setOrganizationName("Lexanni handicraft");
    QCoreApplication::setApplicationName("PJ-17");

    QApplication app(argc, argv);
    QApplication::setQuitOnLastWindowClosed(false);

    UserProfile * pUserProf = new UserProfile();
    LongPollMgr * pLPM = new LongPollMgr(pUserProf, PJ::pQNAM);

    ContactList contactList(pUserProf, pLPM);

    // QQmlApplicationEngine engine;
    // engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
