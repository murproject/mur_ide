#pragma once

#include "QmlUtils.hxx"
#include <QTextCursor>
#include <QTextDocument>

namespace Ide::Ui {
class EditorIndenter : public QObject
{
    Q_OBJECT
public:
    static qml::RegisterType<EditorIndenter> Register;

    static EditorIndenter *instance;

    static void Create();

    void setDocument(QTextDocument *);

    bool isIndentationBlocked = false;

public slots:
    void setIndentationBlock(bool state);

private:
    int blockIndentation(QTextBlock &);

    QString indentationToInsert(int n);

    bool isIndentationGoesIn(const QTextBlock &);

    bool isIndentationGoesOut(const QTextBlock &);

private:
    EditorIndenter();

    void onTextModified();

    void computeIndent();

    void setIndent(int indent);

    bool ltrimDocumentLine();

    int m_prevBlockCount = 0;

    const int indentWidth = 4;

    QTextCursor cursor;

    QStringList startOfCodeBlock;

    QStringList endOfCodeBlock;

    QTextDocument *m_textDocument = nullptr;
};

} // namespace ide::ui
