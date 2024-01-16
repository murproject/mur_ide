#include "EditorHighlighter.hxx"
#include "Application.hxx"
#include "TextIO.hxx"

#include <QJsonObject>
#include <QTextBlock>
#include <QDebug>

namespace Ide::Ui {

EditorHighlighter::EditorHighlighter(QTextDocument *document)
    : QSyntaxHighlighter(document) {
  init();
}

void EditorHighlighter::init() {
  auto json = IO::Read::jsonFromUrl(Ide::Ui::Application::getResourcesDirectory() + "/config/schemes/py.json");
  auto rootObject = json[Key::scheme].toObject();
  auto ruleObjects = rootObject[Key::highlight].toArray();

  QTextCharFormat format;
  Rule rule;
  QJsonObject ruleObject;

  for (auto i = 0; i < ruleObjects.count(); ++i) {
    ruleObject = ruleObjects.at(i).toObject();

    format.setFontWeight(QFont::Normal);

    format.setFontItalic(ruleObject[Key::italic].toBool(false));

    format.setForeground(QColor(ruleObject[Key::color].toString("#ffffff")));

    rule.pattern = QRegExp(ruleObject[Key::match].toString());
    rule.format = format;

    m_rules.append(rule);
  }
}

void EditorHighlighter::highlightBlock(const QString &text) {
  for (const auto &rule : qAsConst(this->m_rules)) {
    auto expression = QRegExp(rule.pattern);
    auto index = expression.indexIn(text);

    while (index >= 0) {
      auto length = expression.matchedLength();
      setFormat(index, length, rule.format);
      index = expression.indexIn(text, index + length);
    }
  }
}
}
