#ifndef SESSIONMANAGERDIALOG_H
#define SESSIONMANAGERDIALOG_H

#include <QDialog>
#include <QThread>

#include "input.h"
#include "sessionlistwidget.h"
#include "sshworker.h"

struct LoginData {
    QString host;
    QString user;
    QString password;
};

class SessionManagerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SessionManagerDialog(QWidget *parent = nullptr);
    ~SessionManagerDialog();

    LoginData getLoginData() const;

public slots:
    void onConnectionResultReceived(bool success, const QString &message);

private slots:
    void onSessionSelected();
    void onSaveSession();
    void showContextMenu(const QPoint &pos);
    void onLoginPressed();
    void onRenameSession();
    void onDuplicateSession();
    void onDeleteSession();
    void onItemActivated(QListWidgetItem *item);
    void onItemEdited(QListWidgetItem *item);

private:
    SessionListWidget *sessionListWidget_;
    Input *hostnameInput_;
    Input *usernameInput_;
    Input *passwordInput_;
    LoginData loginData_;
    bool userEditing_;
    SshWorker *sshWorker_ = nullptr;
    QThread *sshThread_ = nullptr;
};

#endif // SESSIONMANAGERDIALOG_H
