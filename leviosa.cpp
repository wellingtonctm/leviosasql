#include "leviosa.h"
#include "databaseloader.h"

#include <QVBoxLayout>

Leviosa::Leviosa(QWidget *parent) : QMainWindow(parent)
{
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout;
    centralWidget->setLayout(layout);

    DatabaseLoader *databaseLoader = new DatabaseLoader(this);
    layout->addWidget(databaseLoader);

    setCentralWidget(centralWidget);
}

Leviosa::~Leviosa() {}
