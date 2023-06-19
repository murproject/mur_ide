#pragma once

#include <tuple>
#include <QPair>
#include <QString>
#include <QStringList>

namespace Ide::IO {

struct Telemetry
{
    QString vehicle_type;
    QString vehicle_name;
    QString software_version;

    bool is_running = false;
    bool is_remote = false;
    bool is_mission = false;
    bool is_charging = false;
    bool is_shell_running = false;
    bool is_motors_enabled = false;

    double yaw = 0.0;
    double pitch = 0.0;
    double roll = 0.0;
    double depth = 0.0;
    double pressure = 0.0;
    double temperature = 0.0;
    double battery = 0.0;
    double longitude = 0.0;
    double latitude = 0.0;
    double altitude = 0.0;
    double satellites = 0;
    double speed = 0.0;
    double voltage = 0.0;

    double fcu0_voltage = 0.0;
    double fcu0_amperage = 0.0;
    double fcu1_voltage = 0.0;
    double fcu1_amperage = 0.0;
};

struct Notification
{
    QString status;
    QString message;
};

namespace FromJson {
Ide::IO::Telemetry telemetry(const QString &);
Ide::IO::Notification notification(const QString &);
QPair<QString, QString> output(const QString &);
QString diagnostic_log(const QString &);
QStringList software_features(const QString &);
} 

namespace ToJson {
QString code(const QString &, const QString &);
QString remote(const QString &pipeline = "");
QString stop_remote();
QString stop();
QString api_token(const QByteArray& token);
QString diagnostic_log();
QString software_features();
QString shell_command_run(QString cmd, QStringList args);
QString mur_upgrade();
QString set_time(qint64);

} 

} 
