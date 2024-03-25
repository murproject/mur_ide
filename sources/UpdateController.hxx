#pragma once

#include "QmlUtils.hxx"
#include <QObject>
#include <QProcess>

namespace Ide::Ui {
class UpdateController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool updateAvailable READ isUpdateAvailable NOTIFY updateAvailable)
    Q_PROPERTY(bool connected READ isConnected)
    Q_PROPERTY(bool checkUpdate READ isCheckForUpdate WRITE setCheckForUpdate NOTIFY checkForUpdate)

public:
    static UpdateController *instance;
    static UpdateController *Create();

    bool isUpdateAvailable();
    bool isConnected();
    bool isCheckForUpdate();

public slots:

    void setCheckForUpdate(bool);
    void onCheckForUpdates();
    void onUpdate();
    void onCheckConnection();

signals:
    void updateAvailable();
    void checkForUpdate();

private:
    UpdateController();

    void loadSettings();

    void saveSettings();

    static qml::RegisterType<UpdateController> Register;

    bool m_isUpdateAvailable = false;
    bool m_isConnected = false;
    bool m_isCheckForUpdate = false;
};

} 
