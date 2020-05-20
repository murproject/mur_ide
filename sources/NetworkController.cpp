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
    m_webSocket = new QWebSocket{};
    m_pingTimer = new QTimer{};
    m_pongTimer = new QTimer{};
    m_ApiTokenDialog = new ApiTokenDialog{};

    m_reconnectionTimer = new QTimer{};
    m_reconnectionTimer->start(3000);

    m_pingTimer->setInterval(5000);
    m_pongTimer->setInterval(10000);

    connect(m_reconnectionTimer, &QTimer::timeout, this, &NetworkController::onReconnectTimer);
    connect(m_webSocket, &QWebSocket::connected, this, &NetworkController::onConnected);
    connect(m_webSocket, &QWebSocket::disconnected, this, &NetworkController::onDisconnected);
    connect(m_webSocket, &QWebSocket::textMessageReceived, this, &NetworkController::onTelimetryReceived);
    connect(m_webSocket, &QWebSocket::pong, this, &NetworkController::onPongReceived);
    connect(m_pongTimer, &QTimer::timeout, this, &NetworkController::onPongTimeout);
    connect(m_pingTimer, &QTimer::timeout, this, &NetworkController::onPingTimeout);
    connect(m_ApiTokenDialog, &QDialog::accepted, this, &NetworkController::onTokenAccepted);
}

void NetworkController::onConnected()
{
    m_reconnectionTimer->stop();
    m_pingTimer->start();
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
    m_webSocket->open((QUrl("ws://10.3.141.1:2090")));
}

void NetworkController::onTelimetryReceived(const QString &message)
{
    auto doc = QJsonDocument::fromJson(message.toUtf8());
    auto object = doc.object();

    if ((object.contains("type") && object["type"].toString() == "output")) {
        auto [error_b64, output_b64] = Ide::IO::FromJson::output(message);

        auto error = QString(QByteArray::fromBase64(error_b64.toUtf8())).trimmed();
        auto output = QString(QByteArray::fromBase64(output_b64.toUtf8())).trimmed();

        ApplicationLogger::instance->addEntry(output);
        ApplicationLogger::instance->addEntry(error);
    }

    if ((object.contains("type") && object["type"].toString() == "telemetry")) {
        m_telimetry = Ide::IO::FromJson::telemetry(message);
        emit telimetryUpdated();
    }

    if ((object.contains("type") && object["type"].toString() == "request_api_token")) {
        ApplicationLogger::instance->addEntry("API token required");
        m_ApiTokenDialog->open();
    }
}

void NetworkController::onPongReceived(quint64, const QByteArray &)
{
    m_pongTimer->start();
}

void NetworkController::onPingTimeout()
{
    m_webSocket->ping();
    m_pongTimer->start();
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
    return m_telimetry.battery;
}

bool NetworkController::getConnectionStatus()
{
    return m_webSocket->isValid();
}

bool NetworkController::isRomoteScriptRunning()
{
    return m_telimetry.is_running;
}

bool NetworkController::isRemoteModeEnabled()
{
    return m_telimetry.is_remote;
}

double NetworkController::getYaw()
{
    return m_telimetry.yaw;
}

double NetworkController::getPitch()
{
    return m_telimetry.pitch;
}

double NetworkController::getRoll()
{
    return m_telimetry.roll;
}

double NetworkController::getDepth()
{
    return m_telimetry.depth;
}

double NetworkController::getPressure()
{
    return m_telimetry.pressure;
}

bool NetworkController::isUsv()
{
    return m_telimetry.is_usv;
}

double NetworkController::getLatitude()
{
    return m_telimetry.latitude;
}

double NetworkController::getLongitude()
{
    return m_telimetry.longitude;
}

double NetworkController::getSatellites()
{
    return m_telimetry.satellites;
}

double NetworkController::getAltitude()
{
    return m_telimetry.altitude;
}

double NetworkController::getSpeed()
{
    return m_telimetry.speed;
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

    m_webSocket->sendTextMessage(Ide::IO::ToJson::remote());
}

void NetworkController::stopRemote()
{
    if (!m_webSocket->isValid()) {
        return;
    }

    m_webSocket->sendTextMessage(Ide::IO::ToJson::stop_remote());
}

NetworkController *NetworkController::Create()
{
    instance = new NetworkController();
    return instance;
}

} // namespace ide::ui
