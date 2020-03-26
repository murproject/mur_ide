#pragma once

#include "QmlUtils.hxx"

#include <QAbstractListModel>

namespace Ide::Ui {

struct EditorError
{
    int line;
    int column;
    QString message;
};

class EditorErrors : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ get_count NOTIFY countChanged)

public:
    enum roles { LineRole = Qt::UserRole + 1, ColumnRole, MessageRole };

private:
    QList<EditorError> m_preList;
    QList<EditorError> m_errors;
    EditorErrors();

public:
    static qml::RegisterType<EditorErrors> Register;
    static EditorErrors *instance;
    static void Create();
    int rowCount(const QModelIndex &) const override;
    QVariant data(const QModelIndex &, int) const override;
    QHash<int, QByteArray> roleNames() const override;
    int get_count();
public slots:
    void update();
    void add(int, int, const QString &);
    void clear();
signals:
    void countChanged();
};
}
