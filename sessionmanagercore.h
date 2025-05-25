#ifndef SESSIONMANAGERCORE_H
#define SESSIONMANAGERCORE_H

#include <QObject>
#include <QList>
#include <QUuid>

struct Session {
    QUuid id;
    QString name;
    QString host;
    QString user;
    QString password;
};

class SessionManagerCore : public QObject
{
    Q_OBJECT
public:
    explicit SessionManagerCore(QObject *parent = nullptr);

    QList<Session> getSessions() const;
    void addSession(Session &session);
    void editSession(const Session &session);
    void deleteSession(const QUuid &id);
    void loadSessions();
    void saveSessions();
    Session getSession(const QUuid &id) const;
    void setCurrentSession(const QUuid &id);
    Session getCurrentSession() const;

signals:
    void sessionUpdated();
    void sessionAdded();
    void sessionDeleted();
    void sessionSwitched(const Session &session);

private:
    QList<Session> sessions_;
    QUuid currentSessionId_;
};

#endif // SESSIONMANAGERCORE_H
