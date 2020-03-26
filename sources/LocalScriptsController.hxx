#pragma once

#include "QmlUtils.hxx"
#include <QObject>
#include <QProcess>

namespace Ide::Ui {
class LocalScriptsController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool running READ isRunning NOTIFY runningStateChanged)
    Q_PROPERTY(bool local READ isLocal NOTIFY targetStateChanged)

public:
    static LocalScriptsController *instance;
    static LocalScriptsController *Create();
    void run();
    void stop();
    bool isRunning();
    bool isLocal();
    void setLocal();
    void setRemote();
    ~LocalScriptsController();
signals:
    void runningStateChanged();
    void targetStateChanged();
private:
    LocalScriptsController();
    static qml::RegisterType<LocalScriptsController> Register;
    void setupProcess();
    QProcess *m_scriptProcess = nullptr;
    Q_PID m_pid;
    bool m_isLocal = false;
};
} // namespace ide::ui
