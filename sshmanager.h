#ifndef SSHMANAGER_H
#define SSHMANAGER_H

#include <libssh/libssh.h>
#include <string>
#include <functional>

class SshManager
{
public:
    SshManager(const std::string& host, const std::string& user, const std::string& password);
    ~SshManager();

    void setCredentials(const std::string& host, const std::string& user, const std::string& password);
    void connect();
    void disconnect();
    bool isConnected() const;
    std::string executeCommand(const std::string& command);
    void executeCommand(const std::string& command, std::function<void(const std::string&)> onLine);

private:
    std::string host_;
    std::string user_;
    std::string password_;
    ssh_session session_ = nullptr;
    bool isConnected_;

    void initializeSession();
    void authenticate();
    void checkConnection() const;
};

#endif // SSHMANAGER_H
