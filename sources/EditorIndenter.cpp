#include "EditorIndenter.hxx"
#include "EditorSelection.hxx"
#include "EditorUtils.hxx"

#include <QDebug>
#include <QTextBlock>
#include <QTextCursor>

#include <tuple>

namespace Ide::Ui {

EditorIndenter *EditorIndenter::instance = nullptr;
qml::RegisterType<EditorIndenter> EditorIndenter::Register;

static inline QString ltrim(QString string)
{
    for (int pose = 0; pose < string.size(); ++pose) {
        if (!string.at(pose).isSpace()) {
            return string.right(string.size() - pose);
        }
    }
    return {};
}

static inline bool isCharOpenBracket(const QChar &ch)
{
    if (ch == '(' || ch == '[' || ch == '{') {
        return true;
    }
    return false;
}

static inline bool isCharCloseBracket(const QChar &ch)
{
    if (ch == ')' || ch == ']' || ch == '}') {
        return true;
    }
    return false;
}

static std::tuple<QVector<QPair<int, int>>, QPair<int, int>, bool, int> parseText(QString text)
{
    QVector<QPair<int, int>> openBracket;
    QPair<int, int> lastClosedLine = {-1, -1};
    int lastColonLine = -1;
    bool shouldHang = false;

    auto lines = text.split(QRegularExpression("[\r\n]"), Qt::KeepEmptyParts);

    for (auto line = 0; line < lines.size(); ++line) {
        auto currentLine = lines.at(line);
        for (auto idx = 0; idx < lines.at(line).size(); ++idx) {
            if (isCharOpenBracket(currentLine.at(idx))) {
                openBracket.append({line, idx});
                shouldHang = true;
            } else {
                shouldHang = false;
                if (currentLine.at(idx) == ':') {
                    lastColonLine = line;
                } else if (isCharCloseBracket(currentLine.at(idx)) && !openBracket.empty()) {
                    auto openedRow = openBracket.front().first;
                    openBracket.removeFirst();
                    if (line != openedRow) {
                        lastClosedLine = {openedRow, line};
                    }
                }
            }
        }
    }
    return {openBracket, lastClosedLine, shouldHang, lastColonLine};
}

int EditorIndenter::blockIndentation(QTextBlock &block)
{
    auto prevBlock = block.previous();
    if (prevBlock.position() < 0) {
        return 0;
    }
    return prevBlock.text().size() - ltrim(prevBlock.text()).size();
}

QString EditorIndenter::indentationToInsert(int n)
{
    QString result;
    for (int idx = 0; idx < n; ++idx) {
        result.append(" ");
    }
    return result;
}

bool EditorIndenter::isIndentationGoesIn(const QTextBlock &block)
{
    auto prevBlock = block.previous();

    if (prevBlock.text().trimmed().endsWith(":")) {
        return true;
    }

    foreach(QString item, startOfCodeBlock ) {
        if (prevBlock.text().trimmed().startsWith(item)) {
            return true;
        }
    }

    return false;
}

bool EditorIndenter::isIndentationGoesOut(const QTextBlock &block)
{
    auto prevBlock = block.previous();

    foreach(QString item, endOfCodeBlock) {
        if (prevBlock.text().trimmed().endsWith(item)) {
            return true;
        }
    }

    return false;
}

EditorIndenter::EditorIndenter()
{
    if (instance != nullptr) {
        throw std::runtime_error("Instance of editor_controller already exists");
    }
}

void EditorIndenter::Create()
{
    instance = new EditorIndenter;
}

void EditorIndenter::setDocument(QTextDocument *doc)
{
    m_textDocument = doc;
    m_prevBlockCount = doc->blockCount();
    connect(doc, &QTextDocument::blockCountChanged, this, &EditorIndenter::onTextModified);

    startOfCodeBlock << "def"
                     << "if"
                     << "else"
                     << "elif"
                     << "while"
                     << "for"
                     << "class";

    endOfCodeBlock << "break"
                   << "return"
                   << "continue"
                   << "pass"
                   << "raise";
}

void EditorIndenter::setIndent(int indent)
{
    cursor.insertText(indentationToInsert(indent));
    cursor.endEditBlock();
}

bool EditorIndenter::ltrimDocumentLine() {
    auto line = editorSelection::instance->getStartLine();
    auto docText = m_textDocument->toPlainText();

    QTextCursor lineStart(m_textDocument->findBlockByLineNumber(line));
    lineStart.movePosition(QTextCursor::StartOfLine);

    QTextCursor lineEnd(m_textDocument->findBlockByLineNumber(line));
    lineEnd.movePosition(QTextCursor::EndOfLine);

    auto lineText = docText.mid(lineStart.position(), lineEnd.position() - lineStart.position());
    auto lineTextTrimmed = ltrim(lineText);

    if (lineTextTrimmed.size() <= 0 && lineText.size() >= 0) {
        cursor.movePosition(QTextCursor::EndOfLine);
        for (int i = 0; i < lineText.size(); i++) {
            cursor.deletePreviousChar();
        }
        return true;
    } else {
        return false;
    }
}

void EditorIndenter::setIndentationBlock(bool state) {
    isIndentationBlocked = state;
}

void EditorIndenter::onTextModified()
{
    if (isIndentationBlocked) {
        return;
    }

    auto isNotNewBlock = (m_textDocument->blockCount() < m_prevBlockCount);
    m_prevBlockCount = m_textDocument->blockCount();

    auto line = editorSelection::instance->getStartLine();
    cursor = QTextCursor(m_textDocument->findBlockByLineNumber(line));
    cursor.beginEditBlock();
    auto block = cursor.block();
    auto currentIndent = blockIndentation(block);
    auto text = m_textDocument->toPlainText().mid(0, cursor.position());
    auto [bracketStack, lastClosedLine, shouldHang, lastColonLine] = parseText(text);

    auto prevBlock = cursor.block().previous();
    auto nextBlock = cursor.block().next();

    if (isNotNewBlock) {
        bool isTrimmed = ltrimDocumentLine();
        if (!isTrimmed) {
            cursor.endEditBlock();
            return;
        }
    }

    if (shouldHang) {
        if (isIndentationGoesIn(block)) {
            setIndent(currentIndent + indentWidth*2);
            return;
        }
        setIndent(currentIndent + indentWidth);
        return;
    }

    if (!bracketStack.empty()) {
        if (currentIndent > bracketStack.first().second) {
            setIndent(currentIndent);
            return;
        }
        if (currentIndent == 0) {
            setIndent( bracketStack.last().second + 1 );
            return;
        }
    }

    if (lastClosedLine.first != -1 && lastClosedLine.second != -1) {
        if (lastClosedLine.second == line - 1) {
            auto closedBlock = m_textDocument->findBlockByLineNumber(lastClosedLine.first + 1);
            auto indentLevel = blockIndentation(closedBlock);
            if (lastColonLine == line - 1) {
                indentLevel += indentWidth;
            }
            setIndent(indentLevel);
            return;
        }
    }

    if (isIndentationGoesOut(block)) {
        setIndent(currentIndent - indentWidth);
        return;
    } else if (blockIndentation(nextBlock) == 0) {
        if (isIndentationGoesIn(block)) {
            setIndent(currentIndent + indentWidth);
            return;
        }
        cursor.joinPreviousEditBlock();
        setIndent(currentIndent);
        cursor.endEditBlock();
        return;
    }

    cursor.endEditBlock();
}

void EditorIndenter::computeIndent()
{
    auto line = editorSelection::instance->getStartLine();
    QTextCursor cursor(m_textDocument->findBlockByLineNumber(line));
}

}
