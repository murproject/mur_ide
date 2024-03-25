#include "EditorSearch.hxx"

#include "EditorController.hxx"

namespace Ide::Ui {

EditorSearch *EditorSearch::instance = nullptr;
qml::RegisterType<EditorSearch> EditorSearch::Register;

EditorSearch::EditorSearch() {
  if (instance) {
    throw "instance already existing";
  }
}

void EditorSearch::setDocument(QTextDocument *document)
{
    m_document = document;
}

auto EditorSearch::Create() -> void {
  instance = new EditorSearch();
}

void EditorSearch::setVisible(bool visible)
{
    if (m_visible != visible) {
        m_visible = visible;
        emit visibleChanged();
    }
}

bool EditorSearch::getVisible()
{
    return m_visible;
}

void EditorSearch::replaceNext()
{
    if (!m_valid) {
        processSearch();
        return;
    }

    if (m_matches.count() < 1) {
        return;
    }

    if (m_currentMatch == m_matches.count() - 1) {
        auto cursor = m_matches.at(m_currentMatch);
        cursor.insertText(m_replaceString);
        processSearch();
        return;
    }

    auto cursor = m_matches.at(m_currentMatch);
    cursor.insertText(m_replaceString);

    findNext();
}

void EditorSearch::replaceAll() {
    if (!m_valid) {
        processSearch();
        return;
    }

    if (m_matches.count() < 1) {
        return;
    }

    for(int i = 0; i < m_matches.count(); i ++)
    {
        auto cursor = m_matches.at(m_currentMatch);
        cursor.insertText(m_replaceString);
        findNext();
    }
    processSearch();
}

void EditorSearch::setRegexError(const QString &regex_error)
{
    m_regexError = regex_error;
    emit regexErrorChanged();
}

QString EditorSearch::getRegexError()
{
    return m_regexError;
}

void EditorSearch::setRegexValid(bool regex_valid)
{
    if (m_regexValid != regex_valid) {
        m_regexValid = regex_valid;
        emit regexValidChanged();
    }
}

bool EditorSearch::getRegexValid()
{
    return m_regexValid;
}

void EditorSearch::setUseRegex(bool use_regex)
{
    if (m_useRegex != use_regex) {
        m_useRegex = use_regex;
        emit useRegexChanged();

        checkRegex();
        processSearch();
    }
}

bool EditorSearch::getUseRegex()
{
    return m_useRegex;
}

void EditorSearch::setCaseSensitive(bool case_sensitive)
{
    if (m_caseSensitive != case_sensitive) {
        m_caseSensitive = case_sensitive;
        emit caseSensitiveChanged();

        processSearch();
    }
}

bool EditorSearch::getCaseSensitive()
{
    return m_caseSensitive;
}

void EditorSearch::setReplaceString(const QString &replaceString)
{
    m_replaceString = replaceString;
    emit replaceStringChanged();
}

QString EditorSearch::getReplaceString()
{
    return m_replaceString;
}

void EditorSearch::checkRegex()
{
    if (m_useRegex) {
        if (m_findString.isEmpty()) {
            setRegexValid(false);
            setRegexError("Regex is empty");
        } else {
            auto regex = QRegularExpression(m_findString);
            auto valid = regex.isValid();

            if (!valid) {
                setRegexError(regex.errorString());
            } else {
                setRegexError("");
            }

            setRegexValid(valid);
        }
    } else {
        setRegexValid(true);
        setRegexError("");
    }
}

void EditorSearch::setFindString(const QString &findString)
{
    if (m_findString.compare(findString) != 0 || !m_valid) {
        m_findString = findString;
        emit findStringChanged();
        checkRegex();
        processSearch();
    } else {
        findNext();
    }
}

QString EditorSearch::getFindString()
{
    return m_findString;
}

void EditorSearch::setCurrentMatch(int currentMatch)
{
    if (m_currentMatch != currentMatch) {
        m_currentMatch = currentMatch;
        emit currentMatchChanged();
    }
}

int EditorSearch::getCurrentMatch()
{
    return m_currentMatch;
}

int EditorSearch::getMatchCount()
{
    return m_matches.count();
}

void EditorSearch::invalidate()
{
    m_valid = false;
}

int EditorSearch::getStartPosition()
{
    return m_startPosition;
}

int EditorSearch::getEndPosition()
{
    return m_endPosition;
}

void EditorSearch::processSearch()
{
    if (m_useRegex && !m_regexValid) {
        return;
    }

    setCurrentMatch(-1);
    m_matches.clear();

    auto cursor = QTextCursor(m_document);

    cursor.setPosition(0);

    while (true) {
        if (m_useRegex) {
            auto newCursor = m_document->find(QRegularExpression(m_findString), cursor);
            if (newCursor.position() == cursor.position()) {
                return;
            }
            cursor = newCursor;
        } else {
            if (m_caseSensitive) {
                cursor = m_document->find(m_findString, cursor, QTextDocument::FindCaseSensitively);
            } else {
                cursor = m_document->find(m_findString, cursor);
            }
        }

        if (cursor.position() > -1) {
            m_matches.append(cursor);
        } else {
            break;
        }
    }

    m_valid = true;

    emit matchCountChanged();

    findNext();
}

void EditorSearch::findNext()
{
    if (!m_valid) {
        processSearch();
        return;
    }

    if (m_matches.count() < 1) {
        return;
    }
    setCurrentMatch((m_currentMatch + 1) % m_matches.count());

    auto match = m_matches.at(m_currentMatch);
    m_startPosition = match.selectionStart();
    m_endPosition = match.selectionEnd();
    emit EditorController::instance->select(match.selectionStart(), match.selectionEnd());
}

void EditorSearch::findPrevious()
{
    if (!m_valid) {
        processSearch();
        return;
    }

    if (m_matches.count() < 1) {
        return;
    }

    setCurrentMatch((m_currentMatch - 1) % m_matches.count());

    auto match = m_matches.at(m_currentMatch);
    emit EditorController::instance->select(match.selectionStart(), match.selectionEnd());
}
} // namespace ide::ui
