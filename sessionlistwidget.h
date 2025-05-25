#ifndef SESSIONLISTWIDGET_H
#define SESSIONLISTWIDGET_H

#include <QListWidget>
#include <QKeyEvent>

#include "sessionmanagercore.h"

class SessionListWidget : public QListWidget
{
    Q_OBJECT

public:
    explicit SessionListWidget(QWidget *parent = nullptr);
    void selectSessionItem(const QUuid &uuid);
    void addSessionItem(Session &session);
    bool updateSessionItem(const Session &session);
    bool removeSessionItem(const QUuid &uuid);

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    QMap<QUuid, QListWidgetItem*> sessionItemMap_;
    SessionManagerCore *sessionManager_;
};

#endif // SESSIONLISTWIDGET_H
