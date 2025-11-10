#pragma once

#include "QmlUtils.hxx"
#include <QObject>
#include <QTimer>

namespace Ide::Ui {

class ApplicationLogger : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString output READ getOutput NOTIFY outputChanged)
public:
    static ApplicationLogger *instance;
    static ApplicationLogger *Create();

    void addEntry(const QString &);
    void addScriptOutput(const QString &);
    void addOutput(const QString &, const QString &);
    QString getOutput();

public slots:
    void clear();

signals:
    void outputReceived(const QString &, const QString &);
    void outputChanged();

private:
    ApplicationLogger();
    void onUpdate();

    static qml::RegisterType<ApplicationLogger> Register;
    QString m_output;
    QTimer *m_updateViewTimer;
};

}
