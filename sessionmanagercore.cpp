#include "sessionmanagercore.h"

#include <QSettings>

SessionManagerCore::SessionManagerCore(QObject *parent) : QObject(parent) {
    loadSessions();
}

void SessionManagerCore::loadSessions() {
    QSettings settings("LeviosaSQL", "Sessions");
    int size = settings.beginReadArray("sessions");
    sessions_.clear();

    for (int i = 0; i < size; ++i) {
        settings.setArrayIndex(i);
        Session s;
        QString idStr = settings.value("id").toString();
        s.id = idStr.isEmpty() ? QUuid::createUuid() : QUuid(idStr);
        s.name = settings.value("name").toString();
        s.host = settings.value("host").toString();
        s.user = settings.value("user").toString();
        s.password = settings.value("password").toString();
        sessions_.append(s);
        emit sessionAdded();
    }
    settings.endArray();
}

void SessionManagerCore::saveSessions() {
    QSettings settings("LeviosaSQL", "Sessions");
    settings.beginWriteArray("sessions");

    for (int i = 0; i < sessions_.size(); ++i) {
        settings.setArrayIndex(i);
        settings.setValue("id", sessions_[i].id.toString());
        settings.setValue("name", sessions_[i].name);
        settings.setValue("host", sessions_[i].host);
        settings.setValue("user", sessions_[i].user);
        settings.setValue("password", sessions_[i].password);
    }
    settings.endArray();
}

QList<Session> SessionManagerCore::getSessions() const {
    return sessions_;
}

void SessionManagerCore::addSession(Session &session) {
    session.id = QUuid::createUuid();
    sessions_.append(session);
    saveSessions();
    emit sessionAdded();
}

void SessionManagerCore::editSession(const Session &session) {
    for (int i = 0; i < sessions_.size(); ++i) {
        if (sessions_[i].id == session.id) {
            sessions_[i].host = session.host;
            sessions_[i].name = session.name;
            sessions_[i].user = session.user;
            sessions_[i].password = session.password;
            saveSessions();
            emit sessionUpdated();
            return;
        }
    }
}

void SessionManagerCore::deleteSession(const QUuid &id) {
    for (int i = 0; i < sessions_.size(); ++i) {
        if (sessions_[i].id == id) {
            sessions_.removeAt(i);
            saveSessions();
            emit sessionDeleted();
            return;
        }
    }
}

Session SessionManagerCore::getSession(const QUuid &id) const {
    for (const auto &session : std::as_const(sessions_)) {
        if (session.id == id) {
            return session;
        }
    }
    return {};
}

void SessionManagerCore::setCurrentSession(const QUuid &id) {
    for (const auto &session : std::as_const(sessions_)) {
        if (session.id == id) {
            currentSessionId_ = id;
            emit sessionSwitched(session);
            return;
        }
    }
}

Session SessionManagerCore::getCurrentSession() const {
    return getSession(currentSessionId_);
}
