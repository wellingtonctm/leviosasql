#include "databaseloader.h"
#include "input.h"

#include <QLayout>
#include <QPushButton>
#include <QTreeWidget>

DatabaseLoader::DatabaseLoader(QWidget *parent) : QWidget{parent}
{
    QVBoxLayout *layout = new QVBoxLayout;
    setLayout(layout);

    QHBoxLayout *databaseBrowserLayout = new QHBoxLayout;
    Input *databasePathInput = new Input("DB Path", this);
    databasePathInput->setReadOnly(true);
    databasePathInput->setPlaceholderText("/home/leviosa/example.db");
    databaseBrowserLayout->addWidget(databasePathInput);

    QVBoxLayout *buttonLayout = new QVBoxLayout;
    buttonLayout->setAlignment(Qt::AlignBottom);
    QPushButton *browseDatabaseButton = new QPushButton("Browse", this);
    buttonLayout->addWidget(browseDatabaseButton);
    databaseBrowserLayout->addLayout(buttonLayout);

    layout->addLayout(databaseBrowserLayout);

    QPushButton *loadDatabaseButton = new QPushButton("Load", this);
    loadDatabaseButton->setDisabled(true);

    layout->addWidget(loadDatabaseButton);

    QTreeWidget *tablesTree = new QTreeWidget(this);
    tablesTree->setColumnCount(1);
    tablesTree->setHeaderLabels({"Schema"});

    layout->addWidget(tablesTree);
}














