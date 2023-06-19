#include "Application.hxx"
#include "ApplicationController.hxx"
#include "QmlImageItem.hxx"
#include "QmlUtils.hxx"
#include "RemoteController.hxx"

#include <QApplication>
#include <QDebug>
#include <QDirIterator>
#include <QFontDatabase>
#include <QMessageBox>
#include <QQmlApplicationEngine>
#include <QSurfaceFormat>
#include <QWindow>
#include <QtWebEngine>
#include <QQuickStyle>

namespace Ide::Ui {

Application *Application::instance = nullptr;
QString Application::m_resourceDirectory = {};

int Application::execute(int argc, char **argv)
{
    QApplication app(argc, argv);
    QtWebEngine::initialize();

    if (instance == nullptr) {
        instance = new Application;

        Ide::Ui::Application::initialize();
        return QApplication::exec();
    } 
    return 0xDEADBEAF;
}

QString Application::getResourcesDirectory()
{
    return m_resourceDirectory;
}

void Application::initialize()
{
    auto path = QDir{QApplication::applicationDirPath()};

    for (const auto &c : path.absolutePath()) {
        if (c.unicode() > 127) {
            QMessageBox::critical(nullptr,
                                  "Error (Ошибка)",
                                  "Looks like the application path contains non ASCII characters. "
                                  "Please move it to another directory or reinstall it\n"
                                  "Путь к приложению содержит символы отличные от ASCII (русские "
                                  "буквы). Переместите приложение или переустановите его.",
                                  "OK");
            exit(0xDEADBEAF);
        }
    }

    m_resourceDirectory = path.absolutePath() + "/resources/";
    setupEnvironment();

    Ide::qml::InitializeControllers();

    auto engine = new QQmlApplicationEngine;
    auto context = engine->rootContext();
    context->setContextProperty("Controllers", ApplicationController::instance);

    addFontDirectory();

    QQuickStyle::setStyle("Fusion");
    QPalette p;
    p = qApp->palette();
    p.setColor(QPalette::Window, "#282C34");
    p.setColor(QPalette::Button, "#363C46");
    p.setColor(QPalette::Highlight, "#776DAFF2");
    p.setColor(QPalette::ButtonText, "#FFFFFF");
    p.setColor(QPalette::Text, "#FFFFFF");
    p.setColor(QPalette::WindowText, "#BEBEBE");
    p.setColor(QPalette::ToolTipBase, "#282C34");
    p.setColor(QPalette::ToolTipText, "#BEBEBE");
    qApp->setPalette(p);

    engine->addImportPath("qrc:/qml");
    engine->addImportPath("qrc:/qml/Ui");
    engine->addImportPath("qrc:/qml/UiElements");
    qmlRegisterType<QMLImageItem>("mur", 1, 0, "ImageItem");
    engine->load("qrc:/qml/Ui/MainWindow.qml");

    auto root_object = engine->rootObjects().at(0);
    auto window = dynamic_cast<QWindow *>(root_object);
    window->setTitle("MUR IDE " + Ide::Ui::ApplicationController::getVersion());

    QSurfaceFormat format;
    format.setMajorVersion(3);
    format.setMinorVersion(2);
    format.setDepthBufferSize(1);
    format.setStencilBufferSize(2);
    format.setProfile(QSurfaceFormat::CoreProfile);

    window->setFormat(format);
}

void Application::setupEnvironment()
{
    auto gst_path_var_name = "GST_PLUGIN_PATH";
    auto gst_plugins = m_resourceDirectory + "gstplugins/";

    if (!qputenv(gst_path_var_name, gst_plugins.toUtf8())) {
        qWarning() << "Unable to set GST_PLUGIN_PATH";
    }

    auto gst_debug_var_name = "GST_DEBUG";
    auto gst_debug_value = QString("0");

    if (!qputenv(gst_debug_var_name, gst_debug_value.toUtf8())) {
        qWarning() << "Unable to set GST_DEBUG";
    }
}

void Application::addFontDirectory()
{
    QFontDatabase::addApplicationFont(":/fonts/fontawesome-webfont.ttf");
    QFontDatabase::addApplicationFont(":/fonts/NotoSans-Regular.ttf");
    QFontDatabase::addApplicationFont(":/fonts/NotoSans-Bold.ttf");
    QFontDatabase::addApplicationFont(":/fonts/NotoMono-Regular.ttf");
}
} 
