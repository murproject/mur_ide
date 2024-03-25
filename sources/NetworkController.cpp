#include "NetworkController.hxx"
#include "ApplicationLogger.hxx"
#include "EditorController.hxx"
#include "RemoteController.hxx"
#include <QMessageBox>

namespace Ide::Ui {

NetworkController *NetworkController::instance = nullptr;
qml::RegisterType<NetworkController> NetworkController::Register;

NetworkController::NetworkController()
{
    if (instance != nullptr) {
        throw std::runtime_error("Instance of network_controller already exists");
    }

    QSettings settings("settings.ini", QSettings::IniFormat);
    m_connectionAddress = settings.value("Network/connection_address", "10.3.141.1:2090").toString();
    m_reconnectTime = settings.value("Network/reconnect_time", 1000).toInt();
    m_pingTime = settings.value("Network/ping_time", 1000).toInt();
    m_pongTime = settings.value("Network/pong_time", 3000).toInt();

    m_webSocket = new QWebSocket{};
    m_reconnectionTimer = new QTimer{};
    m_pingTimer = new QTimer{};
    m_pongTimer = new QTimer{};
    m_ApiTokenDialog = new ApiTokenDialog{};

    m_reconnectionTimer->start(m_reconnectTime);
    m_pingTimer->setInterval(m_pingTime);
    m_pongTimer->setInterval(m_pongTime);

    connect(m_reconnectionTimer, &QTimer::timeout, this, &NetworkController::onReconnectTimer);
    connect(m_webSocket, &QWebSocket::connected, this, &NetworkController::onConnected);
    connect(m_webSocket, &QWebSocket::disconnected, this, &NetworkController::onDisconnected);
    connect(m_webSocket, &QWebSocket::textMessageReceived, this, &NetworkController::onTelemetryReceived);
    connect(m_webSocket, &QWebSocket::pong, this, &NetworkController::onPongReceived);
    connect(m_pongTimer, &QTimer::timeout, this, &NetworkController::onPongTimeout);
    connect(m_pingTimer, &QTimer::timeout, this, &NetworkController::onPingTimeout);
    connect(m_ApiTokenDialog, &QDialog::accepted, this, &NetworkController::onTokenAccepted);
}

void NetworkController::onConnected()
{
    m_reconnectionTimer->stop();
    m_pingTimer->start();
    requestFeatures();
    emit connectionStatusChanged();
}

void NetworkController::onDisconnected()
{
    m_reconnectionTimer->start();
    m_pingTimer->stop();
    m_pongTimer->stop();
    emit connectionStatusChanged();
    RemoteController::instance->stopImageCapture();
}

void NetworkController::onReconnectTimer()
{
    m_webSocket->abort();
    m_webSocket->open((QUrl("ws://" + m_connectionAddress)));
}

void NetworkController::requestFeatures() {
    m_softwareFeatures.clear();
    m_webSocket->sendTextMessage(Ide::IO::ToJson::software_features());
}

void NetworkController::syncVehicleTime() {
    m_webSocket->sendTextMessage(Ide::IO::ToJson::set_time(QDateTime::currentSecsSinceEpoch()));
}

QString NetworkController::getConnectionAddress() {
    return m_connectionAddress;
}

void NetworkController::setConnectionAddress(QString address) {
    m_connectionAddress = address;
}

void NetworkController::saveSettings() {
    QSettings settings("settings.ini", QSettings::IniFormat);

    settings.setValue("Network/connection_address", m_connectionAddress);
    settings.setValue("Network/ping_time", m_pingTime);
    settings.setValue("Network/pong_time", m_pongTime);
    settings.setValue("Network/reconnect_time", m_reconnectTime);
}

int NetworkController::getPingTime() {
    return m_pingTime;
}

int NetworkController::getPongTime() {
    return m_pongTime;
}

int NetworkController::getReconnectTime() {
    return m_reconnectTime;
}

void NetworkController::setPingTime(int value) {
    m_pingTime = value;
    emit timingChanged();
}

void NetworkController::setPongTime(int value) {
    m_pongTime = value;
    emit timingChanged();
}

void NetworkController::setReconnectTime(int value) {
    m_reconnectTime = value;
    emit timingChanged();
}

void NetworkController::onTelemetryReceived(const QString &message)
{
    auto doc = QJsonDocument::fromJson(message.toUtf8());
    auto object = doc.object();

    if (object.contains("type")) {
        QString type = object["type"].toString();

        if (type == "output") {
            auto [output_b64, error_b64] = Ide::IO::FromJson::output(message);

            auto error = QString(QByteArray::fromBase64(error_b64.toUtf8())).trimmed();
            auto output = QString(QByteArray::fromBase64(output_b64.toUtf8())).trimmed();

            if (object.contains("shell") && object["shell"].toBool() == true) {
                if (output.contains("mur-midauv-meta Version:")) {
                    QRegularExpression version_regexp("mur-midauv-meta Version: (\\d+)\\.(\\d+)-(\\d+)");
                    auto match = version_regexp.match(output);
                    QVector<int> version;

                    if (match.hasMatch()) {
                        version << match.captured(1).toInt()
                                << match.captured(2).toInt()
                                << match.captured(3).toInt();

                    }
                    qDebug() << version[0] << version[1] << version[2];
                    emit firmwareVersionReceived(version[0], version[1], version[2]);
                }

                emit shellOutputReceived(output, error);
            } else {
                output.replace("\n", "<br>");
                error.replace("\n", "<br>");
                ApplicationLogger::instance->addOutput(output, error);
            }
        }

        if (type == "telemetry") {
            m_telemetry = Ide::IO::FromJson::telemetry(message);
            qDebug() << "volts: " << doc["voltage"].toDouble()
                     << "\tamps: " << doc["amperage"].toDouble();
            emit telemetryUpdated();
        }

        if (type == "request_api_token") {
            ApplicationLogger::instance->addEntry("API token required");
            m_ApiTokenDialog->open();
        }

        if (type == "notification") {
            Ide::IO::Notification notification = Ide::IO::FromJson::notification(message);
            emit notificationReceived(notification.status, notification.message);
        }

        if (type == "diagnostic_log_response") {
            QString log = Ide::IO::FromJson::diagnostic_log(message);
            emit diagnosticLogReceived(log);
        }

        if (type == "software_features") {
            QStringList features = Ide::IO::FromJson::software_features(message);
            m_softwareFeatures.clear();
            m_softwareFeatures << features;
            qDebug() << m_softwareFeatures;

            if (isFeatureSupported("set_time")) {
                syncVehicleTime();
            }
        }
    }
}

void NetworkController::onPongReceived(quint64, const QByteArray &)
{
    m_pingTimer->start();
}

void NetworkController::onPingTimeout()
{
    m_pongTimer->start();
    m_pingTimer->stop();
    m_webSocket->ping();
}

void NetworkController::onPongTimeout()
{
    onDisconnected();
}

void NetworkController::onTokenAccepted()
{
    if (!m_webSocket->isValid()) {
        return;
    }
    m_webSocket->sendTextMessage(Ide::IO::ToJson::api_token(m_ApiTokenDialog->getBase64Token()));
}

double NetworkController::getBatteryStatus()
{
    return m_telemetry.battery;
}

bool NetworkController::isCharging() {
    return m_telemetry.is_charging;
}

bool NetworkController::getConnectionStatus()
{
    return m_webSocket->isValid();
}

bool NetworkController::isRemoteScriptRunning()
{
    return m_telemetry.is_running;
}

bool NetworkController::isRemoteModeEnabled()
{
    return m_telemetry.is_remote;
}

double NetworkController::getYaw()
{
    return m_telemetry.yaw;
}

double NetworkController::getPitch()
{
    return m_telemetry.roll;
}

double NetworkController::getRoll()
{
    return m_telemetry.pitch;
}

double NetworkController::getDepth()
{
    return m_telemetry.depth;
}

double NetworkController::getPressure()
{
    return m_telemetry.pressure;
}

double NetworkController::getTemperature()
{
    return m_telemetry.temperature;
}

bool NetworkController::isUsv()
{
    return m_telemetry.vehicle_type == "usv";
}

bool NetworkController::isRov()
{
    return m_telemetry.vehicle_type == "rov";
}

double NetworkController::getLatitude()
{
    return m_telemetry.latitude;
}

double NetworkController::getLongitude()
{
    return m_telemetry.longitude;
}

double NetworkController::getSatellites()
{
    return m_telemetry.satellites;
}

double NetworkController::getAltitude()
{
    return m_telemetry.altitude;
}

double NetworkController::getSpeed()
{
    return m_telemetry.speed;
}

int NetworkController::getAdcCycleCount()
{
    return m_telemetry.fg_cycle_count;
}

int NetworkController::getAdcFullChargeCapacity()
{
    return m_telemetry.fg_full_charge_capacity;
}

int NetworkController::getAdcMaxError()
{
    return m_telemetry.fg_max_error;
}

int NetworkController::getAdcRemainCapacity()
{
    return m_telemetry.fg_remaining_capacity;
}

int NetworkController::getAdcResetCount()
{
    return m_telemetry.fg_reset_count;
}

int NetworkController::getAdcUpdateStatus()
{
    return m_telemetry.fg_update_status;
}

double NetworkController::getAdcTemp()
{
    return m_telemetry.fg_temp;
}

double NetworkController::getAmperage()
{
    return m_telemetry.amperage;
}

double NetworkController::getVoltage()
{
    return m_telemetry.voltage;
}

bool NetworkController::getAdcFlagFc()
{
    return m_telemetry.fg_flag_fc;
}

bool NetworkController::getAdcFlagQen()
{
    return m_telemetry.fg_flag_qen;
}

bool NetworkController::getAdcFlagRupDis()
{
    return m_telemetry.fg_flag_rup_dis;
}

bool NetworkController::getAdcFlagVok()
{
    return m_telemetry.fg_flag_vok_flag;
}

bool NetworkController::getAdcFlagOcvtaken()
{
    return m_telemetry.fg_flag_ocvtaken;
}

QString NetworkController::getVehicleType()
{
    return m_telemetry.vehicle_type;
}

QString NetworkController::getVehicleFancyName(bool html)
{
    if (m_telemetry.vehicle_type == "usv") {
        return html ? "<b>MiddleUSV</b>" : "MiddleUSV";} 
    else if (m_telemetry.vehicle_type == "rov") {
        return html ? "<b>ProROV</b>" : "ProROV";}
    else {
        if (m_telemetry.vehicle_name.contains("MiddleAUV-CM4")) {
            return html ? "<b>MiddleAUV</b>-CM4" : "MiddleAUV-CM4";
        }
        return html ? "<b>MiddleAUV</b>-CM3" : "MiddleAUV-CM3"; 
    }
}

QStringList NetworkController::getSoftwareFeatures() {
    return m_softwareFeatures;
}

bool NetworkController::isFeatureSupported(const QString &feature) {
    return m_softwareFeatures.contains(feature);
}

QList<qreal> NetworkController::getFcuTelemetry() {
    QList<qreal> fcu_telemetry = {
        m_telemetry.fcu0_voltage,
        m_telemetry.fcu0_amperage,
        m_telemetry.fcu1_voltage,
        m_telemetry.fcu1_amperage
    };

    return fcu_telemetry;
}

void NetworkController::setRemoteThrust(const QString &message)
{
    if (!m_webSocket->isValid()) {
        return;
    }

    m_webSocket->sendTextMessage(message);
}

void NetworkController::run()
{
    if (!m_webSocket->isValid()) {
        return;
    }

    auto filename = EditorController::instance->getFileName();
    auto content = EditorController::instance->getText();
    m_webSocket->sendTextMessage(Ide::IO::ToJson::code(filename, content));
}

void NetworkController::stop()
{
    if (!m_webSocket->isValid()) {
        return;
    }

    m_webSocket->sendTextMessage(Ide::IO::ToJson::stop());
}

void NetworkController::remote()
{
    if (!m_webSocket->isValid()) {
        return;
    }

    if (isFeatureSupported("gst_custom")) {
        m_webSocket->sendTextMessage(Ide::IO::ToJson::remote(Ide::Ui::RemoteController::instance->getPipelines()[0])); 
    } else {
        m_webSocket->sendTextMessage(Ide::IO::ToJson::remote());
    }

}

void NetworkController::stopRemote()
{
    if (!m_webSocket->isValid()) {
        return;
    }

    m_webSocket->sendTextMessage(Ide::IO::ToJson::stop_remote());
}

void NetworkController::diagnosticLog()
{
    if (!m_webSocket->isValid()) {
        return;
    }

    m_webSocket->sendTextMessage(Ide::IO::ToJson::diagnostic_log());
}

void NetworkController::runShellCommand(QString cmd, QStringList args)
{
    if (!m_webSocket->isValid()) {
        return;
    }

    m_webSocket->sendTextMessage(Ide::IO::ToJson::shell_command_run(cmd, args));
}

void NetworkController::requestSoftwareVersion()
{
    runShellCommand("/bin/bash",
        {"-c", "echo $(ver=$(dpkg -s mur-midauv-meta | grep Version); echo mur-midauv-meta ${ver:?}); sleep 1"});
}

void NetworkController::murUpgrade()
{
    if (!m_webSocket->isValid()) {
        return;
    }

    qDebug() << Ide::IO::ToJson::mur_upgrade();

    m_webSocket->sendTextMessage(Ide::IO::ToJson::mur_upgrade());
}

void NetworkController::batteryButton(QString cmd){
    if (!m_webSocket->isValid()) {
        return;
    }

    m_webSocket->sendTextMessage(Ide::IO::ToJson::battery_command(cmd));
}

NetworkController *NetworkController::Create()
{
    instance = new NetworkController();
    return instance;
}

}
