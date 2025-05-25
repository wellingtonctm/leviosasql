#include "leviosa.h"
#include "databaseloader.h"
#include "multitabcodeeditor.h"

#include <QVBoxLayout>
#include <QSplitter>

Leviosa::Leviosa(QWidget *parent) : QMainWindow(parent)
{
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout;
    centralWidget->setLayout(layout);

    QSplitter *splitter = new QSplitter(Qt::Horizontal, this);

    DatabaseLoader *databaseLoader = new DatabaseLoader(this);
    splitter->addWidget(databaseLoader);

    MultiTabCodeEditor *codeEditor = new MultiTabCodeEditor(this);
    splitter->addWidget(codeEditor);

    splitter->setSizes(QList<int>{200, 400});
    layout->addWidget(splitter);

    setCentralWidget(centralWidget);
}

Leviosa::~Leviosa() {}
