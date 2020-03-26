#pragma once

#include "QmlUtils.hxx"

#include <QMap>
#include <QObject>
#include <QSharedPointer>
#include <QTextDocument>

namespace Ide::Ui {

class HintBase;

class EditorHints : public QObject
{
    Q_OBJECT
public:
    //TODO(Alexey): Make enum's names PascalCased
    enum HintType { HINT_COMMENT, HINT_INSERT_INDENT, HINT_REMOVE_INDENT };
    Q_ENUMS(HintType)

    static qml::RegisterType<EditorHints> Register;

    static EditorHints *instance;

    static void Create();

    void setDocument(QTextDocument *);
    EditorHints();

public slots:
    void applyHint(HintType);

private:
    static void declareQML();
    QMap<HintType, QSharedPointer<HintBase>> hints;
    QTextDocument *m_textDocument = nullptr;
};

} // namespace ide::ui
