#ifndef SIMPLECODEEDITOR_H
#define SIMPLECODEEDITOR_H

#include <QPlainTextEdit>

class LineNumberArea;

class SimpleCodeEditor : public QPlainTextEdit
{
    Q_OBJECT

public:
    explicit SimpleCodeEditor(QWidget *parent = nullptr);

    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth() const;

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void updateLineNumberAreaWidth();
    void updateLineNumberArea(const QRect &rect, int dy);
    void highlightCurrentLine();

private:
    QWidget *lineNumberArea;
};


class LineNumberArea : public QWidget
{
public:
    LineNumberArea(SimpleCodeEditor *editor) : QWidget(editor), codeEditor(editor)
    {}

    QSize sizeHint() const override
    {
        return QSize(codeEditor->lineNumberAreaWidth(), 0);
    }

protected:
    void paintEvent(QPaintEvent *event) override
    {
        codeEditor->lineNumberAreaPaintEvent(event);
    }

private:
    SimpleCodeEditor *codeEditor;
};

#endif // SIMPLECODEEDITOR_H
