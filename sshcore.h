#ifndef SSHCORE_H
#define SSHCORE_H

#include <libssh/libssh.h>
#include <string>
#include <functional>

class SshCore
{
public:
    SshCore(const std::string& host, const std::string& user, const std::string& password);
    ~SshCore();

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
    bool isConnected_ = false;

    void initializeSession();
    void authenticate();
    void checkConnection() const;
};

#endif // SSHCORE_H
