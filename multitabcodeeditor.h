#ifndef MULTITABCODEEDITOR_H
#define MULTITABCODEEDITOR_H

#include <QTabWidget>

class MultiTabCodeEditor : public QWidget
{
    Q_OBJECT
public:
    explicit MultiTabCodeEditor(QWidget *parent = nullptr);
    void newTab();

private:
    QTabWidget *tabWidget_;
    QStringList keywordCompletions_;
    QStringList functionCompletions_;
    QStringList commandCompletions_;
};

static QStringList loadWords(const QString& fileName);

#endif // MULTITABCODEEDITOR_H
