#pragma once

#include <tuple>
#include <QPair>
#include <QString>

namespace Ide::IO {

struct Telemetry
{
    bool is_running = false;
    bool is_remote = false;
    bool is_mission = false;
    bool is_charging = false;
    bool is_usv = false;

    double yaw = 0.0;
    double pitch = 0.0;
    double roll = 0.0;
    double depth = 0.0;
    double pressure = 0.0;
    double battery = 0.0;
    double longitude = 0.0;
    double latitude = 0.0;
    double altitude = 0.0;
    double satellites = 0;
    double speed = 0.0;
    double voltage = 0.0;

};

namespace FromJson {
Ide::IO::Telemetry telemetry(const QString &);
QPair<QString, QString> output(const QString &);
} // namespace FromJson

namespace ToJson {
QString code(const QString &, const QString &);
QString remote();
QString stop_remote();
QString stop();
QString api_token(const QByteArray& token);

} // namespace ToJson

} // namespace ide::io
