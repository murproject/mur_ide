#include "EditorHints.hxx"
#include "HintBase.hxx"

#include <QQmlEngine>

namespace Ide::Ui {

EditorHints *EditorHints::instance = nullptr;
qml::RegisterType<EditorHints> EditorHints::Register;

class HintInsertIndent final : public HintBase
{
public:
    explicit HintInsertIndent(QTextDocument *document)
        : HintBase(document)
    {}

    void apply() override
    {
        QTextCursor cursor(m_textDocument);

        cursor.setPosition(editorSelection::instance->getStartPosition());
        cursor.beginEditBlock();

        for (size_t i = 0; i < getLinesCount(); i++) {
            cursor.movePosition(QTextCursor::StartOfLine);
            cursor.insertText("    ");
            cursor.movePosition(QTextCursor::Down);
        }

        cursor.endEditBlock();
    }
};

class HintRemoveIndent final : public HintBase
{
public:
    explicit HintRemoveIndent(QTextDocument *document)
        : HintBase(document)
    {}

    void apply() override
    {
        QTextCursor cursor(m_textDocument);

        cursor.setPosition(editorSelection::instance->getStartPosition());
        cursor.beginEditBlock();

        for (size_t currentLine = 0; currentLine < getLinesCount(); ++currentLine) {
            cursor.select(QTextCursor::LineUnderCursor);
            auto text = cursor.selectedText();
            if (text.size() - text.trimmed().size() >= indentWidth) {
                cursor.movePosition(QTextCursor::StartOfLine);
                for (size_t ch = 0; ch < indentWidth; ++ch) {
                    cursor.deleteChar();
                }
            }
            cursor.movePosition(QTextCursor::Down);
        }
        cursor.endEditBlock();
    }
};

class HintComment final : public HintBase
{
public:
    explicit HintComment(QTextDocument *document)
        : HintBase(document)
    {}

    void apply() override
    {
        QTextCursor cursor(m_textDocument);

        cursor.setPosition(editorSelection::instance->getStartPosition());
        cursor.beginEditBlock();

        cursor.movePosition(QTextCursor::StartOfLine);
        cursor.select(QTextCursor::LineUnderCursor);

        if (cursor.selectedText().trimmed().startsWith("#"))
        {
            cursor.clearSelection();
            for (size_t i = 0; i < getLinesCount(); ++i) {
                cursor.movePosition(QTextCursor::StartOfLine);
                cursor.select(QTextCursor::LineUnderCursor);
                if (cursor.selectedText().trimmed().startsWith("#")) {
                    auto commentStartIndex = cursor.selectedText().indexOf("#");
                    cursor.clearSelection();
                    cursor.movePosition(QTextCursor::StartOfLine);
                    cursor.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, commentStartIndex);
                    cursor.deleteChar();
                }
                cursor.movePosition(QTextCursor::Down);
            }
        } else {
            cursor.clearSelection();
            for (size_t i = 0; i < getLinesCount(); ++i) {
                cursor.movePosition(QTextCursor::StartOfLine);
                cursor.insertText("#");
                cursor.movePosition(QTextCursor::Down);
            }
        }
        cursor.endEditBlock();
    }
};

EditorHints::EditorHints() {
    if (instance != nullptr) {
        throw std::runtime_error("Instance of EditorHints already exists");
    }
}

void EditorHints::Create()
{
    declareQML();
    instance = new EditorHints();
}

void EditorHints::applyHint(HintType type) {
    if (hints.contains(type)) {
        hints[type]->apply();
    }
}

void EditorHints::setDocument(QTextDocument *doc) {
    m_textDocument = doc;

    hints[HINT_COMMENT] = QSharedPointer<HintComment>::create(m_textDocument);
    hints[HINT_INSERT_INDENT] = QSharedPointer<HintInsertIndent>::create(m_textDocument);
    hints[HINT_REMOVE_INDENT] = QSharedPointer<HintRemoveIndent>::create(m_textDocument);
}

void EditorHints::declareQML() {
    qmlRegisterType<EditorHints>("Hints", 1, 0, "Hints");
}

} 
