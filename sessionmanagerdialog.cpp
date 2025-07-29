#include "sessionmanagerdialog.h"
#include "leviosa.h"

#include <QSplitter>
#include <QLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QMenu>
#include <QInputDialog>

SessionManagerDialog::SessionManagerDialog(QWidget *parent) : QDialog{parent} {
    setWindowTitle("Session Manager");

    QSplitter* mainSplitter = new QSplitter(this);
    mainSplitter->setOrientation(Qt::Horizontal);

    sessionListWidget_ = new SessionListWidget(mainSplitter);
    mainSplitter->addWidget(sessionListWidget_);

    QWidget* detailsPanel = new QWidget(mainSplitter);
    QVBoxLayout* detailsLayout = new QVBoxLayout(detailsPanel);

    hostnameInput_ = new Input("Hostname:", this);
    usernameInput_ = new Input("Username:", this);
    passwordInput_ = new Input("Password:", this);
    passwordInput_->setEchoMode(QLineEdit::Password);

    detailsLayout->addWidget(hostnameInput_);
    detailsLayout->addWidget(usernameInput_);
    detailsLayout->addWidget(passwordInput_);

    QHBoxLayout* actionButtonLayout = new QHBoxLayout();
    QPushButton* saveButton = new QPushButton("Save", this);
    actionButtonLayout->addWidget(saveButton);

    QPushButton* loginButton = new QPushButton("Login", this);
    actionButtonLayout->addWidget(loginButton);
    detailsLayout->addLayout(actionButtonLayout);

    mainSplitter->addWidget(detailsPanel);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(mainSplitter);
    setLayout(mainLayout);

    connect(sessionListWidget_, &QListWidget::itemActivated, this, &SessionManagerDialog::onItemActivated);
    connect(sessionListWidget_, &QListWidget::itemDoubleClicked, this, &SessionManagerDialog::onItemActivated);
    connect(sessionListWidget_, &QListWidget::itemSelectionChanged, this, &SessionManagerDialog::onSessionSelected);
    connect(sessionListWidget_, &QListWidget::itemChanged, this, &SessionManagerDialog::onItemEdited);
    connect(saveButton, &QPushButton::clicked, this, &SessionManagerDialog::onSaveSession);
    connect(loginButton, &QPushButton::clicked, this, &SessionManagerDialog::onLoginPressed);

    sessionListWidget_->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(sessionListWidget_, &QListWidget::customContextMenuRequested, this, &SessionManagerDialog::showContextMenu);

    sshThread_ = new QThread(this);
    sshWorker_ = new SshWorker();

    sshWorker_->moveToThread(sshThread_);

    connect(sshThread_, &QThread::finished, sshWorker_, &QObject::deleteLater);
    connect(sshThread_, &QThread::finished, sshThread_, &QObject::deleteLater);

    connect(sshWorker_, &SshWorker::connectionResultReceived, this, &SessionManagerDialog::onConnectionResultReceived);

    sshThread_->start();
}

SessionManagerDialog::~SessionManagerDialog()
{
    if (sshThread_->isRunning()) {
        sshThread_->quit();
        sshThread_->wait();
    }
}

void SessionManagerDialog::onLoginPressed()
{
    if (hostnameInput_->text().isEmpty() || usernameInput_->text().isEmpty()) {
        QMessageBox::warning(this, "Invalid Input", "Hostname and Username cannot be empty.");
        return;
    }

    loginData_ = LoginData{
        hostnameInput_->text(),
        usernameInput_->text(),
        passwordInput_->text()
    };

    setDisabled(true);
    sshWorker_->setCredentials(loginData_.host, loginData_.user, loginData_.password);
    QMetaObject::invokeMethod(sshWorker_, "connect", Qt::QueuedConnection);
}

void SessionManagerDialog::onConnectionResultReceived(bool success, const QString &message) {
    setDisabled(false);

    if (success) {
        Leviosa *window = new Leviosa(nullptr, sshWorker_);
        window->setAttribute(Qt::WA_DeleteOnClose);

        connect(window, &Leviosa::initializationFailed, this, [this]() {
            if (isHidden()) show();
        });

        window->show();
        hide();
    }
    else {
        QMessageBox::warning(this, "Connection Failed", message);
    }
}

LoginData SessionManagerDialog::getLoginData() const
{
    return loginData_;
}

