#pragma once

#include <QGamepad>
#include <QObject>

#include "QmlUtils.hxx"

namespace Ide::Ui {

class GamepadAxes : public QObject
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

        AxisCount,
    };
    Q_ENUM(MovementAxes)

    static QList<QString> GamepadAxesNames;
    static QList<QString> MovementAxesNames;
};

class Gamepad : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool gamepadConnected READ isGamepadConnected NOTIFY onGamepadConnectedChanged)

    Q_PROPERTY(QList<qreal> allAxes READ getAllAxes NOTIFY axesValueChanged)
    Q_PROPERTY(QList<bool> allAxesInversions READ getAllAxesInversions NOTIFY rebinded)
    Q_PROPERTY(QList<QString> allAxesBindings READ getAllAxesBindings NOTIFY rebinded)

    Q_PROPERTY(bool isRebinding READ isRebinding NOTIFY rebindingChanged)
    Q_PROPERTY(int rebindingAxis READ getRebindingAxis NOTIFY rebindingChanged)

    Q_PROPERTY(int deadzone READ getDeadzone WRITE setDeadzone NOTIFY deadzoneChanged)
    Q_PROPERTY(double expFactor READ getExpFactor WRITE setExpFactor NOTIFY expFactorChanged)

public:
    static Gamepad *instance;
    static Gamepad *Create();

    QGamepad *getGamepad();
    bool isGamepadConnected();

public slots:
    int getAxisValue(GamepadAxes::MovementAxes);
    int getAxisValueRaw(int);
    bool getButtonValue(GamepadAxes::MovementAxes);

    bool getAxisInversion(GamepadAxes::MovementAxes axis);
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
    void onGamepadConnectedChanged(bool);
    void axesValueChanged();
    void deadzoneChanged();
    void rebindingChanged();
    void rebinded();
    void prorovFunctionsChanged();
    void expFactorChanged();

private:
    Gamepad();
    static qml::RegisterType<Gamepad> Register;

    QGamepad *m_gamepad;
    QMap<int, double> m_gamepadAllValues;

    QMap<GamepadAxes::MovementAxes, int> m_gamepadAxesBindings;
    QMap<GamepadAxes::MovementAxes, bool> m_gamepadAxesInversions;

    void rebindAxis(int axis);

    bool m_isRebinding = false;
    GamepadAxes::MovementAxes m_currentlyRebindingAxis;

    void axisEvent(int deviceId, QGamepadManager::GamepadAxis axis, double value);
    void buttonReleaseEvent(int deviceId, QGamepadManager::GamepadButton button);
    void buttonEvent(int deviceId, QGamepadManager::GamepadButton button, double value);

    int m_deadzone = 0;
    double m_expFactor = 1.0;

    QTimer* m_updateTimer;
    void onUpdateTimeout();
    bool m_axisChanged = false;

    void connectGamepad();
};

} 
