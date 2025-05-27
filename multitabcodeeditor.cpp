#include "multitabcodeeditor.h"
#include "simplecodeeditor.h"
#include "syntaxhighlighter.h"

#include <QVBoxLayout>
#include <QFile>

MultiTabCodeEditor::MultiTabCodeEditor(QWidget *parent) : QWidget{parent}
{
    QVBoxLayout *layout = new QVBoxLayout;
    setLayout(layout);

    tabWidget_ = new QTabWidget(this);
    layout->addWidget(tabWidget_);

    keywordCompletions_ = loadWords(":/resources/sqlite_keywords.txt");
    functionCompletions_ = loadWords(":/resources/sqlite_functions.txt");
    commandCompletions_ = loadWords(":/resources/sqlite_commands.txt");

    newTab();
}

void MultiTabCodeEditor::newTab() {
    SimpleCodeEditor *editor = new SimpleCodeEditor(this);

    SyntaxHighlighter *syntaxHighlighter = new SyntaxHighlighter(editor->document());
    syntaxHighlighter->setKeywords(keywordCompletions_);
    syntaxHighlighter->setFunctions(functionCompletions_);
    syntaxHighlighter->setCommands(commandCompletions_);

    tabWidget_->addTab(editor, "untitled");
}

static QStringList loadWords(const QString& fileName)
{
    QStringList words;

    QFile file(fileName);
    if (!file.open(QFile::ReadOnly))
        return words;

    while (!file.atEnd()) {
        QByteArray line = file.readLine();
        if (!line.isEmpty())
            words << QString::fromUtf8(line.trimmed());
    }

    return words;
}
