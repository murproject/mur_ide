#pragma once
#include "QmlUtils.hxx"
#include "EditorController.hxx"
#include "ApplicationLogger.hxx"
#include "ApplicationMenu.hxx"
#include "RemoteController.hxx"
#include "LocalScriptsController.hxx"
#include "NetworkController.hxx"
#include "SimulatorController.hxx"
#include "UpdateController.hxx"
#include "Joystick.hxx"
#include <QObject>
#include <QString>

namespace Ide::Ui {

class ApplicationController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(EditorController *editor READ getEditor CONSTANT)
    Q_PROPERTY(ApplicationMenu *menu READ getMenu CONSTANT)
    Q_PROPERTY(NetworkController *network READ getNetwork CONSTANT)
    Q_PROPERTY(SimulatorController *simulator READ getSimulator CONSTANT)
    Q_PROPERTY(ApplicationLogger *logger READ getLogger CONSTANT)
    Q_PROPERTY(LocalScriptsController *scripts READ getScripts CONSTANT)
    Q_PROPERTY(RemoteController *image READ getImage CONSTANT)
    Q_PROPERTY(UpdateController *updates READ getUpdates CONSTANT)
    Q_PROPERTY(Ide::Ui::Joystick *joystick READ getJoystick CONSTANT)
    Q_PROPERTY(QString version READ getVersion CONSTANT)
    Q_PROPERTY(bool devMode READ developerMode CONSTANT)

public:
    EditorController *getEditor();
    ApplicationMenu *getMenu();
    NetworkController *getNetwork();
    SimulatorController *getSimulator();
    ApplicationLogger *getLogger();
    LocalScriptsController *getScripts();
    RemoteController *getImage();
    UpdateController *getUpdates();
    Joystick *getJoystick();

    static bool developerMode();
    static QString getVersion() {
        return "0.1.0";
    }

    static ApplicationController *instance;
    static ApplicationController *Create();

private:
    ApplicationController();
    static qml::RegisterType<ApplicationController> Register;
};
} 
