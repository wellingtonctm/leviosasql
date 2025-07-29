#include "databaseloader.h"

#include <QLayout>
#include <QPushButton>
#include <QFileIconProvider>
#include <QDialog>
#include <QApplication>
#include <QMessageBox>

DatabaseLoader::DatabaseLoader(QWidget *parent, SshWorker *sshWorker) : QWidget{parent}, sshWorker_(sshWorker)
{
    QVBoxLayout *layout = new QVBoxLayout;
    setLayout(layout);

    QHBoxLayout *databaseBrowserLayout = new QHBoxLayout;
    databasePathInput_ = new Input("DB Path", this);
    databasePathInput_->setReadOnly(true);
    databasePathInput_->setPlaceholderText("/home/leviosa/example.db");
    databaseBrowserLayout->addWidget(databasePathInput_);

    QVBoxLayout *buttonLayout = new QVBoxLayout;
    buttonLayout->setAlignment(Qt::AlignBottom);
    QPushButton *browseDatabaseButton = new QPushButton("Browse", this);
    buttonLayout->addWidget(browseDatabaseButton);
    databaseBrowserLayout->addLayout(buttonLayout);

    connect(browseDatabaseButton, &QPushButton::clicked, this, &DatabaseLoader::browseRemoteFile);

    layout->addLayout(databaseBrowserLayout);

    QPushButton *loadDatabaseButton = new QPushButton("Load", this);

    connect(loadDatabaseButton, &QPushButton::clicked, this, &DatabaseLoader::loadDatabaseScheme);

    layout->addWidget(loadDatabaseButton);

    tablesTree_ = new QTreeWidget(this);
    tablesTree_->setColumnCount(1);
    tablesTree_->setHeaderLabels({"Schema"});

    layout->addWidget(tablesTree_);
}


void DatabaseLoader::browseRemoteFile()
{
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("Remote File Browser");

    QVBoxLayout *dialogLayout = new QVBoxLayout(dialog);
    QListWidget *fileListWidget = new QListWidget(dialog);
    dialogLayout->addWidget(fileListWidget);

    fileListWidget->addItem("Loading...");
    dialog->open();

    currentFileListWidget_ = fileListWidget;
    currentFileDialog_ = dialog;

    connect(fileListWidget, &QListWidget::itemDoubleClicked, this, &DatabaseLoader::onItemDoubleClicked);
    connect(sshWorker_, &SshWorker::directoryListingReceived, this, &DatabaseLoader::onDirectoryListingResult);

    if (currentRemotePath_.isEmpty())
        currentRemotePath_ = "/home";

    QMetaObject::invokeMethod(sshWorker_, "getDirectoryListing", Qt::QueuedConnection,
                              Q_ARG(QString, currentRemotePath_), Q_ARG(QString, "db"));
}

void DatabaseLoader::onDirectoryListingResult(bool success, const QString &result)
{
    if (!currentFileListWidget_) return;

    QFileIconProvider iconProvider;

    currentFileListWidget_->clear();

    if (success) {
        if (currentRemotePath_ != "/") {
            QListWidgetItem *item = new QListWidgetItem(".. (Parent Directory)", currentFileListWidget_);
            item->setIcon(QApplication::style()->standardIcon(QStyle::SP_ArrowUp));
            item->setData(Qt::UserRole, "up");
        }

        QStringList fileList = result.split('\n', Qt::SkipEmptyParts);
        for (const QString &fileName : std::as_const(fileList)) {
            QListWidgetItem *item = new QListWidgetItem(fileName, currentFileListWidget_);
            if (fileName.endsWith('/')) {
                item->setIcon(iconProvider.icon(QFileIconProvider::Folder));
                item->setData(Qt::UserRole, "dir");
            } else {
                item->setIcon(iconProvider.icon(QFileIconProvider::File));
                item->setData(Qt::UserRole, "file");
            }
        }
    } else {
        currentFileListWidget_->addItem("Error retrieving directory listing.");
    }
}

