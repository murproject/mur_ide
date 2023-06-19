#pragma once
#include "QmlUtils.hxx"
#include "EditorController.hxx"
#include <QObject>
#include <QString>

namespace Ide::Ui {

class EditorController;
class ApplicationMenu;
class NetworkController;
class SimulatorController;
class ApplicationLogger;
class LocalScriptsController;
class RemoteController;
class UpdateController;
class Gamepad;

class ApplicationController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Ide::Ui::EditorController *editor READ getEditor CONSTANT)
    Q_PROPERTY(Ide::Ui::ApplicationMenu *menu READ getMenu CONSTANT)
    Q_PROPERTY(Ide::Ui::NetworkController *network READ getNetwork CONSTANT)
    Q_PROPERTY(Ide::Ui::SimulatorController *simulator READ getSimulator CONSTANT)
    Q_PROPERTY(Ide::Ui::ApplicationLogger *logger READ getLogger CONSTANT)
    Q_PROPERTY(Ide::Ui::LocalScriptsController *scripts READ getScripts CONSTANT)
    Q_PROPERTY(Ide::Ui::RemoteController *image READ getImage CONSTANT)
    Q_PROPERTY(Ide::Ui::UpdateController *updates READ getUpdates CONSTANT)
    Q_PROPERTY(Ide::Ui::Gamepad *gamepad READ getGamepad CONSTANT)
    Q_PROPERTY(QString version READ getVersion CONSTANT)

public:
    EditorController *getEditor();
    ApplicationMenu *getMenu();
    NetworkController *getNetwork();
    SimulatorController *getSimulator();
    ApplicationLogger *getLogger();
    LocalScriptsController *getScripts();
    RemoteController *getImage();
    UpdateController *getUpdates();
    Gamepad *getGamepad();

    static QString getVersion() {
        return "0.1.0 Beta";
    }

    static ApplicationController *instance;
    static ApplicationController *Create();

private:
    ApplicationController();
    static qml::RegisterType<ApplicationController> Register;
};
} 
