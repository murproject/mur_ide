#pragma once

#include "QmlUtils.hxx"
#include <QObject>
#include <QTimer>

namespace Ide::Ui {

class ApplicationLogger : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList entries READ getEntries NOTIFY entriesChanged)
public:
    static ApplicationLogger *instance;
    static ApplicationLogger *Create();

    void addEntry(const QString &);
    QStringList getEntries();

public slots:
    void clear();

signals:
    void entriesChanged();

private:
    ApplicationLogger();
    void onUpdate();

    static qml::RegisterType<ApplicationLogger> Register;
    QStringList m_entries;
    QTimer *m_updateViewTimer;
};

} // namespace ide::ui
