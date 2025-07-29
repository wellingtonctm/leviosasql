#include "sshworker.h"

SshWorker::SshWorker() {}

SshWorker::~SshWorker() {
    if (sshCore_) {
        sshCore_->disconnect();
        delete sshCore_;
        sshCore_ = nullptr;
    }
}

void SshWorker::setCredentials(const QString& host, const QString& user, const QString& password) {
    try {
        sshCore_ = new SshCore(host.toStdString(), user.toStdString(), password.toStdString());
        host_ = host;
        user_ = user;
    } catch (const std::exception &e) {
        emit connectionResultReceived(false, e.what());
    }
}

const QString &SshWorker::getUser() {
    return user_;
}

const QString &SshWorker::getHost() {
    return host_;
}

void SshWorker::connect() {
    try {
        if (!sshCore_) {
            throw std::runtime_error("Credentials not set yet.");
        }

        sshCore_->connect();
        emit connectionResultReceived(true, "Connected to SSH server.");
    } catch (const std::exception &e) {
        emit connectionResultReceived(false, e.what());
    }
}

void SshWorker::disconnect() {
    try {
        if (!sshCore_)
            return;

        if (sshCore_->isConnected())
            sshCore_->disconnect();

        delete sshCore_;
        sshCore_ = nullptr;

        emit disconnectionResultReceived(true, "Disconnected.");
    } catch (const std::exception &e) {
        emit disconnectionResultReceived(false, e.what());
    }
}

void SshWorker::runCommand(const QString &command, CommandOutputMode outputMode) {
    try {
        if (!sshCore_) {
            throw std::runtime_error("Credentials not set yet.");
        }

        if (!sshCore_->isConnected())
            sshCore_->connect();

        std::string cmd = command.toStdString();

        if (outputMode == CommandOutputMode::Chunked) {
            sshCore_->executeCommand(cmd, [this](const std::string& line) {
                emit commandOutputChunkReceived(CommandChunkState::Intermediate, CommandStatus::Success, QString::fromStdString(line));
            });

            emit commandOutputChunkReceived(CommandChunkState::Final, CommandStatus::Success, "");
            return;
        }

        std::string result = sshCore_->executeCommand(cmd);
        emit commandOutputReceived(CommandStatus::Success, QString::fromStdString(result));

    } catch (const std::exception &e) {
        QString error = e.what();

        if (outputMode == CommandOutputMode::Chunked) {
            emit commandOutputChunkReceived(CommandChunkState::Final, CommandStatus::Failure, error);
        } else {
            emit commandOutputReceived(CommandStatus::Failure, error);
        }
    }
}

void SshWorker::getDatabaseSchema(const QString &databasePath) {
    try {
        if (!sshCore_) {
            throw std::runtime_error("Credentials not set yet.");
        }

        if (!sshCore_->isConnected()) {
            sshCore_->connect();
        }

        const QString sqlQuery =
            "SELECT m.name AS table_name, p.name AS column_name, p.type, p.dflt_value, p.pk, p.'notnull' "
            "FROM sqlite_master m "
            "JOIN pragma_table_info(m.name) p "
            "WHERE m.type = 'table' "
            "ORDER BY m.name, p.cid;";

        const QString command = QString("sqlite3 --csv \"file:%1\" \"%2\"")
            .arg(databasePath, sqlQuery);

        std::string databaseSchema = sshCore_->executeCommand(command.toStdString());
        emit databaseSchemaReceived(true, QString::fromStdString(databaseSchema));
    } catch (const std::exception &ex) {
        emit databaseSchemaReceived(false, ex.what());
    }
}

void SshWorker::getDirectoryListing(const QString &directoryPath, const QString &extension) {
    try {
        if (!sshCore_) {
            throw std::runtime_error("Credentials not set yet.");
        }

        if (!sshCore_->isConnected()) {
            sshCore_->connect();
        }

        QString command;
        if (extension.isEmpty()) {
            command = QString("ls -Ap --group-directories-first %1").arg(directoryPath);
        } else {
            command = QString("ls -Ap --group-directories-first %1 | grep -E '/$|\\.%2$' || true")
                .arg(directoryPath, extension);
        }

        std::string directoryListing = sshCore_->executeCommand(command.toStdString());
        emit directoryListingReceived(true, QString::fromStdString(directoryListing));
    } catch (const std::exception &ex) {
        emit directoryListingReceived(false, ex.what());
    }
}
