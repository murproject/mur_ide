#pragma once

#include "QmlUtils.hxx"
#include "EditorSelection.hxx"
#include "EditorUtils.hxx"

#include <QObject>
#include <QtGlobal>
#include <QTextCursor>
#include <QTextDocument>
#include <QTextBlock>


namespace Ide::Ui {

class HintBase
{
public:
    HintBase() = delete;

    HintBase(QTextDocument *);

    virtual void apply() = 0;

protected:
    int getLinesCount();

    static constexpr int indentWidth = 4;

    QTextDocument *m_textDocument = nullptr;
};

}