void SessionManagerDialog::showContextMenu(const QPoint &pos)
{
    QListWidgetItem *item = sessionListWidget_->itemAt(pos);
    if (!item) {
        return;
    }

    QVariant data = item->data(Qt::UserRole);
    if (!data.isValid()) {
        return;
    }

    sessionListWidget_->setCurrentItem(item);

    QMenu contextMenu;
    QAction *renameAction = contextMenu.addAction("Rename");
    QAction *duplicateAction = contextMenu.addAction("Duplicate");
    QAction *deleteAction = contextMenu.addAction("Delete");

    connect(renameAction, &QAction::triggered, this, &SessionManagerDialog::onRenameSession);
    connect(duplicateAction, &QAction::triggered, this, &SessionManagerDialog::onDuplicateSession);
    connect(deleteAction, &QAction::triggered, this, &SessionManagerDialog::onDeleteSession);

    contextMenu.exec(sessionListWidget_->mapToGlobal(pos));
}

void SessionManagerDialog::onDuplicateSession()
{
    QListWidgetItem* currentItem = sessionListWidget_->currentItem();
    if (!currentItem)
        return;

    QVariant data = currentItem->data(Qt::UserRole);
    if (data.isValid()) {
        Session session = data.value<Session>();
        session.name += " (copy)";
        sessionListWidget_->addSessionItem(session);
    }
}

void SessionManagerDialog::onItemActivated(QListWidgetItem *item) {
    Q_UNUSED(item);
    userEditing_ = true;
}

void SessionManagerDialog::onRenameSession()
{
    QListWidgetItem* currentItem = sessionListWidget_->currentItem();
    if (currentItem) {
        sessionListWidget_->editItem(currentItem);
        userEditing_ = true;
    }
}

void SessionManagerDialog::onItemEdited(QListWidgetItem *item)
{
    if (!userEditing_)
        return;

    QVariant data = item->data(Qt::UserRole);
    if (data.isValid()) {
        userEditing_ = false;
        Session session = data.value<Session>();
        session.name = item->text();
        sessionListWidget_->updateSessionItem(session);
    }
}

void SessionManagerDialog::onDeleteSession()
{
    QListWidgetItem* currentItem = sessionListWidget_->currentItem();
    if (currentItem) {
        QVariant data = currentItem->data(Qt::UserRole);
        if (data.isValid()) {
            Session session = data.value<Session>();

            int ret = QMessageBox::warning(this, "Delete Session", "Are you sure you want to delete this session?",
                                           QMessageBox::Yes | QMessageBox::No);
            if (ret == QMessageBox::Yes) {
                sessionListWidget_->removeSessionItem(session.id);
            }
        }
    }
}

void SessionManagerDialog::onSaveSession()
{
    QListWidgetItem* currentItem = sessionListWidget_->currentItem();
    if (currentItem) {
        QVariant data = currentItem->data(Qt::UserRole);
        if (data.isValid()) {
            Session session = data.value<Session>();
            session.host = hostnameInput_->text();
            session.user = usernameInput_->text();
            session.password = passwordInput_->text();
            sessionListWidget_->updateSessionItem(session);
        }
        else {
            if (hostnameInput_->text().isEmpty() || usernameInput_->text().isEmpty()) {
                QMessageBox::warning(this, "Invalid Input", "Hostname and Username cannot be empty.");
                return;
            }

            bool ok;
            QString sessionName = QInputDialog::getText(this, "New Session", "Enter session name:", QLineEdit::Normal, "", &ok);

            if (ok && !sessionName.isEmpty()) {
                Session newSession;
                newSession.name = sessionName;
                newSession.host = hostnameInput_->text();
                newSession.user = usernameInput_->text();
                newSession.password = passwordInput_->text();
                sessionListWidget_->addSessionItem(newSession);
                sessionListWidget_->selectSessionItem(newSession.id);
            }
        }
    }
}

void SessionManagerDialog::onSessionSelected()
{
    QList<QListWidgetItem*> items = sessionListWidget_->selectedItems();
    if (items.size() > 0) {
        QVariant data = items.last()->data(Qt::UserRole);
        if (!data.isValid()) {
            hostnameInput_->clear();
            usernameInput_->clear();
            passwordInput_->clear();
            hostnameInput_->setEnabled(true);
            usernameInput_->setEnabled(true);
            passwordInput_->setEnabled(true);
        } else {
            Session session = data.value<Session>();
            hostnameInput_->setText(session.host);
            usernameInput_->setText(session.user);
            passwordInput_->setText(session.password);
            hostnameInput_->setEnabled(true);
            usernameInput_->setEnabled(true);
            passwordInput_->setEnabled(true);
        }
    }
}
