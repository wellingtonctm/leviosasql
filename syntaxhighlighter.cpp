#include "syntaxhighlighter.h"

SyntaxHighlighter::SyntaxHighlighter(QTextDocument *parent) : QSyntaxHighlighter{parent}
{
    HighlightingRule rule;

    QTextCharFormat numberFormat;
    numberFormat.setForeground(QColor(171, 157, 242));
    rule.pattern = QRegularExpression("\\b(\\d+\\.\\d*|\\.\\d+|\\d+([eE][-+]?\\d+)?)\\b");
    rule.format = numberFormat;
    commonRules.append(rule);

    QTextCharFormat quotationFormat;
    quotationFormat.setForeground(QColor(255, 216, 102));
    rule.pattern = QRegularExpression("'.*'");
    rule.format = quotationFormat;
    commonRules.append(rule);

    QTextCharFormat singleLineCommentFormat;
    singleLineCommentFormat.setForeground(QColor(117, 113, 94));
    rule.pattern = QRegularExpression("--[^\n]*");
    rule.format = singleLineCommentFormat;
    commonRules.append(rule);
}

void SyntaxHighlighter::setKeywords(const QStringList &words) {
    HighlightingRule rule;
    QTextCharFormat keywordFormat;
    keywordFormat.setForeground(QColor(249, 38, 114));
    keywordFormat.setFontWeight(QFont::Bold);
    keywordsRules.clear();
    foreach (const QString &pattern, words) {
        rule.pattern = QRegularExpression("\\b" + pattern + "\\b", QRegularExpression::CaseInsensitiveOption);
        rule.format = keywordFormat;
        keywordsRules.append(rule);
    }
}

void SyntaxHighlighter::setFunctions(const QStringList &words) {
    HighlightingRule rule;
    QTextCharFormat functionFormat;
    functionFormat.setForeground(QColor(166, 226, 46));
    functionsRules.clear();
    foreach (const QString &pattern, words) {
        rule.pattern = QRegularExpression("\\b" + pattern + "\\b", QRegularExpression::CaseInsensitiveOption);
        rule.format = functionFormat;
        functionsRules.append(rule);
    }
}

void SyntaxHighlighter::setCommands(const QStringList &words) {
    HighlightingRule rule;
    QTextCharFormat sqliteCommandFormat;
    sqliteCommandFormat.setFontItalic(true);
    sqliteCommandFormat.setForeground(QColor(169, 220, 118));
    commandsRules.clear();
    foreach (const QString &pattern, words) {
        rule.pattern = QRegularExpression("(^|\\s)\\" + pattern + "\\b");
        rule.format = sqliteCommandFormat;
        commandsRules.append(rule);
    }
}

void SyntaxHighlighter::highlightBlock(const QString &text) {
    QVector<HighlightingRule> highlightingRules = keywordsRules + functionsRules + commandsRules + commonRules;
    foreach (const HighlightingRule &rule, highlightingRules) {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }
}
