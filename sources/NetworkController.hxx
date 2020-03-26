#pragma once

#include "JsonUtils.hxx"
#include "QmlUtils.hxx"

#include <QObject>
#include <QTimer>
#include <QWebSocket>

namespace Ide::Ui {
class NetworkController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool connected READ getConnectionStatus NOTIFY connectionStatusChanged)
    Q_PROPERTY(bool running READ isRomoteScriptRunning NOTIFY telimetryUpdated)
    Q_PROPERTY(bool remote READ isRemoteModeEnabled NOTIFY telimetryUpdated)

    Q_PROPERTY(double battery READ getBatteryStatus NOTIFY telimetryUpdated)
    Q_PROPERTY(double yaw READ getYaw NOTIFY telimetryUpdated)
    Q_PROPERTY(double pitch READ getPitch NOTIFY telimetryUpdated)
    Q_PROPERTY(double roll READ getRoll NOTIFY telimetryUpdated)
    Q_PROPERTY(double depth READ getDepth NOTIFY telimetryUpdated)
    Q_PROPERTY(double pressure READ getPressure NOTIFY telimetryUpdated)

public:
    static NetworkController *instance;
    static NetworkController *Create();

    double getBatteryStatus();
    bool getConnectionStatus();
    bool isRomoteScriptRunning();
    bool isRemoteModeEnabled();
    double getYaw();
    double getPitch();
    double getRoll();
    double getDepth();
    double getPressure();

    void setRemoteThrust(const QString &);

    void run();
    void stop();
    void remote();
    void stopRemote();

signals:
    void connectionStatusChanged();
    void telimetryUpdated();

private:
    NetworkController();
    static qml::RegisterType<NetworkController> Register;

    void onConnected();
    void onDisconnected();
    void onReconnectTimer();
    void onTelimetryReceived(const QString &);
    void onPongReceived(quint64, const QByteArray &);
    void onPingTimeout();
    void onPongTimeout();

    Ide::IO::Telemetry m_telimetry;
    QWebSocket *m_webSocket;
    QTimer *m_reconnectionTimer;
    QTimer *m_pingTimer;
    QTimer *m_pongTimer;
};

} // namespace ide::ui
