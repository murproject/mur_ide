#pragma once

#include <QString>
#include <QSyntaxHighlighter>

namespace Ide::Ui {

class EditorHighlighter : public QSyntaxHighlighter
{
private:
  struct Key {
    static constexpr auto highlight = "highlight";
    static constexpr auto scheme = "scheme";
    static constexpr auto italic = "italic";
    static constexpr auto color = "color";
    static constexpr auto match = "match";
    static constexpr auto bold = "bold";
  };
  struct Rule {
    QRegExp pattern;
    QTextCharFormat format;
  };
  QList<Rule> m_rules;

public:
    explicit EditorHighlighter(QTextDocument *);
    void init();
    void highlightBlock(const QString &) override;
};
}
