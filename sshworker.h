#ifndef SSHWORKER_H
#define SSHWORKER_H

#include <QObject>

#include "sshcore.h"

enum class CommandOutputMode {
    Complete,
    Chunked
};

enum class CommandChunkState {
    Intermediate,
    Final
};

enum class CommandStatus {
    Success,
    Failure
};

class SshWorker : public QObject
{
    Q_OBJECT
public:
    explicit SshWorker();
    ~SshWorker();

    const QString &getUser();
    const QString &getHost();

public slots:
    void setCredentials(const QString& host, const QString& user, const QString& password);
    void connect();
    void disconnect();
    void runCommand(const QString &command, CommandOutputMode outputMode);
    void getDatabaseSchema(const QString &databasePath);
    void getDirectoryListing(const QString &directoryPath, const QString &extension = QString());

signals:
    void connectionResultReceived(bool success, const QString &message);
    void disconnectionResultReceived(bool success, const QString &message);
    void commandOutputReceived(CommandStatus status, const QString &message);
    void commandOutputChunkReceived(CommandChunkState chunkState, CommandStatus status, const QString &message);
    void databaseSchemaReceived(bool success, const QString &message);
    void directoryListingReceived(bool success, const QString &message);

private:
    SshCore* sshCore_ = nullptr;
    QString user_;
    QString host_;
};

#endif // SSHWORKER_H
