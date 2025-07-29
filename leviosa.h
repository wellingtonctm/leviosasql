#ifndef LEVIOSA_H
#define LEVIOSA_H

#include <QMainWindow>
#include <QThread>

#include "sessionmanagerdialog.h"
#include "sshworker.h"

class Leviosa : public QMainWindow
{
    Q_OBJECT

public:
    Leviosa(QWidget *parent, SshWorker *sshWorker);

    // void OpenSession();

public slots:
    // void onConnectionResultReceived(bool success, const QString &message);

signals:
    void initializationFailed();

private:
    SshWorker *sshWorker_;
};
#endif // LEVIOSA_H
