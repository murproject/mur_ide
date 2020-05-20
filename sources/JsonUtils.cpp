#include "JsonUtils.hxx"
#include <QJsonDocument>
#include <QJsonObject>

namespace Ide::IO {
namespace FromJson {
Ide::IO::Telemetry telemetry(const QString &json)
{
    Ide::IO::Telemetry telemetry_local;

    auto doc = QJsonDocument::fromJson(json.toUtf8());
    auto object = doc.object();

    if (!(object.contains("type") && object["type"].toString() == "telemetry")) {
        return Ide::IO::Telemetry{};
    }

    if (object.contains("vehicle_type") && object["vehicle_type"].toString() == "usv") {
        bool is_running = object["running"].toBool();
        bool is_remote = object["remote"].toBool();
        bool is_mission = object["mission"].toBool();
        double yaw = object["yaw"].toDouble();
        double pitch = object["pitch"].toDouble();
        double roll = object["roll"].toDouble();
        double battery = object["battery"].toDouble();
        double satellites = object["gps_satellites"].toDouble();
        double longitude = object["gps_lng"].toDouble();
        double latitude = object["gps_lat"].toDouble();
        double altitude = object["gps_alt"].toDouble();
        double speed = object["gps_speed"].toDouble();

        telemetry_local.is_running= is_running;
        telemetry_local.is_remote = is_remote;
        telemetry_local.yaw = yaw;
        telemetry_local.pitch = pitch;
        telemetry_local.roll = roll;
        telemetry_local.battery = battery;
        telemetry_local.is_mission = is_mission;
        telemetry_local.satellites = satellites;
        telemetry_local.longitude = longitude;
        telemetry_local.latitude = latitude;
        telemetry_local.speed = speed;
        telemetry_local.altitude = altitude;

        return telemetry_local;
    }

    bool is_running = object["running"].toBool();
    bool is_remote = object["remote"].toBool();

    double yaw = object["yaw"].toDouble();
    double pitch = object["pitch"].toDouble();
    double roll = object["roll"].toDouble();
    double depth = object["depth"].toDouble();
    double pressure = object["pressure"].toDouble();
    double battery = object["battery"].toDouble();

    telemetry_local.is_running= is_running;
    telemetry_local.is_remote = is_remote;
    telemetry_local.yaw = yaw;
    telemetry_local.pitch = pitch;
    telemetry_local.roll = roll;
    telemetry_local.depth = depth;
    telemetry_local.pressure = pressure;
    telemetry_local.battery = battery;

    return telemetry_local;
}

QPair<QString, QString> output(const QString &json)
{
    auto doc = QJsonDocument::fromJson(json.toUtf8());
    auto object = doc.object();

    if (!(object.contains("type") && object["type"].toString() == "output")) {
        return {};
    }

    QString err = object["stderr"].toString();
    QString out = object["stdout"].toString();

    return {out, err};
}

} // namespace FromJson

namespace ToJson {

QString code(const QString &filename, const QString &content)
{
    QJsonObject json;

    json["type"] = "run";
    json["filename"] = filename;
    json["content"] = QString(content.toUtf8().toBase64());

    QJsonDocument doc(json);
    QString str(doc.toJson(QJsonDocument::Compact));

    return str;
}

QString remote()
{
    QJsonObject json;

    json["type"] = "remote";

    QJsonDocument doc(json);
    QString str(doc.toJson(QJsonDocument::Compact));

    return str;
}

QString stop()
{
    QJsonObject json;

    json["type"] = "stop";

    QJsonDocument doc(json);
    QString str(doc.toJson(QJsonDocument::Compact));

    return str;
}

QString stop_remote()
{
    QJsonObject json;

    json["type"] = "stop_remote";

    QJsonDocument doc(json);
    QString str(doc.toJson(QJsonDocument::Compact));

    return str;
}

QString api_token(const QByteArray& token)
{
    QJsonObject json;

    json["type"] = "set_api_token";
    json["api_token"] = QString(token);

    QJsonDocument doc(json);
    QString str(doc.toJson(QJsonDocument::Compact));

    return str;
}

} // namespace ToJson

} // namespace ide::io
