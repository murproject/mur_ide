#pragma once

#include <QObject>
#include <QTextCursor>
#include <QTextDocument>

#include "QmlUtils.hxx"

namespace Ide::Ui {

class EditorSearch : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString findString READ getFindString WRITE setFindString NOTIFY findStringChanged)
    Q_PROPERTY(QString replaceString READ getReplaceString WRITE setReplaceString NOTIFY replaceStringChanged)
    Q_PROPERTY(QString regexError READ getRegexError NOTIFY regexErrorChanged)
    Q_PROPERTY(int currentMatch READ getCurrentMatch NOTIFY currentMatchChanged)
    Q_PROPERTY(int matchCount READ getMatchCount NOTIFY matchCountChanged)
    Q_PROPERTY(int startPosition READ getStartPosition NOTIFY startPositionChanged)
    Q_PROPERTY(int endPosition READ getEndPosition NOTIFY endPositionChanged)
    Q_PROPERTY(bool caseSensitive READ getCaseSensitive WRITE setCaseSensitive NOTIFY caseSensitiveChanged)
    Q_PROPERTY(bool useRegex READ getUseRegex WRITE setUseRegex NOTIFY useRegexChanged)
    Q_PROPERTY(bool regexValid READ getRegexValid NOTIFY regexValidChanged)
    Q_PROPERTY(bool visible READ getVisible WRITE setVisible NOTIFY visibleChanged)

public:
    static qml::RegisterType<EditorSearch> Register;
    static void Create();
    static EditorSearch *instance;
    void setDocument(QTextDocument *);
    void setVisible(bool);
    bool getVisible();
    void processSearch();
    void setCaseSensitive(bool);
    bool getCaseSensitive();
    void setUseRegex(bool);
    bool getUseRegex();
    void checkRegex();
    void setRegexValid(bool);
    bool getRegexValid();
    void setRegexError(const QString &);
    QString getRegexError();
    void setFindString(const QString &);
    QString getFindString();
    void setReplaceString(const QString &);
    QString getReplaceString();
    void setCurrentMatch(int);
    int getCurrentMatch();
    int getMatchCount();
    int getStartPosition();
    int getEndPosition();
    void invalidate();

public slots:
    void findNext();
    void findPrevious();
    void replaceNext();
    void replaceAll();
signals:
    void visibleChanged();
    void useRegexChanged();
    void regexErrorChanged();
    void regexValidChanged();
    void findStringChanged();
    void currentMatchChanged();
    void caseSensitiveChanged();
    void replaceStringChanged();
    void matchCountChanged();
    void startPositionChanged();
    void endPositionChanged();

private:
    bool m_visible = false;
    QTextDocument *m_document = nullptr;
    QList<QTextCursor> m_matches;
    QString m_findString;
    QString m_replaceString;
    QString m_regexError;
    bool m_regexValid = true;
    bool m_caseSensitive = false;
    bool m_useRegex = false;
    bool m_valid = false;
    int m_currentMatch = -1;
    int m_startPosition = 0;
    int m_endPosition = 0;
    EditorSearch();
};
}
