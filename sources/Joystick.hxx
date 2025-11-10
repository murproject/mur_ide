#pragma once
#include <QObject>
#include "QmlUtils.hxx"
#include <QQmlApplicationEngine>
#include <QTimer>
#include <GLFW/glfw3.h>


namespace Ide::Ui
{

class JoystickAxes : public QObject
{
    Q_OBJECT

public:
    enum MovementAxes {
        AxisZero,

        AxisXm,
        AxisXp,
        AxisYm,
        AxisYp,
        AxisWm,
        AxisWp,
        AxisZm,
        AxisZp,

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
    Q_PROPERTY(QVariantList allAxesBindings READ getAllAxesBindings NOTIFY rebinded)
    Q_PROPERTY(QVariantList presetNames READ getPresetNames NOTIFY presetSaved)

    Q_PROPERTY(bool isRebinding READ isRebinding NOTIFY rebindingChanged)
    Q_PROPERTY(int rebindingAxis READ getRebindingAxis NOTIFY rebindingChanged)

    Q_PROPERTY(int deadzone READ getDeadzone WRITE setDeadzone NOTIFY deadzoneChanged)
    Q_PROPERTY(double expFactor READ getExpFactor WRITE setExpFactor NOTIFY expFactorChanged)
    Q_PROPERTY(int buttonThreshold READ getButtonThreshold WRITE setButtonThreshold NOTIFY buttonThresholdChanged)
    Q_PROPERTY(bool keyboardMode READ getKeyboardMode WRITE setKeyboardMode NOTIFY keyboardModeChanged)
    Q_PROPERTY(bool calibrated READ getCalibration WRITE setCalibration NOTIFY calibrationChanged)


public:
    static Joystick *instance;
    static Joystick *Create();

    bool isJoystickConnected();
    int getAxesValue(JoystickAxes::MovementAxes, JoystickAxes::MovementAxes);

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
    QVariantList getAllAxesBindings();
    void setForceAxisValue(int, int);
    void addForceAxisValue(int, int);

    std::pair<QString, QString> getGamepadAxisName(int axis);
    QString getMovementAxisName(int axis);

    int getDeadzone();
    void setDeadzone(int);

    double getExpFactor();
    void setExpFactor(double);

    int getButtonThreshold();
    void setButtonThreshold(int);

    bool getKeyboardMode();
    void setKeyboardMode(bool);

    bool getCalibration();
    void setCalibration(bool);

    void saveSettings();
    void loadSettings();

    QVariantList getPresetNames();
    void deletePreset(QString);

    void toDefaultSettings(bool force = true);

    int calcAxisValue(int);

    void setLastPresetName(QString);
    QString getLastPresetName();

signals:
    void axesValueChanged();
    void onJoystickConnectedChanged(bool);
    void deadzoneChanged();
    void keyboardModeChanged();
    void rebindingChanged();
    void rebinded();
    void prorovFunctionsChanged();
    void expFactorChanged();
    void buttonThresholdChanged();
    void presetSaved();
    void calibrationChanged();

private:
    Joystick();
    static qml::RegisterType<Joystick> Register;

    QMap<int, double> m_gamepadAllValues;
    QMap<int, double> m_calibrationValues;

    QMap<JoystickAxes::MovementAxes, int> m_gamepadAxesBindings;
    QMap<JoystickAxes::MovementAxes, bool> m_gamepadAxesInversions;

    static void joystickConnectionCallback(int, int);
    void rebindAxis(int axis);

    void onAxisEvent(int, int, double);
    void onButtonEvent(int, int, bool);

    void onUpdateTimeout();
    void connectJoystick();

    void calibrateJoystick();
    void resetCalibration();

    bool m_isRebinding = false;
    JoystickAxes::MovementAxes m_currentlyRebindingAxis;

    int m_deadzone = 0;
    double m_expFactor = 1.0;
    int m_buttonThreshold = 50;
    double rebindThreshold = 0.5;
    bool m_isKeyboardMode = false;
    bool m_isCalibrated = false;

    QTimer* m_updateTimer;
    bool m_axisChanged = false;

    QMetaEnum metaEnum = QMetaEnum::fromType<JoystickAxes::MovementAxes>();
};
}
