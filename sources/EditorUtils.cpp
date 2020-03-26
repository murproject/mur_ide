#include "EditorUtils.hxx"
#include "EditorController.hxx"
#include "EditorSelection.hxx"

#include <QTextBlock>

namespace Ide::Ui {

QTextDocument *EditorUtils::document = nullptr;

QString EditorUtils::getLine(const int line) {
  auto block = document->findBlockByLineNumber(line);

  return block.isValid() ? block.text() : nullptr;
}

QTextCursor EditorUtils::getCursor(const int position) {
  auto cursor = QTextCursor(document);

  cursor.setPosition(position);
  return cursor;
}

QChar EditorUtils::getChar(const int position) {
  return document->characterAt(position);
}
}
