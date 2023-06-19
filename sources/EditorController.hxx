#pragma once

#include "QmlUtils.hxx"

#include <QObject>
#include <QQuickTextDocument>

namespace Ide::Ui {

class EditorUtils;
class EditorSearch;
class EditorErrors;
class editorSelection;
class EditorHighlighter;
class EditorIndenter;
class EditorHints;

class EditorController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int fontSize READ getFontSize NOTIFY fontSizeChanged)
    Q_PROPERTY(bool expanded READ getExpanded NOTIFY expandedChanged)
    Q_PROPERTY(bool modified READ getModified NOTIFY modifiedChanged)
    Q_PROPERTY(int cursorX READ getCursorX WRITE setCursorX NOTIFY coucursorPositionChanged)
    Q_PROPERTY(int cursorY READ getCursorY WRITE setCursorY NOTIFY coucursorPositionChanged)

    Q_PROPERTY(QString fileUrl READ getFileUrl NOTIFY fileUrlChanged)
    Q_PROPERTY(QString fileName READ getFileName NOTIFY fileNameChanged)
    Q_PROPERTY(QQuickTextDocument *document READ getDocument WRITE setDocument NOTIFY documentChanged)
    Q_PROPERTY(Ide::Ui::EditorErrors *errors READ getErrors CONSTANT)
    Q_PROPERTY(Ide::Ui::EditorSearch *search READ getSearch CONSTANT)
    Q_PROPERTY(Ide::Ui::editorSelection *selection READ getSelection CONSTANT)
    Q_PROPERTY(Ide::Ui::EditorIndenter *indenter READ getIndenter CONSTANT)
    Q_PROPERTY(Ide::Ui::EditorHints *hints READ getHints CONSTANT)
public:
    friend class Util;
    static EditorController *instance;
    static qml::RegisterType<EditorController> Register;
    static void Create();

public slots:
    void setModified(bool);
    void run();
    void newFile();
    void openFile();
    void saveFile();
    void saveFileAs();
    void showSearch();
    void resetFontSize();
    void increaseFontSize();
    void decreaseFontSize();
    EditorIndenter *getIndenter();
    EditorHints *getHints();

    QString getText();

public:
    void setDocument(QQuickTextDocument *);
    QQuickTextDocument *getDocument();
    editorSelection *getSelection();
    EditorErrors *getErrors();
    EditorSearch *getSearch();
    void openFile(const QString &);
    bool getExpanded();
    QString getFileUrl();
    QString getFileName();
    bool getModified();
    void toggleExpanded();
    int getFontSize();

    int getCursorX();
    int getCursorY();

    void setCursorX(int);
    void setCursorY(int);

    ~EditorController();
signals:
    void select(int start, int end);
    void fileNameChanged();
    void fontSizeChanged();
    void documentChanged();
    void expandedChanged();
    void modifiedChanged();
    void fileUrlChanged();
    void compile();
    void clear();
    void coucursorPositionChanged();

private:
    EditorController();

    QString m_fileUrl;
    QString m_fileName;
    int m_fontSizeMin = 8;
    int m_fontSize = 9;
    int m_fontSizeMax = 20;

    int m_cursorPositionX = 0;
    int m_cursorPositionY = 0;

    bool m_expanded = true;
    bool m_modified = false;
    bool m_about_to_be_saved = false;
    EditorHighlighter *m_highlighter = nullptr;
    QFileSystemWatcher *m_fileWatcher = nullptr;

    QQuickTextDocument *m_document;

    void setFileUrl(const QString &);
    void setFileName(const QString &);
    void observedFileChanged(const QString &);
    void observeFile(const QString &);
};
} 
