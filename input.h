#ifndef INPUT_H
#define INPUT_H

#include <QLineEdit>
#include <QLabel>

#include <QWidget>

class Input : public QWidget
{
    Q_OBJECT

public:
    explicit Input(QString label, QWidget *parent = nullptr);

    QString text();
    void setValidator(const QString &pattern);
    bool validate();
    void setEchoMode(QLineEdit::EchoMode mode);
    void setText(const QString &text);
    void setPlaceholderText(const QString &text);
    void setEnabled(bool enabled);
    void setReadOnly(bool readOnly);
    void clear();

private:
    QLabel *label_;
    QLineEdit *input_;
};

#endif // INPUT_H
