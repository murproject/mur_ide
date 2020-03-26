#pragma once

#include <QObject>
#include <QTextDocument>

#include "QmlUtils.hxx"

namespace Ide::Ui {

class Format;

class editorSelection : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int startPosition READ getStartPosition WRITE setStartPosition NOTIFY startPositionChanged)
    Q_PROPERTY(int endPosition READ getEndPosition WRITE setEndPosition NOTIFY endPositionChanged)
    Q_PROPERTY(int startColumn READ getStartColumn NOTIFY startColumnChanged)
    Q_PROPERTY(int endColumn READ getEndColumn NOTIFY endColumnChanged)
    Q_PROPERTY(int startLine READ getStartLine NOTIFY startLineChanged)
    Q_PROPERTY(int endLine READ getEndLine NOTIFY endLineChanged)
    Q_PROPERTY(bool empty READ isEmpty NOTIFY emptyChanged)

public:
    static Ide::qml::RegisterType<editorSelection> Register;
    static void Create();
    static editorSelection *instance;

public:
    void setDocument(QTextDocument *);
    void setStartPosition(int);
    int getStartPosition();
    void setEndPosition(int);
    int getEndPosition();
    int getStartColumn();
    int getEndColumn();
    int getStartLine();
    int getEndLine();
    bool isEmpty();

signals:
    void startPositionChanged();
    void endPositionChanged();
    void startColumnChanged();
    void endColumnChanged();
    void startLineChanged();
    void endLineChanged();
    void emptyChanged();

private:
    friend class Format;
    QTextDocument *m_document = nullptr;
    int m_startPosition = 0;
    int m_endPosition = 0;
    int m_startColumn = 0;
    int m_endColumn = 0;
    int m_startLine = 0;
    int m_endLine = 0;
    bool m_empty = true;

    editorSelection();
    void setEmpty(bool);
    void setEndLine(int);
    void setStartLine(int);
    void setStartColumn(int);
    void setEndColumn(int);
};
}