void DatabaseLoader::onItemDoubleClicked(QListWidgetItem *item)
{
    QString type = item->data(Qt::UserRole).toString();
    QString newPath = currentRemotePath_;
    if (type == "up")
        newPath += "/..";
    else if (type == "dir")
        newPath += "/" + item->text();
    else if (type == "file") {
        newPath += "/" + item->text();
        databasePathInput_->setText(newPath);
        currentFileDialog_->close();
        return;
    } else {
        return;
    }

    currentRemotePath_ = normalizeRemotePath(newPath);

    QMetaObject::invokeMethod(sshWorker_, "getDirectoryListing", Qt::QueuedConnection,
                              Q_ARG(QString, currentRemotePath_), Q_ARG(QString, "db"));
}

QString DatabaseLoader::normalizeRemotePath(const QString &path)
{
    QStringList parts = path.split('/', Qt::SkipEmptyParts);
    QStringList normalizedParts;
    for (const QString &part : std::as_const(parts)) {
        if (part == ".")
            continue;
        else if (part == "..") {
            if (!normalizedParts.isEmpty())
                normalizedParts.removeLast();
        } else {
            normalizedParts.append(part);
        }
    }

    return "/" + normalizedParts.join('/');
}

void DatabaseLoader::loadDatabaseScheme() {
    QString databasePath = databasePathInput_->text();

    connect(sshWorker_, &SshWorker::databaseSchemaReceived, this, &DatabaseLoader::onDatabaseSchemaReceived);

    QMetaObject::invokeMethod(sshWorker_, "getDatabaseSchema",
                              Qt::QueuedConnection, Q_ARG(QString, databasePath));
}

struct TableColumn {
    QString name;
    QString type;
    QString defaultValue;
    int primaryKey;
    bool notNull;
};

struct TableInfo {
    QString tableName;
    QVector<TableColumn> columns;
};

inline QStringList csvLineToObject(const QString &csv_line) {
    return csv_line.split(',', Qt::KeepEmptyParts);
}

QTreeWidgetItem* DatabaseLoader::findTableItem(const QString &tableName)
{
    for (int i = 0; i < tablesTree_->topLevelItemCount(); ++i) {
        QTreeWidgetItem *item = tablesTree_->topLevelItem(i);
        if (item->text(0) == tableName)
            return item;
    }

    return nullptr;
}

void DatabaseLoader::onDatabaseSchemaReceived(bool success, const QString &message) {
    if (!success) {
        QMessageBox::critical(this, "Load Database", message);
        return;
    }

    QMap<QString, TableInfo> databaseSchema;
    QMap<QString, QStringList> schema;

    QFileIconProvider iconProvider;

    tablesTree_->clear();

    QStringList lines = message.trimmed().split("\n", Qt::SkipEmptyParts);
    for (const QString &line : std::as_const(lines)) {
        QStringList tupla = csvLineToObject(line);
        if (tupla.size() < 6) continue;

        QString tableName = tupla[0];
        TableColumn tableColumn{
            .name = tupla[1],
            .type = tupla[2],
            .defaultValue = tupla[3],
            .primaryKey = tupla[4].toInt(),
            .notNull = (tupla[5].toShort() != 0)
        };
        schema[tableName] << tableColumn.name;

        QTreeWidgetItem *tableItem = nullptr;
        if (!databaseSchema.contains(tableName)) {
            databaseSchema[tableName] = {tableName, {}};
            tableItem = new QTreeWidgetItem(tablesTree_);
            tableItem->setText(0, tableName);
            tableItem->setIcon(0, iconProvider.icon(QFileIconProvider::Folder));
        } else {
            tableItem = findTableItem(tableName);
        }

        if (tableItem) {
            databaseSchema[tableName].columns.append(tableColumn);
            QTreeWidgetItem *columnItem = new QTreeWidgetItem(tableItem);
            columnItem->setText(0, tableColumn.name + " (" + tableColumn.type + ")");
            columnItem->setIcon(0, iconProvider.icon(QFileIconProvider::File));
            if (tableColumn.primaryKey) {
                QFont font;
                font.setBold(true);
                columnItem->setFont(0, font);
            }
        }
    }
}








