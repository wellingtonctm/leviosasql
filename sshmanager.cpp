#include "sshmanager.h"

#include <stdexcept>
#include <memory>

SshManager::SshManager(const std::string& host, const std::string& user, const std::string& password) : session_(ssh_new()), isConnected_(false) {
    if (session_ == nullptr) {
        throw std::runtime_error("Failed to create SSH session");
    }

    setCredentials(host, user, password);
}

SshManager::~SshManager() {
    disconnect();
    if (session_) {
        ssh_free(session_);
        session_ = nullptr;
    }
}

void SshManager::initializeSession() {
    ssh_options_set(session_, SSH_OPTIONS_HOST, host_.c_str());
    ssh_options_set(session_, SSH_OPTIONS_USER, user_.c_str());
}

void SshManager::authenticate() {
    int rc = ssh_userauth_password(session_, nullptr, password_.c_str());
    if (rc != SSH_AUTH_SUCCESS) {
        throw std::runtime_error("Authentication failed: " + std::string(ssh_get_error(session_)));
    }
}

void SshManager::setCredentials(const std::string& host, const std::string& user, const std::string& password) {
    host_ = host;
    user_ = user;
    password_ = password;
}

void SshManager::connect() {
    initializeSession();
    int rc = ssh_connect(session_);
    if (rc != SSH_OK) {
        throw std::runtime_error("Connection failed: " + std::string(ssh_get_error(session_)));
    }

    authenticate();
    isConnected_ = true;
}

void SshManager::disconnect() {
    if (isConnected_) {
        ssh_disconnect(session_);
        isConnected_ = false;
    }
}

std::string SshManager::executeCommand(const std::string& command) {
    checkConnection();
    ssh_channel channel = ssh_channel_new(session_);
    if (channel == nullptr) {
        throw std::runtime_error("Failed to create SSH channel");
    }

    auto channelGuard = [](ssh_channel* ch) {
        ssh_channel_send_eof(*ch);
        ssh_channel_close(*ch);
        ssh_channel_free(*ch);
    };
    std::unique_ptr<ssh_channel, decltype(channelGuard)> channelPtr(&channel, channelGuard);

    int rc = ssh_channel_open_session(channel);
    if (rc != SSH_OK) {
        throw std::runtime_error("Failed to open SSH channel: " + std::string(ssh_get_error(session_)));
    }

    rc = ssh_channel_request_exec(channel, command.c_str());
    if (rc != SSH_OK) {
        throw std::runtime_error("Failed to execute command: " + std::string(ssh_get_error(session_)));
    }

    std::string result;
    char buffer[256];
    int nbytes;
    while ((nbytes = ssh_channel_read(channel, buffer, sizeof(buffer), 0)) > 0) {
        result.append(buffer, nbytes);
    }

    if (nbytes < 0) {
        throw std::runtime_error("Error reading from channel: " + std::string(ssh_get_error(session_)));
    }

    return result;
}

void SshManager::executeCommand(const std::string& command, std::function<void(const std::string&)> onLine) {
    checkConnection();
    ssh_channel channel = ssh_channel_new(session_);
    if (channel == nullptr) {
        throw std::runtime_error("Failed to create SSH channel");
    }

    auto channelGuard = [](ssh_channel* ch) {
        ssh_channel_send_eof(*ch);
        ssh_channel_close(*ch);
        ssh_channel_free(*ch);
    };
    std::unique_ptr<ssh_channel, decltype(channelGuard)> channelPtr(&channel, channelGuard);

    int rc = ssh_channel_open_session(channel);
    if (rc != SSH_OK) {
        throw std::runtime_error("Failed to open SSH channel: " + std::string(ssh_get_error(session_)));
    }

    rc = ssh_channel_request_exec(channel, command.c_str());
    if (rc != SSH_OK) {
        throw std::runtime_error("Failed to execute command: " + std::string(ssh_get_error(session_)));
    }

    constexpr size_t bufferSize = 256;
    char buffer[bufferSize];
    std::string lineBuffer;

    int nbytes;
    while ((nbytes = ssh_channel_read(channel, buffer, bufferSize, 0)) > 0) {
        size_t start = 0;
        for (size_t i = 0; i < static_cast<size_t>(nbytes); ++i) {
            if (buffer[i] == '\n') {
                lineBuffer.append(&buffer[start], i - start);
                onLine(lineBuffer);
                lineBuffer.clear();
                start = i + 1;
            }
        }

        if (start < static_cast<size_t>(nbytes)) {
            lineBuffer.append(&buffer[start], nbytes - start);
        }
    }

    if (nbytes < 0) {
        throw std::runtime_error("Error reading from channel: " + std::string(ssh_get_error(session_)));
    }

    if (!lineBuffer.empty()) {
        onLine(lineBuffer);
    }
}


void SshManager::checkConnection() const {
    if (!isConnected_) {
        throw std::runtime_error("Not connected to SSH server");
    }
}

bool SshManager::isConnected() const {
    return isConnected_;
}
