#ifndef SYSTEMTRAY_H
#define SYSTEMTRAY_H

#include <QWidget>
#include <QQuickView>
#include <QSystemTrayIcon>
#include <QMenu>
#include "ContactListModel.h"
#include "LongPollMgr.h"
#include "Message.h"
#include "DialogueModel.h"

class QSystemTrayIcon;
class QMenu;

class ContactList : public QWidget {
Q_OBJECT
private:
    QSystemTrayIcon *   m_ptrayIcon;
    QMenu *             m_ptrayIconMenu;
    ContactListModel *  clmodel;
    LongPollMgr *       pLPM;
    UserProfile *       pUserProf;

    QMap<int, DialogueModel * >  listDialModel;

protected:
    virtual void closeEvent(QCloseEvent*);

public:
    ContactList(UserProfile * pUserProf, LongPollMgr * pLPM, QWidget* parent = nullptr);
    ~ContactList();

public slots:
    void slotShowHide   (QSystemTrayIcon::ActivationReason);
    void slotShowDialog(int user_id);
    void slotNewMessage(QVariantList);
    void startContactList();

};

#endif // SYSTEMTRAY_H
