#include "HintBase.hxx"

namespace Ide::Ui {

HintBase::HintBase(QTextDocument *document)
    : m_textDocument(document)
{}

int HintBase::getLinesCount() {
    return editorSelection::instance->getEndLine() - editorSelection::instance->getStartLine() + 1;
}

}
