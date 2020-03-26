#include "EditorController.hxx"
#include "EditorErrors.hxx"
#include "EditorHighlighter.hxx"
#include "EditorIndenter.hxx"
#include "EditorHints.hxx"
#include "EditorSearch.hxx"
#include "EditorSelection.hxx"
#include "EditorUtils.hxx"
#include "TextIO.hxx"

#include <QDebug>
#include <QFile>
#include <QTextDocument>
#include <QTextOption>

namespace Ide::Ui {

EditorController *EditorController::instance = nullptr;
qml::RegisterType<EditorController> EditorController::Register;

EditorController::EditorController()
{
    if (instance != nullptr) {
        throw std::runtime_error("Instance of editor_controller already exists");
    }
}

void EditorController::Create()
{
    instance = new EditorController();
    instance->m_fileWatcher = new QFileSystemWatcher;
    QObject::connect(instance->m_fileWatcher,
                     &QFileSystemWatcher::fileChanged,
                     instance,
                     &EditorController::observedFileChanged);
}

void EditorController::setDocument(QQuickTextDocument *document)
{
    m_document = document;
    auto textDocument = document->textDocument();
    auto options = textDocument->defaultTextOption();
    options.setTabStop(20);
    textDocument->setDefaultTextOption(options);
    m_highlighter = new EditorHighlighter(textDocument);

    EditorUtils::document = textDocument;
    editorSelection::instance->setDocument(textDocument);
    EditorSearch::instance->setDocument(textDocument);
    EditorIndenter::instance->setDocument(textDocument);
    EditorHints::instance->setDocument(textDocument);
    emit documentChanged();
}

void EditorController::setModified(bool modified)
{
    if (m_modified != modified) {
        m_modified = modified;
        emit modifiedChanged();
    }
}

bool EditorController::getModified()
{
    return m_modified;
}

void EditorController::setFileName(const QString &fileName)
{
    m_fileName = fileName;
    emit fileNameChanged();
}

void EditorController::observedFileChanged(const QString &)
{
    setModified(true);
    if (m_fileUrl.length() > 0 && !m_about_to_be_saved) {
        openFile(m_fileUrl);
    }
    setModified(false);
    m_about_to_be_saved = false;
}

void EditorController::observeFile(const QString &fileUrl)
{
    m_fileWatcher->addPath(fileUrl);
}

QString EditorController::getFileName()
{
    return m_fileName;
}

void EditorController::setFileUrl(const QString &fileUrl)
{
    m_fileUrl = fileUrl;
    setFileName(IO::fileNameFromUrl(fileUrl, IO::FileSuffix::On));
    emit fileUrlChanged();
}

QString EditorController::getFileUrl()
{
    return m_fileUrl;
}

bool EditorController::getExpanded()
{
    return m_expanded;
}

void EditorController::toggleExpanded()
{
    m_expanded = !m_expanded;
    emit expandedChanged();
}

QQuickTextDocument *EditorController::getDocument()
{
    return m_document;
}

EditorErrors *EditorController::getErrors()
{
    return EditorErrors::instance;
}

EditorSearch *EditorController::getSearch()
{
    return EditorSearch::instance;
}

editorSelection *EditorController::getSelection()
{
    return editorSelection::instance;
}

void EditorController::openFile(const QString &fileUrl)
{
    if (IO::fileExists(fileUrl)) {
        if (m_fileUrl.length() > 0) {
            m_fileWatcher->removePath(this->m_fileUrl);
        }

        EditorSearch::instance->invalidate();
        EditorErrors::instance->clear();
        m_document->textDocument()->setPlainText(IO::Read::textFromUrl(fileUrl));
        setModified(false);
        setFileUrl(fileUrl);
        observeFile(fileUrl);
    }
}

void EditorController::run()
{
    emit compile();
}

void EditorController::saveFile()
{
    if (m_fileUrl.length() < 1 || !IO::fileExists(m_fileUrl)) {
        saveFileAs();
    } else {
        m_about_to_be_saved = true;
        IO::Write::textToFile(m_document->textDocument()->toPlainText(), m_fileUrl);
    }
}

void EditorController::saveFileAs()
{
    auto newUrl = IO::FromDialog::selectSavFileUrl();

    if (newUrl.isNull() || newUrl.isEmpty()) {
        return;
    }

    if (!newUrl.endsWith(".py")) {
        newUrl.append(".py");
    }

    m_about_to_be_saved = true;
    if (IO::Write::textToFile(m_document->textDocument()->toPlainText(), newUrl)) {
        setModified(false);
        setFileUrl(newUrl);
    }
}

void EditorController::newFile()
{
    EditorSearch::instance->invalidate();
    EditorErrors::instance->clear();
    emit clear();
    setModified(false);
    setFileUrl("");
}

void EditorController::openFile()
{
    openFile(IO::FromDialog::selectOpenFileUrl("*.py"));
}

void EditorController::showSearch()
{
    EditorSearch::instance->setVisible(true);
}

void EditorController::resetFontSize()
{
    m_fontSize = 9;
    emit fontSizeChanged();
}

void EditorController::increaseFontSize()
{
    if (m_fontSize < m_fontSizeMax) {
        m_fontSize += 2;
        emit fontSizeChanged();
    }
}

void EditorController::decreaseFontSize()
{
    if (m_fontSize > m_fontSizeMin) {
        m_fontSize -= 2;
        emit fontSizeChanged();
    }
}

EditorIndenter *EditorController::getIndenter()
{
    return EditorIndenter::instance;
}

EditorHints *EditorController::getHints()
{
    return EditorHints::instance;
}

int EditorController::getFontSize()
{
    return m_fontSize;
}

int EditorController::getCursorX()
{
    return m_cursorPositionX;
}

int EditorController::getCursorY()
{
    return m_cursorPositionY;
}

void EditorController::setCursorX(int pose)
{
    m_cursorPositionX = pose;
}

void EditorController::setCursorY(int pose)
{
    m_cursorPositionY = pose;
}

QString EditorController::getText()
{
    return m_document->textDocument()->toPlainText();
}

EditorController::~EditorController()
{
    delete m_highlighter;
    m_highlighter = nullptr;
}
} // namespace ide::ui
