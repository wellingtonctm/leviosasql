#include "input.h"

#include <QVBoxLayout>
#include <QRegularExpressionValidator>

Input::Input(QString label, QWidget *parent) : QWidget{parent}
{
    QVBoxLayout *layout = new QVBoxLayout;
    setLayout(layout);

    layout->setContentsMargins(0,0,0,0);
    label_ = new QLabel(label, this);
    layout->addWidget(label_);
    input_ = new QLineEdit(this);
    layout->addWidget(input_);
}

void Input::setValidator(const QString &pattern) {
    auto *validator = new QRegularExpressionValidator(QRegularExpression(pattern), this);
    input_->setValidator(validator);
}

bool Input::validate() {
    QString str = text();
    int pos = 0;
    return input_->validator()->validate(str, pos) == QValidator::Acceptable;
}

void Input::setEchoMode(QLineEdit::EchoMode mode) {
    input_->setEchoMode(mode);
}

QString Input::text() {
    return input_->text();
}

void Input::setText(const QString &text) {
    return input_->setText(text);
}

void Input::setPlaceholderText(const QString &text) {
    return input_->setPlaceholderText(text);
}

void Input::setEnabled(bool enabled) {
    return input_->setEnabled(enabled);
}

void Input::setReadOnly(bool readOnly) {
    return input_->setReadOnly(readOnly);
}

void Input::clear() {
    return input_->clear();
}
