#include "ContactList.h"
#include <QtWidgets>
#include <QQuickWidget>
#include <QQmlContext>
#include <QQuickItem>

// ----------------------------------------------------------------------
ContactList::ContactList(UserProfile * pUserProf, LongPollMgr *pLPM, DataBase *dataBase, QWidget* parent /*=0*/)
    : QWidget(parent)
    , pLPM(pLPM)
    , pUserProf(pUserProf)
    , dataBase(dataBase)
{
    if(pLPM->getIsReady() == false)
        connect(pLPM, SIGNAL(longPollMgrReady()), this, SLOT(startContactList()));
    else
        startContactList();
}


void ContactList::startContactList()
{
    disconnect(pLPM, SIGNAL(longPollMgrReady()), this, SLOT(startContactList()));
    connect(pLPM, &LongPollMgr::longPollMgrReady, &LongPollMgr::startLongPoll);

    setWindowTitle("PJ-17");
    setWindowFlags(Qt::Tool | Qt::WindowStaysOnTopHint);

    QSettings qst(QApplication::organizationName(), QApplication::applicationName());
    QRect geom = qst.value("CLGeometry", QRect()).toRect();
    if(geom != QRect())
        setGeometry(geom);
    setContentsMargins(0, 0, 0, 0);
    QQuickWidget* pv = new QQuickWidget();  // NOTE: подумать над заменой на QQuickViw

    QVBoxLayout* pvbx = new QVBoxLayout();
    pvbx->addWidget(pv);
    setLayout(pvbx);

    QQmlContext* pcon = pv->rootContext();
    clmodel = new ContactListModel(pLPM->getPUserProf(), pLPM, dataBase, this);

    pcon->setContextProperty("clmodel", clmodel);
    pcon->setContextProperty("parentWidget", this);
    pv->setSource(QUrl("qrc:///contactList.qml"));

    QAction* pactQuit = new QAction("&Quit", this);
    connect(pactQuit, SIGNAL(triggered()), qApp, SLOT(quit()));

    m_ptrayIconMenu = new QMenu(this);
    m_ptrayIconMenu->addAction(pactQuit);

    m_ptrayIcon = new QSystemTrayIcon(this);
    m_ptrayIcon->setContextMenu(m_ptrayIconMenu);
    m_ptrayIcon->setToolTip(QApplication::applicationName());

    connect(m_ptrayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this,        SLOT(slotShowHide(QSystemTrayIcon::ActivationReason)));

    connect(pLPM, SIGNAL(newMessage(QVariantList)), this, SLOT(slotNewMessage(QVariantList)));

//    slotChangeIcon();

    QString strPixmapName = ":/images/Capsaqua.ico";
    m_ptrayIcon->setIcon(QPixmap(strPixmapName));

    m_ptrayIcon->show();
    show();

    pLPM->startLongPoll();
}


ContactList::~ContactList()
{
    QSettings qst(QApplication::organizationName(), QApplication::applicationName());
    qst.setValue("CLGeometry", geometry());
}

/*virtual*/void ContactList::closeEvent(QCloseEvent*)
{
    if (m_ptrayIcon->isVisible()) {
        hide();
    }
}

void ContactList::slotShowHide(QSystemTrayIcon::ActivationReason reason)
{
    if(reason == QSystemTrayIcon::Trigger)
            setVisible(!isVisible());
}

void ContactList::slotShowDialog(int user_id)
{
    qDebug() << "userId = " << user_id << endl;
    if(listDialModel.contains(user_id) == false)
    {
        Person * pPers = clmodel->getPerson(user_id);
        DialogueModel * pDial = new DialogueModel(pPers, pUserProf, pLPM, this);
        pDial->vkGetHistory();
        listDialModel[user_id] = pDial;
    }

    QQuickView * pv = new QQuickView();
    QQmlContext* pcon = pv->rootContext();
    pcon->setContextProperty("dialModel", listDialModel[user_id]);
    pcon->setContextProperty("companion", clmodel->getPerson(user_id));
    pv->setSource(QUrl("qrc:/dialogue.qml"));
    pv->show();
    connect(pv, SIGNAL(closing(QQuickCloseEvent*)), pv, SLOT(deleteLater()));

    connect(pv->rootObject(),       SIGNAL(sendMessage(QString)),
            listDialModel[user_id], SLOT(vkSendMessage(QString)));
}

void ContactList::slotNewMessage(QVariantList varList)
{
    int msgFlags = varList[2].toInt();
    qDebug() << "flags = " << msgFlags << endl;
    if(msgFlags & VK::LP::MsgFlg::GroupChat)
        return;

    Message * msg = new Message();

    msg->setId(varList[1].toInt());

    int user_id = varList[3].toInt();
    msg->setUser_id(user_id);

    if(msgFlags & VK::LP::MsgFlg::Outbox)
        msg->setFrom_id(pUserProf->getUserId().toInt());
    else
        msg->setFrom_id(user_id);

    QString msgBody = varList[5].toString();

    msg->setBody(msgBody);
    msg->setDate(varList[4].toUInt());

    if(listDialModel.contains(user_id) == false)
    {
        Person * pPers = clmodel->getPerson(user_id);
        DialogueModel * pDial = new DialogueModel(pPers, pUserProf, pLPM, this);
        pDial->vkGetHistory();
        listDialModel[user_id] = pDial;
    }

    listDialModel[user_id]->add(msg);

    qDebug() << "id = "   << msg->getId()   << endl
             << "user_id" << msg->getUser_id()   << endl
             << "body = " << msg->getBody()   << endl
             << "date = " << msg->getDate()   << endl;

    if(!(msgFlags & VK::LP::MsgFlg::Outbox))
    {
        m_ptrayIcon->showMessage(QApplication::applicationName(), msgBody,
                                    QSystemTrayIcon::Information, 3000);
    }

}
