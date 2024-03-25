#pragma once
#include <QObject>
#include <QJoysticks.h>
#include "QmlUtils.hxx"
#include <QQmlApplicationEngine>
#include <QTimer>

namespace Ide::Ui
{

class JoystickAxes : public QObject
{
    Q_OBJECT

public:
    enum MovementAxes {
        AxisZero,

        AxisX,
        AxisY,
        AxisW,
        AxisZ,

        AxisYaw = AxisX,
        AxisForward = AxisY,
        AxisSide = AxisW,
        AxisDepth = AxisZ,
        L2,
        R2,

        SpeedSlow,
        SpeedFast,

        AxisCount,
    };
    Q_ENUM(MovementAxes)

    static QList<QString> JoystickAxesNames;
    static QList<QString> MovementAxesNames;
};

class Joystick : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool joystickConnected READ isJoystickConnected NOTIFY onJoystickConnectedChanged)

    Q_PROPERTY(QList<qreal> allAxes READ getAllAxes NOTIFY axesValueChanged)
    Q_PROPERTY(QList<bool> allAxesInversions READ getAllAxesInversions NOTIFY rebinded)
    Q_PROPERTY(QList<QString> allAxesBindings READ getAllAxesBindings NOTIFY rebinded)

    Q_PROPERTY(bool isRebinding READ isRebinding NOTIFY rebindingChanged)
    Q_PROPERTY(int rebindingAxis READ getRebindingAxis NOTIFY rebindingChanged)

    Q_PROPERTY(int deadzone READ getDeadzone WRITE setDeadzone NOTIFY deadzoneChanged)
    Q_PROPERTY(double expFactor READ getExpFactor WRITE setExpFactor NOTIFY expFactorChanged)


public:
    static Joystick *instance;
    static Joystick *Create();

    QJoysticks *getJoystick();
    bool isJoystickConnected();

public slots:
    int getAxisValue(JoystickAxes::MovementAxes); 
    bool getButtonValue(JoystickAxes::MovementAxes);

    bool getAxisInversion(JoystickAxes::MovementAxes axis);
    void setAxisInversion(int, bool);

    void clearAxis(int);
    void requestRebind(int);
    bool isRebinding();
    int getRebindingAxis();
    void swapAxes(int, int);

    QList<qreal> getAllAxes();
    QList<bool> getAllAxesInversions();
    QList<QString> getAllAxesBindings();
    void setForceAxisValue(int, int);
    void addForceAxisValue(int, int);

    QString getGamepadAxisName(int axis);
    QString getMovementAxisName(int axis);

    int getDeadzone();
    void setDeadzone(int);

    double getExpFactor();
    void setExpFactor(double);

    void saveSettings();
    void loadSettings();

    int calcAxisValue(int);

signals:
    void axesValueChanged();
    void onJoystickConnectedChanged(bool);
    void deadzoneChanged();
    void rebindingChanged();
    void rebinded();
    void prorovFunctionsChanged();
    void expFactorChanged();

private:
    Joystick();
    static qml::RegisterType<Joystick> Register;

    QJoysticks *m_joystick;
    QMap<int, double> m_gamepadAllValues;

    QMap<JoystickAxes::MovementAxes, int> m_gamepadAxesBindings;
    QMap<JoystickAxes::MovementAxes, bool> m_gamepadAxesInversions;

    void rebindAxis(int axis);

    bool m_isRebinding = false;
    JoystickAxes::MovementAxes m_currentlyRebindingAxis;

    void onAxisEvent(int, int, double);
    void onButtonEvent(int, int, bool);
    void onPOVEvent(int, int, int);

    int m_deadzone = 0;
    double m_expFactor = 1.0;
    int povStartIndex = 17;
    int povEndIndex = 21;

    QTimer* m_updateTimer;
    void onUpdateTimeout();
    bool m_axisChanged = false;

    void connectJoystick();
};
}
