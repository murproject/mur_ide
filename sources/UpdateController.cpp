#include "UpdateController.hxx"

#include <QApplication>
#include <QMessageBox>
#include <QtConcurrent>

namespace Ide::Ui {

UpdateController *UpdateController::instance = nullptr;
qml::RegisterType<UpdateController> UpdateController::Register;

UpdateController::UpdateController()
{
    if (instance != nullptr) {
        throw std::runtime_error("Instance of UpdateController already exists");
    }

    loadSettings();
    QtConcurrent::run([this]() { onCheckConnection(); });
}

void UpdateController::loadSettings()
{
    QSettings settings("settings.ini", QSettings::IniFormat);
    m_isCheckForUpdate = settings.value("checkForUpdate", true).toBool();
}

void UpdateController::saveSettings()
{
    QSettings settings("settings.ini", QSettings::IniFormat);
    settings.setValue("checkForUpdate", m_isCheckForUpdate);
}

UpdateController *UpdateController::Create()
{
    instance = new UpdateController();
    return instance;
}

bool UpdateController::isUpdateAvailable()
{
    return m_isUpdateAvailable;
}

bool UpdateController::isConnected()
{
    return m_isConnected;
}

bool UpdateController::isCheckForUpdate()
{
    return m_isCheckForUpdate;
}

void UpdateController::setCheckForUpdate(bool flag)
{
    m_isCheckForUpdate = flag;
    emit checkForUpdate();
    saveSettings();
}

void UpdateController::onCheckForUpdates()
{
    QTcpSocket сonnectionSocket;
    сonnectionSocket.connectToHost("google.com", 80);
    сonnectionSocket.waitForConnected(4000);

    if (!(сonnectionSocket.state() == QTcpSocket::ConnectedState)) {
        return;
    }
    сonnectionSocket.close();
    QProcess process;
    QStringList args("--checkupdates");
    process.start("maintenancetool", args);

    process.waitForFinished();

    if (process.error() != QProcess::UnknownError) {
        return;
    }

    QByteArray data = process.readAllStandardOutput();

    if (data.isEmpty()) {
        m_isUpdateAvailable = false;
        return;
    }

    if (data.contains("no updates available")) {
        m_isUpdateAvailable = false;
        return;
    }

    m_isUpdateAvailable = true;
    emit updateAvailable();
}

void UpdateController::onUpdate()
{
    QMessageBox msgBox;

    msgBox.setStyleSheet("background-color: #21252B; color: #6E7582;");
    msgBox.setText("Update process require closing the IDE.\nDo you want to proceed?");
    msgBox.addButton(QMessageBox::Ok);
    msgBox.addButton(QMessageBox::Cancel);
    int ret = msgBox.exec();

    if (ret == QMessageBox::Cancel) {
        return;
    }

    QStringList args("--updater");
    QProcess::startDetached("maintenancetool", args);
    QApplication::exit(0);
}

void UpdateController::onCheckConnection()
{
    if (!m_isCheckForUpdate) {
        return;
    }

    QProcess process;
    process.start("ping -n 1 8.8.8.8");
    process.waitForFinished(1000);

    if (process.exitCode() == 0) {
        m_isConnected = true;
       onCheckForUpdates();
        process.close();
        return;
    }

    process.close();
}

} 
