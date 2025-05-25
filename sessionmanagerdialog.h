#ifndef SESSIONMANAGERDIALOG_H
#define SESSIONMANAGERDIALOG_H

#include <QDialog>

#include "input.h"
#include "sessionlistwidget.h"

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
    std::optional<LoginData> getLoginData() const;

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
    std::optional<LoginData> loginData_;
    bool userEditing_;
};

#endif // SESSIONMANAGERDIALOG_H
