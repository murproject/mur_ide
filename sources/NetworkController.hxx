#pragma once

#include "JsonUtils.hxx"
#include "QmlUtils.hxx"

#include <QObject>
#include <QTimer>
#include <QWebSocket>
#include "ApiTokenDialog.hxx"

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
    Q_PROPERTY(bool usv READ isUsv NOTIFY telimetryUpdated)
    Q_PROPERTY(double latitude READ getLatitude NOTIFY telimetryUpdated)
    Q_PROPERTY(double longitude READ getLongitude NOTIFY telimetryUpdated) //altitude
    Q_PROPERTY(double satellites READ getSatellites NOTIFY telimetryUpdated)
    Q_PROPERTY(double altitude READ getAltitude NOTIFY telimetryUpdated)
    Q_PROPERTY(double speed READ getSpeed NOTIFY telimetryUpdated)

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
    bool isUsv();
    double getLatitude();
    double getLongitude();
    double getSatellites();
    double getAltitude();
    double getSpeed();


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
    void onTokenAccepted();

    Ide::IO::Telemetry m_telimetry;
    QWebSocket *m_webSocket;
    QTimer *m_reconnectionTimer;
    QTimer *m_pingTimer;
    QTimer *m_pongTimer;
    ApiTokenDialog* m_ApiTokenDialog;
};

} // namespace ide::ui
