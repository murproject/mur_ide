#include "ApplicationMenu.hxx"
#include "Application.hxx"
#include "ApplicationController.hxx"
#include "ApplicationLogger.hxx"
#include "EditorController.hxx"
#include "LocalScriptsController.hxx"
#include "NetworkController.hxx"
#include "RemoteController.hxx"
#include "SimulatorController.hxx"
#include "TextIO.hxx"

#include <QDesktopServices>
#include <QMenuBar>
#include <QApplication>

namespace Ide::Ui {

ApplicationMenu *ApplicationMenu::instance = nullptr;
qml::RegisterType<ApplicationMenu> ApplicationMenu::Register;

ApplicationMenu::ApplicationMenu() {
  if (instance) {
    throw std::runtime_error("Instance of application_menu already exists");
  }

  init();
}

void ApplicationMenu::Create() {
  instance = new ApplicationMenu();
}

void ApplicationMenu::init()
{
    m_examples = IO::fileNamesFromDir(Application::instance->getResourcesDirectory()
                                             + "examples/",
                                         {"*.py"},
                                         IO::FileSuffix::On);
}

QStringList ApplicationMenu::getExamples()
{
    return m_examples;
}

void ApplicationMenu::onFileNew()
{
    EditorController::instance->newFile();
}

void ApplicationMenu::onFileOpen()
{
    EditorController::instance->openFile();
}

void ApplicationMenu::onFileSave()
{
    EditorController::instance->saveFile();
}

void ApplicationMenu::onFileSaveAs()
{
    EditorController::instance->saveFileAs();
}

void ApplicationMenu::onFindFind()
{
    EditorController::instance->showSearch();
}

void ApplicationMenu::onCodeRun()
{
    if (LocalScriptsController::instance->isLocal()) {
        LocalScriptsController::instance->run();
    } else if (NetworkController::instance->getConnectionStatus()) {
        NetworkController::instance->run();
        ApplicationLogger::instance->addEntry("<br>- Program started.<br>");
    }
}

void ApplicationMenu::onCodeStop()
{
    if (LocalScriptsController::instance->isLocal()) {
        LocalScriptsController::instance->stop();
        ApplicationLogger::instance->addEntry("- Program stopped.");
    } else if (NetworkController::instance->getConnectionStatus()) {
        NetworkController::instance->stop();
    }
}

void ApplicationMenu::onRunSimulator()
{
    SimulatorController::instance->run();
}

void ApplicationMenu::onRunRemote()
{
    if (LocalScriptsController::instance->isLocal()) {
        return;
    }

    if (!RemoteController::instance->isReadingImages()) {
        RemoteController::instance->startImageCapture();
        NetworkController::instance->remote();
    } else {
        RemoteController::instance->stopImageCapture();
        NetworkController::instance->stopRemote();
    }
}

void ApplicationMenu::onTargetModeChanged()
{
    if (LocalScriptsController::instance->isLocal()) {
        LocalScriptsController::instance->setRemote();
    } else {
        LocalScriptsController::instance->setLocal();
    }
}

void ApplicationMenu::onViewIncreaseFontSize()
{
    EditorController::instance->increaseFontSize();
}

void ApplicationMenu::onViewDecreaseFontSize()
{
    EditorController::instance->decreaseFontSize();
}

void ApplicationMenu::onViewResetFontSize()
{
    EditorController::instance->resetFontSize();
}

void ApplicationMenu::onViewToggleEditor()
{
    EditorController::instance->toggleExpanded();
}

void ApplicationMenu::onHelpExample(const QString &exampleName)
{
    EditorController::instance->openFile(Application::getResourcesDirectory() + "examples/" + exampleName);
}

void ApplicationMenu::onRestart() {
    QProcess::startDetached(QApplication::applicationFilePath());

    if (QSysInfo::kernelType() == "winnt") { 
        qApp->quit();
    } else {
        std::exit(EXIT_SUCCESS);
    }
}
}
