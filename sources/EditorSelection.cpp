#include "EditorSelection.hxx"

#include <QTextBlock>
#include <QTextCursor>

namespace Ide::Ui {

editorSelection *editorSelection::instance = nullptr;
qml::RegisterType<editorSelection> editorSelection::Register;

editorSelection::editorSelection() {
    if (instance) {
        throw std::runtime_error("Instance of editor_selection already exists");
    }
}

void editorSelection::Create()
{
    instance = new editorSelection();
}

void editorSelection::setDocument(QTextDocument *document)
{
    m_document = document;
}

void editorSelection::setStartPosition(int startPosition) {
    if (m_startPosition != startPosition) {
        m_startPosition = startPosition;
        emit startPositionChanged();

        auto cursor = QTextCursor(m_document);

        cursor.setPosition(m_startPosition);

        setStartColumn(cursor.columnNumber());
        setStartLine(cursor.blockNumber());

        setEmpty(m_startPosition == m_endPosition);
    }
}

int editorSelection::getStartPosition()
{
    return m_startPosition;
}

void editorSelection::setEndPosition(int endPosition)
{
    if (m_endPosition != endPosition) {
        m_endPosition = endPosition;
        emit endPositionChanged();

        auto cursor = QTextCursor(m_document);

        cursor.setPosition(m_endPosition);

        setEndColumn(cursor.columnNumber());
        setEndLine(cursor.blockNumber());

        setEmpty(m_startPosition == m_endPosition);
    }
}

int editorSelection::getEndPosition()
{
    return m_endPosition;
}

void editorSelection::setStartColumn(int startColumn)
{
    if (m_startColumn != startColumn) {
        m_startColumn = startColumn;
        emit startColumnChanged();
    }
}

int editorSelection::getStartColumn()
{
    return m_startColumn;
}

void editorSelection::setEndColumn(int endColumn)
{
    if (m_endColumn != endColumn) {
        m_endColumn = endColumn;
        emit endColumnChanged();
    }
}

int editorSelection::getEndColumn()
{
    return m_endColumn;
}

void editorSelection::setStartLine(int startLine)
{
    if (m_startLine != startLine) {
        m_startLine = startLine;
        emit startLineChanged();
    }
}

int editorSelection::getStartLine()
{
    return m_startLine;
}

void editorSelection::setEndLine(int endLine)
{
    if (m_endLine != endLine) {
        m_endLine = endLine;
        emit endLineChanged();
    }
}

int editorSelection::getEndLine()
{
    return m_endLine;
}

void editorSelection::setEmpty(bool empty)
{
    if (m_empty != empty) {
        m_empty = empty;
        emit emptyChanged();
    }
}

bool editorSelection::isEmpty()
{
    return m_empty;
}
}
