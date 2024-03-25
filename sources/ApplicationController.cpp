#include "ApplicationController.hxx"

namespace Ide::Ui {

ApplicationController *ApplicationController::instance = nullptr;
qml::RegisterType<ApplicationController> ApplicationController::Register;

ApplicationController::ApplicationController()
{
    if (instance != nullptr) {
        throw std::runtime_error("Instance of application_controller already exists");
    }
}

EditorController *ApplicationController::getEditor()
{
    return EditorController::instance;
}

ApplicationMenu *ApplicationController::getMenu()
{
    return ApplicationMenu::instance;
}

NetworkController *ApplicationController::getNetwork()
{
    return NetworkController::instance;
}

SimulatorController *ApplicationController::getSimulator()
{
    return SimulatorController::instance;
}

ApplicationLogger *ApplicationController::getLogger()
{
    return ApplicationLogger::instance;
}

LocalScriptsController *ApplicationController::getScripts()
{
    return LocalScriptsController::instance;
}

RemoteController *ApplicationController::getImage()
{
    return RemoteController::instance;
}

UpdateController *ApplicationController::getUpdates()
{
    return UpdateController::instance;
}

Joystick *ApplicationController::getJoystick()
{
    return Joystick::instance;
}

ApplicationController *ApplicationController::Create()
{
    instance = new ApplicationController();
    return instance;
}

bool ApplicationController::developerMode()
{
    QSettings settings("settings.ini", QSettings::IniFormat);
    bool mode = settings.value("DevMode/active", false).toBool();
    return mode;
}

} 
