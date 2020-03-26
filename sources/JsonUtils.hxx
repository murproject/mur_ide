#pragma once

#include <tuple>
#include <QPair>
#include <QString>

namespace Ide::IO {

struct Telemetry
{
    bool is_running = false;
    bool is_remote = false;
    double yaw = 0.0;
    double pitch = 0.0;
    double roll = 0.0;
    double depth = 0.0;
    double pressure = 0.0;
    double battery = 0.0;
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

} // namespace ToJson

} // namespace ide::io
