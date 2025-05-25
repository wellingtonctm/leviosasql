#include "sessionlistwidget.h"

SessionListWidget::SessionListWidget(QWidget *parent)
    : QListWidget(parent)
{
    sessionManager_ = new SessionManagerCore(this);

    QListWidgetItem* newSessionItem = new QListWidgetItem("New Session", this);
    newSessionItem->setIcon(QIcon::fromTheme("document-new"));
    addItem(newSessionItem);

    QList<Session> sessions = sessionManager_->getSessions();
    for (int i = 0; i < sessions.size(); ++i) {
        Session session = sessions[i];
        QListWidgetItem *item = new QListWidgetItem(session.name, this);
        QIcon icon = QIcon::fromTheme(session.host.contains("localhost", Qt::CaseInsensitive) ? "computer" : "network-server");
        item->setIcon(icon);
        item->setData(Qt::UserRole, QVariant::fromValue(session));
        item->setFlags(item->flags() | Qt::ItemIsEditable);
        addItem(item);
        sessionItemMap_.insert(session.id, item);
    }

    newSessionItem->setSelected(true);
}

void SessionListWidget::selectSessionItem(const QUuid &uuid)
{
    if (sessionItemMap_.contains(uuid)) {
        sessionItemMap_[uuid]->setSelected(true);
    }
}

void SessionListWidget::addSessionItem(Session &session)
{
    sessionManager_->addSession(session);

    QListWidgetItem *item = new QListWidgetItem(session.name, this);
    QIcon icon = QIcon::fromTheme(session.host.contains("localhost", Qt::CaseInsensitive) ? "computer" : "network-server");
    item->setIcon(icon);
    item->setData(Qt::UserRole, QVariant::fromValue(session));
    item->setFlags(item->flags() | Qt::ItemIsEditable);
    addItem(item);
    sessionItemMap_.insert(session.id, item);
}

bool SessionListWidget::updateSessionItem(const Session &session)
{
    sessionManager_->editSession(session);

    QUuid uuid = session.id;
    if (sessionItemMap_.contains(uuid)) {
        sessionItemMap_[uuid]->setText(session.name);
        QIcon icon = QIcon::fromTheme(session.host.contains("localhost", Qt::CaseInsensitive) ? "computer" : "network-server");
        sessionItemMap_[uuid]->setIcon(icon);
        sessionItemMap_[uuid]->setData(Qt::UserRole, QVariant::fromValue(session));
        return true;
    }
    return false;
}

bool SessionListWidget::removeSessionItem(const QUuid &uuid)
{
    sessionManager_->deleteSession(uuid);

    if (sessionItemMap_.contains(uuid)) {
        delete sessionItemMap_.take(uuid);
        return true;
    }
    return false;
}

void SessionListWidget::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_F2) {
        QListWidgetItem* currentItem = this->currentItem();
        if (currentItem) {
            this->editItem(currentItem);
            emit itemActivated(currentItem);
        }
    } else {
        QListWidget::keyPressEvent(event);
    }
}
