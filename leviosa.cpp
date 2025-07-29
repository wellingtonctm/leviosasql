#include "leviosa.h"
#include "databaseloader.h"
#include "multitabcodeeditor.h"

#include <QVBoxLayout>
#include <QSplitter>
#include <QMessageBox>
#include <QTimer>

Leviosa::Leviosa(QWidget *parent, SshWorker *sshWorker) : QMainWindow(parent), sshWorker_(sshWorker)
{
    if (!sshWorker_) {
        QMessageBox::warning(this, tr("Connection Error"), tr("No connection data was provided."));
        QTimer::singleShot(0, this, &Leviosa::initializationFailed);
        QTimer::singleShot(0, this, &QWidget::close);
        return;
    }

    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout;
    centralWidget->setLayout(layout);

    setWindowTitle(tr("LeviosaSQL (%1@%2)")
        .arg(sshWorker_->getUser(), sshWorker_->getHost()));

    QSplitter *splitter = new QSplitter(Qt::Horizontal, this);

    DatabaseLoader *databaseLoader = new DatabaseLoader(this, sshWorker_);
    splitter->addWidget(databaseLoader);

    MultiTabCodeEditor *codeEditor = new MultiTabCodeEditor(this);
    splitter->addWidget(codeEditor);

    splitter->setSizes(QList<int>{200, 400});
    layout->addWidget(splitter);

    setCentralWidget(centralWidget);
}
