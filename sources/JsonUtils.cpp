#include "JsonUtils.hxx"
#include <QJsonDocument>
#include <QJsonObject>

namespace Ide::IO {
namespace FromJson {
Ide::IO::Telemetry telemetry(const QString &json)
{
    auto doc = QJsonDocument::fromJson(json.toUtf8());
    auto object = doc.object();

    if (!(object.contains("type") && object["type"].toString() == "telemetry")) {
        return Ide::IO::Telemetry{};
    }

    bool is_running = object["running"].toBool();
    bool is_remote = object["remote"].toBool();

    double yaw = object["yaw"].toDouble();
    double pitch = object["pitch"].toDouble();
    double roll = object["roll"].toDouble();
    double depth = object["depth"].toDouble();
    double pressure = object["pressure"].toDouble();
    double battery = object["battery"].toDouble();

    return Ide::IO::Telemetry{is_running, is_remote, yaw, pitch, roll, depth, pressure, battery};
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

} // namespace ToJson

} // namespace ide::io
