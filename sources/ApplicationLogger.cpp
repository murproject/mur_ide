#include "ApplicationLogger.hxx"

namespace Ide::Ui {
ApplicationLogger *ApplicationLogger::instance = nullptr;
qml::RegisterType<ApplicationLogger> ApplicationLogger::Register;

ApplicationLogger *ApplicationLogger::Create()
{
    instance = new ApplicationLogger{};
    return instance;
}

void ApplicationLogger::addEntry(const QString &string)
{
    m_output += "<br>" + string + "<br>";
    emit outputChanged();
}

void ApplicationLogger::addScriptOutput(const QString &string){
    m_output += string;
    emit outputChanged();
}

void ApplicationLogger::addOutput(const QString &output, const QString &error)
{
    m_output += output;
    m_output += "<br>";
    if (!error.isEmpty()) {
        m_output += "<font color=\"#E74C3C\">" + error + "</font><br>";
    }
}

QString ApplicationLogger::getOutput()
{
    static const QString header = "<tt style=\"white-space:pre-wrap;border: 1px solid yellow;\">";
    static const QString footer = "</tt>";
    return header + m_output + footer;
}

void ApplicationLogger::clear()
{
    m_output.clear();
    emit outputChanged();
}

ApplicationLogger::ApplicationLogger()
{
    if (instance != nullptr) {
        throw std::runtime_error("Instance of application_logger already exists");
    }
    m_updateViewTimer = new QTimer;
    connect(m_updateViewTimer, &QTimer::timeout, this, &ApplicationLogger::onUpdate);
    m_updateViewTimer->start(200);
}

void ApplicationLogger::onUpdate()
{
    static long unsigned last_output_size = 0;
    if (last_output_size != m_output.length()) {
        m_output = m_output.right(20000);
        last_output_size = m_output.length();
        emit outputChanged();
    }
}

} 
