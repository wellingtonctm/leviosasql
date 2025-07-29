#ifndef DATABASELOADER_H
#define DATABASELOADER_H

#include <QWidget>
#include <QListWidget>
#include <QTreeWidget>

#include "sshworker.h"
#include "input.h"

class DatabaseLoader : public QWidget
{
    Q_OBJECT
public:
    explicit DatabaseLoader(QWidget *parent, SshWorker *sshWorker);

private slots:
    void browseRemoteFile();
    void onItemDoubleClicked(QListWidgetItem *item);
    void onDirectoryListingResult(bool success, const QString &result);
    void loadDatabaseScheme();
    void onDatabaseSchemaReceived(bool success, const QString &message);

signals:

private:
    SshWorker *sshWorker_;
    QListWidget *currentFileListWidget_ = nullptr;
    QDialog *currentFileDialog_ = nullptr;
    QString currentRemotePath_;
    Input *databasePathInput_ = nullptr;
    QTreeWidget *tablesTree_ = nullptr;

    QString normalizeRemotePath(const QString &path);
    QTreeWidgetItem* findTableItem(const QString &tableName);
};

#endif // DATABASELOADER_H
