#pragma once 

#include "QmlUtils.hxx"
#include <QObject>
#include <QApplication>
#include <QSettings>
#include <QSysInfo>

namespace Ide::Ui {
class SettingsController : public QObject
{
    Q_OBJECT

public:
    static SettingsController *instance;
    static SettingsController *Create();
    QSettings *settings;
    QString getResourcePath();
    QString getPythonPath();
    QString getSimulatorPath();
    QString getSettingsLocation();

private:
    SettingsController();
    QString m_resourcePath;
    QString m_pythonPath;
    QString m_simulatorPath;

};
}
