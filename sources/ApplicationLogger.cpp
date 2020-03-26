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
    if (m_entries.size() > 4500) {
        clear();
    }
    m_entries.append(string);
}

QStringList ApplicationLogger::getEntries()
{
    return m_entries;
}

void ApplicationLogger::clear()
{
    m_entries.clear();
    emit entriesChanged();
}

ApplicationLogger::ApplicationLogger()
{
    if (instance != nullptr) {
        throw std::runtime_error("Instance of application_logger already exists");
    }
    m_updateViewTimer = new QTimer;
    connect(m_updateViewTimer, &QTimer::timeout, this, &ApplicationLogger::onUpdate);
    m_updateViewTimer->start(150);
}

void ApplicationLogger::onUpdate()
{
    static int entries_last_size = 0;
    if (entries_last_size != m_entries.size()) {
        emit entriesChanged();
        entries_last_size = m_entries.size();
    }
}

} // namespace ide::ui
