#ifndef SYNTAXHIGHLIGHTER_H
#define SYNTAXHIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QRegularExpression>

class SyntaxHighlighter : public QSyntaxHighlighter
{
public:
    explicit SyntaxHighlighter(QTextDocument *parent = nullptr);
    void setKeywords(const QStringList &words);
    void setFunctions(const QStringList &words);
    void setCommands(const QStringList &words);

protected:
    virtual void highlightBlock(const QString &text) override;

private:
    QStringList loadWords(const QString& fileName);

private:
    struct HighlightingRule {
        QRegularExpression pattern;
        QTextCharFormat format;
    };

    QVector<HighlightingRule> keywordsRules;
    QVector<HighlightingRule> functionsRules;
    QVector<HighlightingRule> commandsRules;
    QVector<HighlightingRule> commonRules;
};

#endif // SYNTAXHIGHLIGHTER_H
