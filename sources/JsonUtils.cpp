#include "JsonUtils.hxx"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QRegularExpression>
#include <QStringList>

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

    if (object.contains("vehicle_type")) {
        telemetry_local.vehicle_type = object["vehicle_type"].toString();
    } else {
        telemetry_local.vehicle_type = "auv";
    }

    if (object.contains("vehicle_name")) {
        telemetry_local.vehicle_name = object["vehicle_name"].toString();
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
    bool is_charging = object["is_charging"].toBool();
    bool is_shell_running = object["is_shell_running"].toBool();
    bool is_motors_enabled = object["is_motors_enabled"].toBool();

    double yaw = object["yaw"].toDouble();
    double pitch = object["pitch"].toDouble();
    double roll = object["roll"].toDouble();
    double depth = object["depth"].toDouble();
    double pressure = object["pressure"].toDouble();
    double temperature = object["temperature"].toDouble();
    double amperage = object["amperage"].toDouble();
    double voltage = object["voltage"].toDouble();
    double battery = object["battery"].toDouble();

    telemetry_local.is_running= is_running;
    telemetry_local.is_remote = is_remote;
    telemetry_local.yaw = yaw;
    telemetry_local.pitch = pitch;
    telemetry_local.roll = roll;
    telemetry_local.depth = depth;
    telemetry_local.pressure = pressure;
    telemetry_local.temperature = temperature;
    telemetry_local.amperage = amperage;
    telemetry_local.voltage = voltage;
    telemetry_local.battery = battery;
    telemetry_local.is_charging = is_charging;
    telemetry_local.is_shell_running = is_shell_running;
    telemetry_local.is_motors_enabled = is_motors_enabled;

    if (object.contains("battery_chip") && object["battery_chip"].toString() == "BQ34Z100G1") {
        QJsonObject battery_obj = object["fg"].toObject();

        int fg_cycle_count = battery_obj["cycle_count"].toInt();
        int fg_full_charge_capacity = battery_obj["full_charge_capacity"].toInt();

        int fg_max_error = battery_obj["max_error"].toInt();
        int fg_remaining_capacity = battery_obj["remaining_capacity"].toInt();
        int fg_reset_count = battery_obj["reset_count"].toInt();
        int fg_update_status = battery_obj["update_status"].toInt();

        double fg_temp = battery_obj["temp"].toDouble();

        bool fg_flag_fc = battery_obj["flag_fc"].toBool();
        bool fg_flag_qen = battery_obj["flag_qen"].toBool();
        bool fg_flag_rup_dis = battery_obj["flag_rup_dis"].toBool();
        bool fg_flag_vok_flag = battery_obj["flag_vok_flag"].toBool();
        bool fg_flag_ocvtaken = battery_obj["flag_ocvtaken"].toBool();

        telemetry_local.fg_cycle_count = fg_cycle_count;
        telemetry_local.fg_full_charge_capacity = fg_full_charge_capacity;
        telemetry_local.fg_max_error = fg_max_error;
        telemetry_local.fg_remaining_capacity = fg_remaining_capacity;
        telemetry_local.fg_reset_count = fg_reset_count;
        telemetry_local.fg_update_status = fg_update_status;
        telemetry_local.fg_temp = fg_temp;
        telemetry_local.fg_flag_fc = fg_flag_fc;
        telemetry_local.fg_flag_qen = fg_flag_qen;
        telemetry_local.fg_flag_rup_dis = fg_flag_rup_dis;
        telemetry_local.fg_flag_vok_flag = fg_flag_vok_flag;
        telemetry_local.fg_flag_ocvtaken = fg_flag_ocvtaken;
    }

    if (object.contains("vehicle_type") && object["vehicle_type"].toString() == "rov") {
        double fcu0_voltage  = object["fcu"].toArray()[0].toObject()["voltage"].toDouble();
        double fcu0_amperage = object["fcu"].toArray()[0].toObject()["amperage"].toDouble();
        double fcu1_voltage  = object["fcu"].toArray()[1].toObject()["voltage"].toDouble();
        double fcu1_amperage = object["fcu"].toArray()[1].toObject()["amperage"].toDouble();

        telemetry_local.fcu0_voltage = fcu0_voltage;
        telemetry_local.fcu0_amperage = fcu0_amperage;
        telemetry_local.fcu1_voltage = fcu1_voltage;
        telemetry_local.fcu1_amperage = fcu1_amperage;
    }

    return telemetry_local;
}

Ide::IO::Notification notification(const QString &json)
{
    Ide::IO::Notification notification;

    auto doc = QJsonDocument::fromJson(json.toUtf8());
    auto object = doc.object();

    if (!(object.contains("type") && object["type"].toString() == "notification")) {
        return Ide::IO::Notification{};
    }

    notification.status = object["status"].toString();
    notification.message = object["message"].toString();
    qDebug() << notification.status << notification.message;

    return notification;
}

QString diagnostic_log(const QString &json)
{
    auto doc = QJsonDocument::fromJson(json.toUtf8());
    auto object = doc.object();

    if (!(object.contains("type") && object["type"].toString() == "diagnostic_log_response")) {
        return "";
    }

    QString log = object["output"].toString();
    log.replace(QRegularExpression("\x1b\[[0-9;]*m"), "");

    return log;
}

QStringList software_features(const QString &json)
{
    auto doc = QJsonDocument::fromJson(json.toUtf8());
    auto object = doc.object();
    QStringList result;

    if (!(object.contains("type") && object["type"].toString() == "software_features")) {
        return result;
    }

    QJsonArray features = object["features"].toArray();

    for (auto item : features) {
        result << item.toString();
    }

    return result;
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

} 

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

QString remote(const QString &pipeline)
{
    QJsonObject json;

    json["type"] = "remote";

    if (!pipeline.isEmpty()) {
        json["pipeline"] = pipeline;
    }

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

QString diagnostic_log()
{
    QJsonObject json;

    json["type"] = "get_diagnostic_log";

    QJsonDocument doc(json);
    QString str(doc.toJson(QJsonDocument::Compact));

    return str;
}

QString software_features()
{
    QJsonObject json;

    json["type"] = "get_software_features";

    QJsonDocument doc(json);
    QString str(doc.toJson(QJsonDocument::Compact));

    return str;
}

QString shell_command_run(QString cmd, QStringList args)
{
    QJsonObject json;

    json["type"] = "shell_command_run";
    json["cmd"] = cmd;
    json["args"] = QJsonArray::fromStringList(args);

    QJsonDocument doc(json);
    QString str(doc.toJson(QJsonDocument::Compact));

    return str;
}

QString mur_upgrade()
{
    QJsonObject json;

    json["type"] = "mur_upgrade";
    json["proxy_port"] = "10001";

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

QString set_time(const qint64 time)
{
    QJsonObject json;

    json["type"] = "set_time";
    json["time"] = QString::number(time);

    QJsonDocument doc(json);
    QString str(doc.toJson(QJsonDocument::Compact));

    return str;
}

QString battery_command(QString command){
    QJsonObject json;
    json["type"] = "battery_command";
    json["command"] = command;

    QJsonDocument doc(json);
    QString str(doc.toJson(QJsonDocument::Compact));

    return str;

}

} 

} 
