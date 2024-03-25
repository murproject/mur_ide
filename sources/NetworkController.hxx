#pragma once

#include "JsonUtils.hxx"
#include "QmlUtils.hxx"

#include <QObject>
#include <QTimer>
#include <QString>
#include <QWebSocket>
#include "ApiTokenDialog.hxx"

namespace Ide::Ui {
class NetworkController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool connected READ getConnectionStatus NOTIFY connectionStatusChanged)
    Q_PROPERTY(bool running READ isRemoteScriptRunning NOTIFY telemetryUpdated)
    Q_PROPERTY(bool remote READ isRemoteModeEnabled NOTIFY telemetryUpdated)

    Q_PROPERTY(double battery READ getBatteryStatus NOTIFY telemetryUpdated)
    Q_PROPERTY(bool is_charging READ isCharging NOTIFY telemetryUpdated)
    Q_PROPERTY(double yaw READ getYaw NOTIFY telemetryUpdated)
    Q_PROPERTY(double pitch READ getPitch NOTIFY telemetryUpdated)
    Q_PROPERTY(double roll READ getRoll NOTIFY telemetryUpdated)
    Q_PROPERTY(double depth READ getDepth NOTIFY telemetryUpdated)
    Q_PROPERTY(double pressure READ getPressure NOTIFY telemetryUpdated)
    Q_PROPERTY(double temperature READ getTemperature NOTIFY telemetryUpdated)
    Q_PROPERTY(bool usv READ isUsv NOTIFY telemetryUpdated)
    Q_PROPERTY(bool rov READ isRov NOTIFY telemetryUpdated)
    Q_PROPERTY(double latitude READ getLatitude NOTIFY telemetryUpdated)
    Q_PROPERTY(double longitude READ getLongitude NOTIFY telemetryUpdated)
    Q_PROPERTY(double satellites READ getSatellites NOTIFY telemetryUpdated)
    Q_PROPERTY(double altitude READ getAltitude NOTIFY telemetryUpdated)
    Q_PROPERTY(double speed READ getSpeed NOTIFY telemetryUpdated)

    Q_PROPERTY(int pingTime READ getPingTime WRITE setPingTime NOTIFY timingChanged)
    Q_PROPERTY(int pongTime READ getPongTime WRITE setPongTime NOTIFY timingChanged)
    Q_PROPERTY(int reconnectTime READ getReconnectTime WRITE setReconnectTime NOTIFY timingChanged)

    Q_PROPERTY(int fg_cycle_count READ getAdcCycleCount NOTIFY telemetryUpdated)
    Q_PROPERTY(int fg_full_charge_capacity READ getAdcFullChargeCapacity NOTIFY telemetryUpdated)
    Q_PROPERTY(int fg_max_error READ getAdcMaxError NOTIFY telemetryUpdated)
    Q_PROPERTY(int fg_remaining_capacity READ getAdcRemainCapacity NOTIFY telemetryUpdated)
    Q_PROPERTY(int fg_reset_count READ getAdcResetCount NOTIFY telemetryUpdated)
    Q_PROPERTY(int fg_update_status READ getAdcUpdateStatus NOTIFY telemetryUpdated)

    Q_PROPERTY(double fg_temp READ getAdcTemp NOTIFY telemetryUpdated)
    Q_PROPERTY(double amperage READ getAmperage NOTIFY telemetryUpdated)
    Q_PROPERTY(double voltage READ getVoltage NOTIFY telemetryUpdated)

    Q_PROPERTY(bool fg_flag_fc READ getAdcFlagFc NOTIFY telemetryUpdated)
    Q_PROPERTY(bool fg_flag_qen READ getAdcFlagQen NOTIFY telemetryUpdated)
    Q_PROPERTY(bool fg_flag_rup_dis READ getAdcFlagRupDis NOTIFY telemetryUpdated)
    Q_PROPERTY(bool fg_flag_vok_flag READ getAdcFlagVok NOTIFY telemetryUpdated)
    Q_PROPERTY(bool fg_flag_ocvtaken READ getAdcFlagOcvtaken NOTIFY telemetryUpdated)

    Q_PROPERTY(QList<qreal> fcu_telemetry READ getFcuTelemetry NOTIFY telemetryUpdated)

    Q_PROPERTY(QString vehicle_type READ getVehicleType NOTIFY telemetryUpdated)
    Q_PROPERTY(QString vehicle_name READ getVehicleFancyName NOTIFY telemetryUpdated)

public:
    static NetworkController *instance;
    static NetworkController *Create();

    QString getVehicleType();
    QString getVehicleFancyName(bool html = true);
    QStringList getSoftwareFeatures();
    bool isFeatureSupported(const QString &);
    double getBatteryStatus();
    bool isCharging();
    bool getConnectionStatus();
    bool isRemoteScriptRunning();
    bool isRemoteModeEnabled();
    double getYaw();
    double getPitch();
    double getRoll();
    double getDepth();
    double getPressure();
    double getTemperature();
    bool isUsv();
    bool isRov();
    double getLatitude();
    double getLongitude();
    double getSatellites();
    double getAltitude();
    double getSpeed();

    int getAdcCycleCount();
    int getAdcFullChargeCapacity();
    int getAdcMaxError();
    int getAdcRemainCapacity();
    int getAdcResetCount();
    int getAdcUpdateStatus();

    double getAdcTemp();
    double getAmperage();
    double getVoltage();

    bool getAdcFlagFc();
    bool getAdcFlagQen();
    bool getAdcFlagRupDis();
    bool getAdcFlagVok();
    bool getAdcFlagOcvtaken();

    QList<qreal> getFcuTelemetry();
    void setRemoteThrust(const QString &);

    void run();
    void stop();
    void remote();
    void stopRemote();
    void requestFeatures();
    void syncVehicleTime();

public slots:
    void diagnosticLog();
    void runShellCommand(QString, QStringList);
    void requestSoftwareVersion();
    void murUpgrade();
    QString getConnectionAddress();
    void saveSettings();
    void setConnectionAddress(QString);
    void setPingTime(int);
    void setPongTime(int);
    void setReconnectTime(int);
    int getPingTime();
    int getPongTime();
    int getReconnectTime();
    void batteryButton(QString cmd);

signals:
    void connectionStatusChanged();
    void telemetryUpdated();
    void notificationReceived(QString status, QString message);
    void diagnosticLogReceived(QString);
    void firmwareVersionReceived(int, int, int);

    void outputReceived(QString, QString);
    void shellOutputReceived(QString, QString);
    void timingChanged();

private:
    NetworkController();
    static qml::RegisterType<NetworkController> Register;

    void onConnected();
    void onDisconnected();
    void onReconnectTimer();
    void onTelemetryReceived(const QString &);
    void onPongReceived(quint64, const QByteArray &);
    void onPingTimeout();
    void onPongTimeout();
    void onTokenAccepted();

    QString m_connectionAddress;
    QStringList m_softwareFeatures;

    int m_pingTime;
    int m_pongTime;
    int m_reconnectTime;

    Ide::IO::Telemetry m_telemetry;
    QWebSocket *m_webSocket;
    QTimer *m_reconnectionTimer;
    QTimer *m_pingTimer;
    QTimer *m_pongTimer;
    ApiTokenDialog* m_ApiTokenDialog;
};

} 
