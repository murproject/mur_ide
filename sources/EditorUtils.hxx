#pragma once

#include <QTextCursor>
#include <QTextDocument>

namespace Ide::Ui {

class EditorUtils
{
public:
    EditorUtils() = delete;
    static QTextDocument *document;
    static QTextCursor getCursor(int);
    static QString getLine(int);
    static QChar getChar(int);
};
}
