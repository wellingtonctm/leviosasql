#include "sessionmanagerdialog.h"
#include "sessionlistwidget.h"
#include "input.h"

#include <QSplitter>
#include <QLayout>
#include <QPushButton>

SessionManagerDialog::SessionManagerDialog(QWidget *parent) : QDialog{parent} {
    setWindowTitle("Session Manager");

    QSplitter* mainSplitter = new QSplitter(this);
    mainSplitter->setOrientation(Qt::Horizontal);

    SessionListWidget *sessionListWidget = new SessionListWidget(mainSplitter);
    mainSplitter->addWidget(sessionListWidget);

    QWidget* detailsPanel = new QWidget(mainSplitter);
    QVBoxLayout* detailsLayout = new QVBoxLayout(detailsPanel);

    Input *hostnameInput = new Input("Hostname:", this);
    Input *usernameInput = new Input("Username:", this);
    Input *passwordInput = new Input("Password:", this);
    passwordInput->setEchoMode(QLineEdit::Password);

    detailsLayout->addWidget(hostnameInput);
    detailsLayout->addWidget(usernameInput);
    detailsLayout->addWidget(passwordInput);

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
}
