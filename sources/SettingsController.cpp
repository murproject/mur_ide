#include "SettingsController.hxx"
#include <QDebug>

namespace Ide::Ui {

SettingsController *SettingsController::instance = nullptr;

bool isValidPath(QString path) {
    const QFileInfo target(path);
    if (target.exists() && target.isReadable()) {
        return true;
    }
    return false;
}

SettingsController::SettingsController()
{
    if (instance != nullptr) {
        throw std::runtime_error("Instance of settings_controller already exists");
    }

    if (isValidPath("settings.ini")) {
        settings = new QSettings("settings.ini", QSettings::IniFormat);
    } else {
        settings = new QSettings(QSettings::IniFormat, QSettings::UserScope, "mur-ide", "settings");
    }

    m_resourcePath = settings->value("Paths/resources").toString();
    m_pythonPath = settings->value("Paths/python").toString();
    m_simulatorPath = settings->value("Paths/simulator").toString();

    auto os = QSysInfo::kernelType();
    auto path = QDir{QApplication::applicationDirPath()};

    if (os == "winnt") {
        if (!isValidPath(m_resourcePath)) {
            m_resourcePath = path.absolutePath() + "/resources/";
        }

        if (!isValidPath(m_pythonPath)) {
            m_pythonPath = m_resourcePath + "/python/python.exe";
        }

        if (!isValidPath(m_simulatorPath)) {
            m_simulatorPath = m_resourcePath + "/simulator/murSimulator.exe";
        }
    }

    if (os == "linux") {
        if (!isValidPath(m_resourcePath)) {
            m_resourcePath = "/usr/local/share/mur-ide/";
        }

        if (!isValidPath(m_pythonPath)) {
            m_pythonPath = "venv/bin/python3";
        }

        if (!isValidPath(m_simulatorPath)) {
            m_simulatorPath = "simulator/simulator"; 

            if (!isValidPath(m_simulatorPath)) {
                m_simulatorPath = "mur-ide.simulator"; 
            }
        }
    }

    settings->setValue("Paths/resources", m_resourcePath);
    settings->setValue("Paths/python", m_pythonPath);
    settings->setValue("Paths/simulator", m_simulatorPath);

    qInfo() << "paths:"        << Qt::endl
            << "  resources: " << m_resourcePath  << Qt::endl
            << "  python:    " << m_pythonPath    << Qt::endl
            << "  simulator: " << m_simulatorPath << Qt::endl;
}

SettingsController *SettingsController::Create()
{
    instance = new SettingsController();

    return instance;
}

QString SettingsController::getResourcePath()
{
    return m_resourcePath;
}

QString SettingsController::getPythonPath()
{
    return m_pythonPath;
}

QString SettingsController::getSimulatorPath()
{
    return m_simulatorPath;
}

QString SettingsController::getSettingsLocation()
{
    return settings->fileName();
}

}
